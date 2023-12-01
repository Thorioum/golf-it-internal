#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <psapi.h>
#include <TlHelp32.h>
#include <functional>

namespace mem {
    //"GolfIt-Win64-Shipping.exe"+0x059AD180
    std::vector <unsigned int> xoffsets = { 0x0,0x20,0x390,0x408,0x120,0x80,0x50, 0x20 };
    std::vector <unsigned int> zoffsets = { 0x0,0x20,0x390,0x408,0x120,0x80,0x50, 0x24 };
    std::vector <unsigned int> yoffsets = { 0x0,0x20,0x390,0x408,0x120,0x80,0x50, 0x28 };

    std::vector <unsigned int> xveloffsets = { 0x0,0x20,0x390,0x408,0x120,0x80,0x50, 0x50 };
    std::vector <unsigned int> zveloffsets = { 0x0,0x20,0x390,0x408,0x120,0x80,0x50, 0x54 };
    std::vector <unsigned int> yveloffsets = { 0x0,0x20,0x390,0x408,0x120,0x80,0x50, 0x58 };
    //"GolfIt-Win64-Shipping.exe"+05850170
    std::vector <unsigned int> cameraQuatXOffsets = { 0x28,0x2C0,0x390,0x28,0x130, 0x0 };
    std::vector <unsigned int> cameraQuatZOffsets = { 0x28,0x2C0,0x390,0x28,0x130, 0x4 };
    std::vector <unsigned int> cameraQuatYOffsets = { 0x28,0x2C0,0x390,0x28,0x130, 0x8 };
    std::vector <unsigned int> cameraQuatWOffsets = { 0x28,0x2C0,0x390,0x28,0x130, 0xC };
  
    std::vector <unsigned int> cameraXOffsets = { 0x28,0x2C0,0x390,0x28,0x130, 0x10 };
    std::vector <unsigned int> cameraZOffsets = { 0x28,0x2C0,0x390,0x28,0x130, 0x14 };
    std::vector <unsigned int> cameraYOffsets = { 0x28,0x2C0,0x390,0x28,0x130, 0x18 };

}


class Memory {
public:
    static DWORD GetProcId(const char* procName)
    {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snap != INVALID_HANDLE_VALUE) {

            PROCESSENTRY32 procEntry;
            procEntry.dwSize = sizeof(procEntry);

            Process32First(snap, &procEntry);
            do {
                if (strcmp(procEntry.szExeFile, procName) == 0) {
                    CloseHandle(snap);
                    return procEntry.th32ProcessID;
                }
            } while (Process32Next(snap, &procEntry));
        }
        CloseHandle(snap);
        return 0;
    }

    static uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName)
    {
        uintptr_t modBaseAddr = 0;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
        if (hSnap != INVALID_HANDLE_VALUE)
        {
            MODULEENTRY32 modEntry;
            modEntry.dwSize = sizeof(modEntry);
            if (Module32First(hSnap, &modEntry))
            {
                do
                {
                    if (!strcmp(modEntry.szModule, modName))
                    {
                        modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                        break;
                    }
                } while (Module32Next(hSnap, &modEntry));
            }
        }
        CloseHandle(hSnap);
        return modBaseAddr;
    }
    static uintptr_t getPtrWithOffsets(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
    {
        uintptr_t addr = ptr;
        for (unsigned int i = 0; i < offsets.size(); ++i)
        {
            ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
            addr += offsets[i];
        }
        return addr;
    }

    static int readInt(HANDLE proc, uintptr_t ptrVal) {
        int val = 0;
        ReadProcessMemory(proc, (BYTE*)ptrVal, &val, sizeof(val), nullptr);
        return val;
    }
    static double readDouble(HANDLE proc, uintptr_t ptrVal) {
        double val = 0;
        ReadProcessMemory(proc, (BYTE*)ptrVal, &val, sizeof(val), nullptr);
        return val;
    }
    static float readFloat(HANDLE proc, uintptr_t ptrVal) {
        float val = 0;
        ReadProcessMemory(proc, (BYTE*)ptrVal, &val, sizeof(val), nullptr);
        return val;
    }
    static char readChar(HANDLE proc, uintptr_t ptrVal) {
        char val = 0;
        ReadProcessMemory(proc, (BYTE*)ptrVal, &val, sizeof(val), nullptr);
        return val;
    }
    static long readLong(HANDLE proc, uintptr_t ptrVal) {
        long val = 0;
        ReadProcessMemory(proc, (BYTE*)ptrVal, &val, sizeof(val), nullptr);
        return val;
    }
    static bool readBool(HANDLE proc, uintptr_t ptrVal) {
        bool val = 0;
        ReadProcessMemory(proc, (BYTE*)ptrVal, &val, sizeof(val), nullptr);
        return val;
    }

};