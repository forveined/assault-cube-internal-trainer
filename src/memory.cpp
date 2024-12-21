#include "memory.h"

namespace memory {
    DWORD GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
        DWORD modBaseAddr = 0;
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
        if (snap != INVALID_HANDLE_VALUE) {
            MODULEENTRY32W modEntry;
            modEntry.dwSize = sizeof(modEntry);
            if (Module32FirstW(snap, &modEntry)) {
                do {
                    if (_wcsicmp(modEntry.szModule, modName) == 0) {
                        modBaseAddr = (DWORD)modEntry.modBaseAddr;
                        break;
                    }
                } while (Module32NextW(snap, &modEntry));
            }
            CloseHandle(snap);
        }
        return modBaseAddr;
    }

    DWORD GetProcessId(const wchar_t* processName) {
        DWORD pid = 0;
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32W pe = { sizeof(pe) };
            for (BOOL ok = Process32FirstW(snap, &pe); ok; ok = Process32NextW(snap, &pe))
                if (_wcsicmp(pe.szExeFile, processName) == 0) {
                    pid = pe.th32ProcessID;
                    break;
                }
            CloseHandle(snap);
        }
        return pid;
    }

    bool WriteValue(HANDLE process, DWORD_PTR address, const int& value) {
        return WriteProcessMemory(process, (LPVOID)address, &value, sizeof(value), 0);
    }

    bool ReadValue(HANDLE process, DWORD_PTR address, DWORD_PTR& value) {
        return ReadProcessMemory(process, (LPCVOID)address, &value, sizeof(value), 0);
    }
}