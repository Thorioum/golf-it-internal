#pragma once
#include "memory.h"
struct Vec3 {
	float x;
	float y;
	float z;
};
struct Vec4 {
	float x;
	float y;
	float z;
	float w;
};
class Golf {
public:

	uintptr_t golfBase;
	uintptr_t physicsBase;
	HANDLE h = 0;
public:
	
	Golf() = delete;

	Golf(HANDLE& h, DWORD procId) : camera(*this) {
		this->h = h;
		this->golfBase = Memory::GetModuleBaseAddress(procId, "GolfIt-Win64-Shipping.exe");
		this->physicsBase = Memory::GetModuleBaseAddress(procId, "PhysX_64.dll");
	}
	const Vec3 getPos() {
		uintptr_t dynamicPtrBaseAddr = golfBase + 0x059AD180;
		uintptr_t xAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::xoffsets);
		uintptr_t yAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::yoffsets);
		uintptr_t zAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::zoffsets);
		float x = Memory::readFloat(h, xAddr);
		float y = Memory::readFloat(h, yAddr);
		float z = Memory::readFloat(h, zAddr);
		return Vec3(x, y, z);
	}
	void setPos(Vec3 pos) {
		uintptr_t dynamicPtrBaseAddr = golfBase + 0x059AD180;
		uintptr_t xAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::xoffsets);
		uintptr_t yAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::yoffsets);
		uintptr_t zAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::zoffsets);
		WriteProcessMemory(h, (LPVOID)xAddr, &pos.x, sizeof(pos.x), nullptr);
		WriteProcessMemory(h, (LPVOID)yAddr, &pos.y, sizeof(pos.y), nullptr);
		WriteProcessMemory(h, (LPVOID)zAddr, &pos.z, sizeof(pos.z), nullptr);
	}
	const Vec3 getVel() {
		uintptr_t dynamicPtrBaseAddr = golfBase + 0x059AD180;
		uintptr_t xAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::xveloffsets);
		uintptr_t yAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::yveloffsets);
		uintptr_t zAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::zveloffsets);
		float x = Memory::readFloat(h, xAddr);
		float y = Memory::readFloat(h, yAddr);
		float z = Memory::readFloat(h, zAddr);
		return Vec3(x, y, z);
	}
	void setVel(Vec3 pos) {
		uintptr_t dynamicPtrBaseAddr = golfBase + 0x059AD180;
		uintptr_t xAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::xveloffsets);
		uintptr_t yAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::yveloffsets);
		uintptr_t zAddr = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::zveloffsets);
		WriteProcessMemory(h, (LPVOID)xAddr, &pos.x, sizeof(pos.x), nullptr);
		WriteProcessMemory(h, (LPVOID)yAddr, &pos.y, sizeof(pos.y), nullptr);
		WriteProcessMemory(h, (LPVOID)zAddr, &pos.z, sizeof(pos.z), nullptr);
	}

	Vec3 updateIfAddrChanged(Vec3 pos) {
		uintptr_t dynamicPtrBaseAddr = golfBase + 0x05948FE8;
		uintptr_t current = Memory::getPtrWithOffsets(h, dynamicPtrBaseAddr, mem::xveloffsets);
		if (lastAddr != current) {
			lastAddr = current;
			return getPos();
		}
		return pos;
	}
public:
	
	class Camera {
		public:

		Camera() = delete;

		Camera(Golf& golf) : golf(golf) {}

		const Vec3 getPos() {
			uintptr_t dynamicPtrBaseAddr = golf.golfBase + 0x05850170;
			uintptr_t xAddr = Memory::getPtrWithOffsets(golf.h, dynamicPtrBaseAddr, mem::cameraXOffsets);
			uintptr_t yAddr = Memory::getPtrWithOffsets(golf.h, dynamicPtrBaseAddr, mem::cameraYOffsets);
			uintptr_t zAddr = Memory::getPtrWithOffsets(golf.h, dynamicPtrBaseAddr, mem::cameraZOffsets);
			float x = Memory::readFloat(golf.h, xAddr);
			float y = Memory::readFloat(golf.h, yAddr);
			float z = Memory::readFloat(golf.h, zAddr);
			return Vec3(x, y, z);
		}
		const Vec4 getQuat() {
			uintptr_t dynamicPtrBaseAddr = golf.golfBase + 0x05850170;
			uintptr_t xAddr = Memory::getPtrWithOffsets(golf.h, dynamicPtrBaseAddr, mem::cameraQuatXOffsets);
			uintptr_t zAddr = Memory::getPtrWithOffsets(golf.h, dynamicPtrBaseAddr, mem::cameraQuatZOffsets);
			uintptr_t yAddr = Memory::getPtrWithOffsets(golf.h, dynamicPtrBaseAddr, mem::cameraQuatYOffsets);
			uintptr_t wAddr = Memory::getPtrWithOffsets(golf.h, dynamicPtrBaseAddr, mem::cameraQuatWOffsets);
			float x = Memory::readFloat(golf.h, xAddr);
			float z = Memory::readFloat(golf.h, zAddr);
			float y = Memory::readFloat(golf.h, yAddr);
			float w = Memory::readFloat(golf.h, wAddr);
			return Vec4(x, y, z, w);
		}

		private:
			Golf& golf;
	};
	Camera camera;
private:
	
	uintptr_t lastAddr = 0;
};