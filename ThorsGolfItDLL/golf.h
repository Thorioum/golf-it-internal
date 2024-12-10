#pragma once
#include "Scanner.h"
#include "Memory.h"
#include <Windows.h>
#include <winDNS.h>
#include "MinHook.h"
#include <Intsafe.h>
#include <emmintrin.h>
#include <stdio.h>
#include <__msvc_int128.hpp>
#pragma comment(lib, "MinHook.x64.lib")
struct Vec3 {
	float x;
	float y;
	float z;
};
struct Vec4 {
	float x;
	float z;
	float y;
	float w;
};
struct cameraInfo {
	Vec4 quaternions;
	Vec3 position;
};
cameraInfo camInfo = { 0,0,0,0,0,0,0 };
uintptr_t* pPosition = nullptr;


typedef __int64 (__fastcall* tickBall)(__int64 ball);
tickBall pFuncBall = nullptr;
__int64 __fastcall detourTick(__int64 ball) {

	__int64 lastOrentation = *(QWORD*)(*(QWORD*)(ball + 0x38) + 8 * 0);
	DWORD* pOrientation = *(DWORD**)(lastOrentation + 0x28);
	pPosition = (uintptr_t*)pOrientation;


	return pFuncBall(ball);
}


typedef __int64(__fastcall* cameraFunc)(__m128* camera);
cameraFunc pFuncCam = nullptr;
__int64 __fastcall detourCameraFunc(__m128* camera) {

	unsigned __int64 v9 = camera->m128_u64[0] + 304;
	__int64 v19 = *(QWORD*)v9 + 192i64 * camera->m128_i32[3];
	camInfo = *(cameraInfo*)v19;
	return pFuncCam(camera);
}

class Golf {
public:

	uintptr_t golfBase;
	uintptr_t physicsBase;
	BYTE* golfModule;
	BYTE* physicsModule;
	DWORD procId = 0;
	HANDLE h = GetModuleHandle(NULL);

public:
	
	Golf() = delete;

	Golf(DWORD procId) {
		this->procId = procId;
		this->golfBase = Memory::getModuleBaseAddr(procId, "GolfIt-Win64-Shipping.exe");
		this->physicsBase = Memory::getModuleBaseAddr(procId, "PhysX_64.dll");
		this->golfModule = (BYTE*)GetModuleHandle("GolfIt-Win64-Shipping.exe");
		this->physicsModule = (BYTE*)GetModuleHandle("PhysX_64.dll");

		//pattern scan for cameraFunc
		ScanData signature = ScanData("4C 8B DC 55 57 41 57 49 8D 6B ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B 01");
		size_t size = Memory::getModuleSize(procId, "GolfIt-Win64-Shipping.exe");
		ScanData data = ScanData(size);
		uintptr_t moduleBaseAddr = Memory::getModuleBaseAddr(procId, "GolfIt-Win64-Shipping.exe");
		HANDLE golfItHandle = GetCurrentProcess();
		Memory::read(golfItHandle, moduleBaseAddr, data.data, data.size);
		uintptr_t cameraAddr = Scanner::scan(signature, data);
		std::cout << "Found Camera At: " << cameraAddr << std::endl;

		//

		if (MH_Initialize() != MH_OK) {
			std::cout << "minhook failed to init" << std::endl;
			return;
		}
		//hooks
		//setBallHook
		MH_STATUS status1 = MH_CreateHook(reinterpret_cast<void**>(physicsBase + 0xCA9C0), &detourTick, reinterpret_cast<void**>(&pFuncBall));
		//setCameraHook
		MH_STATUS status2 = MH_CreateHook(reinterpret_cast<void**>(golfBase + cameraAddr), &detourCameraFunc, reinterpret_cast<void**>(&pFuncCam));
		
		MH_EnableHook(MH_ALL_HOOKS);
	}
	const Vec3 getPos() {
		if (!pPosition) return { 0,0,0 };
		uintptr_t* xAddr = (uintptr_t*)((char*)(pPosition) + 0x10);
		uintptr_t* zAddr = (uintptr_t*)((char*)(pPosition) + 0x14);
		uintptr_t* yAddr = (uintptr_t*)((char*)(pPosition) + 0x18);
		
		if (!xAddr || !zAddr || !yAddr) return { 0,0,0 };
		return Vec3(*(float*)(xAddr), *(float*)yAddr, *(float*)zAddr);
	}
	void setPos(Vec3 newPos) {
		if (!pPosition) return;
		uintptr_t* xAddr = (uintptr_t*)((char*)(pPosition)+0x10);
		uintptr_t* zAddr = (uintptr_t*)((char*)(pPosition)+0x14);
		uintptr_t* yAddr = (uintptr_t*)((char*)(pPosition)+0x18);
		
		if (!xAddr || !zAddr || !yAddr) return;
		*(float*)xAddr = newPos.x;
		*(float*)yAddr = newPos.y;
		*(float*)zAddr = newPos.z;

	}
	const Vec3 getVelocity() {
		if (!pPosition) return { 0,0,0 };
		uintptr_t* xVelAddr = (uintptr_t*)((char*)(pPosition)+0x40);
		uintptr_t* zVelAddr = (uintptr_t*)((char*)(pPosition)+0x44);
		uintptr_t* yVelAddr = (uintptr_t*)((char*)(pPosition)+0x48);
		if (!xVelAddr || !zVelAddr || !yVelAddr) return { 0,0,0 };
		return Vec3(*(float*)(xVelAddr), *(float*)yVelAddr, *(float*)zVelAddr);
	}
	void setVelocity(Vec3 newVel) {
		if (!pPosition) return;
		uintptr_t* xVelAddr = (uintptr_t*)((char*)(pPosition)+0x40);
		uintptr_t* zVelAddr = (uintptr_t*)((char*)(pPosition)+0x44);
		uintptr_t* yVelAddr = (uintptr_t*)((char*)(pPosition)+0x48);

		if (!xVelAddr || !zVelAddr || !yVelAddr) return;
		*(float*)xVelAddr = newVel.x;
		*(float*)yVelAddr = newVel.y;
		*(float*)zVelAddr = newVel.z;
	}
	const cameraInfo getCameraInfo() {
		return camInfo;
	}
};