#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <psapi.h>
#include <TlHelp32.h>

namespace mem {
    std::vector <unsigned int> xoffsets = { 0x8,0x0,0x58,0x0,0x0,0x88, 0x10 };
    std::vector <unsigned int> zoffsets = { 0x8,0x0,0x58,0x0,0x0,0x88, 0x14 };
    std::vector <unsigned int> yoffsets = { 0x8,0x0,0x58,0x0,0x0,0x88, 0x18 };

    std::vector <unsigned int> xveloffsets = { 0x8,0x0,0x58,0x0,0x0,0x88, 0x40 };
    std::vector <unsigned int> zveloffsets = { 0x8,0x0,0x58,0x0,0x0,0x88, 0x44 };
    std::vector <unsigned int> yveloffsets = { 0x8,0x0,0x58,0x0,0x0,0x88, 0x48 };
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
    static float readChar(HANDLE proc, uintptr_t ptrVal) {
        char val = 0;
        ReadProcessMemory(proc, (BYTE*)ptrVal, &val, sizeof(val), nullptr);
        return val;
    }
    static float readLong(HANDLE proc, uintptr_t ptrVal) {
        long val = 0;
        ReadProcessMemory(proc, (BYTE*)ptrVal, &val, sizeof(val), nullptr);
        return val;
    }

};