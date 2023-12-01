#define WIN32_LEAN_AND_MEAN

#include "golf.h"


int main() {
	float speed = 10.0f;
	bool flying = false;
	Vec3 localPos{};

	DWORD procId = Memory::GetProcId("GolfIt-Win64-Shipping.exe");
	if (!procId) {
		std::cout << "Couldnt find process!";
		Sleep(2000);
		return -1;
	}
	HANDLE h = 0;
	h = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
	
	uintptr_t modulebase = Memory::GetModuleBaseAddress(procId, "GolfIt-Win64-Shipping.exe");

	Golf g = Golf(h, procId);

	std::cout << "Flying: [OFF] (RSHIFT TO TOGGLE)" << std::endl;
	uintptr_t lastY = 0;
	localPos = g.getPos();
	while (!GetAsyncKeyState(VK_END)) {

		//toggle flying
		if (GetAsyncKeyState(VK_RSHIFT)) {
			flying = !flying;
			
			if (flying) {
				localPos = g.getPos();
				std::cout << "Flying: [ON]" << std::endl;
			}
			else {
				std::cout << "Flying: [OFF]" << std::endl;
			}
			Sleep(200);
		}
		
		if (flying) {
			//set all velocity near 0
			//if we set all velocity to zero it will detect the ball has stoped rolling and teleport end the stroke
			g.setVel(Vec3(24, 24, 24));
			

			//quaternion math wtf (i dont dont know why this works)
			Vec4 q = g.camera.getQuat();
			float dx = 1 - 2 * (q.y * q.y + q.z * q.z);
			float dy = 2 * (q.x * q.y - q.w * q.z);
			float dz = 2 * (q.x * q.z + q.w * q.y);

			//movement logic
			if (GetAsyncKeyState('W')) {
				localPos.x += (dx * speed);
				localPos.y += (dy * speed);
				localPos.z += (dz * speed);

			}
			if (GetAsyncKeyState('S')) {
				localPos.x -= (dx * speed);
				localPos.y -= (dy * speed);
				localPos.z -= (dz * speed);
			}
			//speed up slow down
			if (GetAsyncKeyState(VK_CONTROL)) {
				speed += 0.25;
				if (speed > 30.0f) {
					speed = 30.0f;
				}
			}
			if (GetAsyncKeyState(VK_LSHIFT)) {
				speed -= 0.25;
				if (speed < 4.0f) {
					speed = 4.0f;
				}
			}
			


			//set the actual pos variables to the local ones
			localPos = g.updateIfAddrChanged(localPos);
			g.setPos(localPos);
		}
		Sleep(1);
	}

	CloseHandle(h);
}