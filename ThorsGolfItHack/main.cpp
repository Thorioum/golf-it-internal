#define WIN32_LEAN_AND_MEAN

#include "memory.h"
float speed = 4.0f;

float speedx = 4.0f;
float speedz = 4.0f;
float speedy = 4.0f;
bool flying = false;
float localx, localy, localz;
int main() {
	DWORD procId = Memory::GetProcId("GolfIt-Win64-Shipping.exe");
	if (!procId) {
		std::cout << "Couldnt find process!";
		Sleep(2000);
		return -1;
	}
	HANDLE h = 0;
	h = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
	
	uintptr_t modulebase = Memory::GetModuleBaseAddress(procId, "GolfIt-Win64-Shipping.exe");

	uintptr_t dynamicPtrBaseAddr = modulebase + 0x05948FE8;


	std::cout << "Flying: [OFF]";
	uintptr_t lastY = 0;
	while (!GetAsyncKeyState(VK_END)) {
		//get position addresses
		uintptr_t xAddress = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::xoffsets);
		uintptr_t yAddress = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::yoffsets);
		uintptr_t zAddress = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::zoffsets);

		//toggle flying
		if (GetAsyncKeyState(VK_RSHIFT)) {
			flying = !flying;
			
			if (flying) {
				localx = Memory::readFloat(h, xAddress);
				localy = Memory::readFloat(h, yAddress);
				localz = Memory::readFloat(h, zAddress);
				std::cout << "Flying: [ON]" << std::endl;;
			}
			else {
				std::cout << "Flying: [OFF]" << std::endl;
			}
			Sleep(200);
		}
		if (!flying) {
			continue;
		}
		//set all velocity near 0
		//if we set all velocity to zero it will detect the ball has stoped rolling and teleport end the stroke
		/*float n = 24;
		WriteProcessMemory(h, (LPVOID)Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::xveloffsets), &n, sizeof(localx), nullptr);
		WriteProcessMemory(h, (LPVOID)Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::yveloffsets), &n, sizeof(localx), nullptr);
		WriteProcessMemory(h, (LPVOID)Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::zveloffsets), &n, sizeof(localx), nullptr);*/

		//every time the hole is changed the address of the position does as well, update local position values to match the spawn
		if (lastY != yAddress) {
			lastY = yAddress;
			localx = Memory::readFloat(h, xAddress);
			localy = Memory::readFloat(h, yAddress);
			localz = Memory::readFloat(h, zAddress);
		}

		//movement logic
		if (GetAsyncKeyState('W')) {
			if (speedx < speed*3) {
				speedx += 0.1f;
			}
			if (speedx < 0) {
				speedx = speed;
			}
			localx += speedx;
		}
		if (GetAsyncKeyState('S')) {
			if (speedx > -speed * 3) {
				speedx -= 0.1f;
			}
			if (speedx > 0) {
				speedx = -speed;
			}
			localx += speedx;
		}
		if (GetAsyncKeyState('A')) {
			if (speedz > -speed * 3) {
				speedz -= 0.1f;
			}
			if (speedz > 0) {
				speedz = -speed;
			}
			localz += speedz;
		}
		if (GetAsyncKeyState('D')) {
			if (speedz < speed * 3) {
				speedz += 0.1f;
			}
			if (speedz < 0) {
				speedz = speed;
			}
			localz += speedz;
		}
		if (GetAsyncKeyState(VK_CONTROL)) {
			if (speedy < speed * 3) {
				speedy += 0.1f;
			}
			if (speedy < 0) {
				speedy = speed;
			}
			localy += speedy;
		}
		if (GetAsyncKeyState(VK_LSHIFT)) {
			if (speedy > -speed * 3) {
				speedy -= 0.1f;
			}
			if (speedy > 0) {
				speedy = -speed;
			}
			localy += speedy;
		}


		//set the actual pos variables to the local ones
		WriteProcessMemory(h, (LPVOID)xAddress, &localx, sizeof(localx), nullptr);
		WriteProcessMemory(h, (LPVOID)yAddress, &localy, sizeof(localy), nullptr);
		WriteProcessMemory(h, (LPVOID)zAddress, &localz, sizeof(localz), nullptr);
		
		Sleep(1);
	}

}