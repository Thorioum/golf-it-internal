#define WIN32_LEAN_AND_MEAN

#include "Memory.h"


int main() {

	DWORD procId = Memory::getProcId("GolfIt-Win64-Shipping.exe");

	if (procId) {
		std::cout << procId;

		//check test dll source
		wchar_t szPath[MAX_PATH];
		GetModuleFileNameW(NULL, szPath, MAX_PATH);
		std::filesystem::path workingDir = std::filesystem::path{ szPath }.parent_path();

		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
		std::string asgardDLLPath = workingDir.string() + "\\Asgard.dll";
		std::string minHook = workingDir.string() + "\\MinHook.x64.dll";
		std::string golfItDLLPath = workingDir.string() + "\\ThorsGolfItDLL.dll";		

		bool a = Memory::simpleInject(asgardDLLPath.c_str(), handle);
		bool c = Memory::simpleInject(minHook.c_str(), handle);
		bool b = Memory::simpleInject(golfItDLLPath.c_str(), handle);

	}
	else {
		std::cout << "Golf it application not found!";
	}
	return 0;
}