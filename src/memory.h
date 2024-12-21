#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include<iostream>
extern HANDLE gameProcess;

namespace memory {
    template<typename T>
    T Read(DWORD address) {
        return *((T*)address);
    }
    template<typename T>
    void Write(DWORD address, T value) {
        *((T*)address) = value;
    }
    DWORD GetProcessId(const wchar_t* processName);
    DWORD GetModuleBaseAddress(DWORD procId, const wchar_t* modName = nullptr);
    bool WriteValue(HANDLE process, DWORD_PTR address, const int& value);
    bool ReadValue(HANDLE process, DWORD_PTR address, DWORD_PTR& value);
}