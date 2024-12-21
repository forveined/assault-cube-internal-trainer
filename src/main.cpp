#include <windows.h>
#include "hook.h"
#include "gui.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Hook::Initialize, nullptr, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        Hook::Cleanup();
        break;
    }
    return TRUE;
}