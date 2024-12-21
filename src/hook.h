#pragma once
#include <windows.h>
#include "MinHook.h"
#pragma comment(lib, "libMinHook-x86-v141-md.lib")  


class Hook {
public:
    static void Initialize();
    static void Cleanup();
    static BOOL WINAPI HookwglSwapBuffers(HDC hdc);

private:
    static void CreateDebugWindow();
    static HANDLE gameProcess;
    static DWORD_PTR baseAddress;
    static bool showMenu;
};