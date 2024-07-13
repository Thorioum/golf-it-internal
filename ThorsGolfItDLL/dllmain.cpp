#include "golf.h"

DWORD WINAPI MainThread(HMODULE hmodule) {

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	float speed = 2.0f;
	bool flying = false;
	Vec3 lPos{};

	DWORD procId = Memory::getProcId("GolfIt-Win64-Shipping.exe");
	if (!procId) {
		std::cout << "Couldnt find process!";
		Sleep(2000);
		return -1;
	}
	HANDLE h = 0;
	h = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	uintptr_t modulebase = Memory::getModuleBaseAddr(procId, "GolfIt-Win64-Shipping.exe");

	Golf g = Golf(procId);

	std::cout << "Flying: [OFF] (RSHIFT TO TOGGLE)" << std::endl;
	uintptr_t lastY = 0;
	lPos = { 0,0,0 };
	while (!GetAsyncKeyState(VK_END)) {

		//toggle flying
		if (GetAsyncKeyState(VK_RSHIFT)) {
			flying = !flying;

			if (flying) {
				lPos = g.getPos();
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
			g.setVelocity(Vec3(0, 5, 0));


			//quaternion math wtf (i dont dont know why this works)
			Vec4 q = g.getCameraInfo().quaternions;
			float dx = 1 - 2 * (q.y * q.y + q.z * q.z);
			float dy = 2 * (q.x * q.y - q.w * q.z);
			float dz = 2 * (q.x * q.z + q.w * q.y);
			//movement logic
			if (GetAsyncKeyState('W')) {
				lPos.x += (dx * speed);
				lPos.y += (dy * speed);
				lPos.z += (dz * speed);

			}
			if (GetAsyncKeyState('S')) {
				lPos.x -= (dx * speed);
				lPos.y -= (dy * speed);
				lPos.z -= (dz * speed);
			}
			//speed up slow down
			if (GetAsyncKeyState(VK_CONTROL)) {
				speed += 0.15;
				if (speed > 30.0f) {
					speed = 30.0f;
				}
			}
			if (GetAsyncKeyState(VK_LSHIFT)) {
				speed -= 0.15;
				if (speed < 1.0f) {
					speed = 1.0f;
				}
			}



			//set the actual pos variables to the local ones
			g.setPos(lPos);
		}
		Sleep(1);
	}

	MH_Uninitialize();
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hmodule, 0);
	return 0;
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

