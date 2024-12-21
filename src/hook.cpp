#include "hook.h"
#include "gui.h"
#include "offsets.h"
#include <stdio.h>

typedef BOOL(WINAPI* twglSwapBuffers)(HDC hdc);


HANDLE Hook::gameProcess = nullptr;
DWORD_PTR Hook::baseAddress = 0;
bool Hook::showMenu = true;
static twglSwapBuffers originalSwapBuffers = nullptr;

void Hook::Initialize() {
    CreateDebugWindow();
    printf("DLL Injected. Waiting for OpenGL...\n");

    HMODULE openGL = nullptr;
    while (!openGL) {
        openGL = GetModuleHandle(L"opengl32.dll");
        if (!openGL) Sleep(100);
    }
    MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK) {
        fprintf(stderr, "Failed to initialize MinHook. Error code: %d\n", initStatus);
        return;
    }

    PVOID swapBuffers = GetProcAddress(openGL, "wglSwapBuffers");
    if (!swapBuffers) {
        fprintf(stderr, "Could not find wglSwapBuffers function\n");
        return;
    }

    MH_STATUS hookStatus = MH_CreateHook(swapBuffers, &HookwglSwapBuffers, (LPVOID*)&originalSwapBuffers);
    if (hookStatus != MH_OK) {
        fprintf(stderr, "Failed to create hook. Error code: %d\n", hookStatus);
        return;
    }

    MH_STATUS enableStatus = MH_EnableHook(swapBuffers);
    if (enableStatus != MH_OK) {
        fprintf(stderr, "Failed to enable hook. Error code: %d\n", enableStatus);
        return;
    }

    printf("Hook installed successfully!\n");
}

BOOL WINAPI Hook::HookwglSwapBuffers(HDC hdc) {
    static bool firstCall = true;
    if (firstCall) {
        GUI::Init(hdc);
        baseAddress = (DWORD_PTR)GetModuleHandle(NULL);
        gameProcess = GetCurrentProcess();
        firstCall = false;
    }

    if (!GUI::IsInitialized()) return originalSwapBuffers(hdc);
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        showMenu = !showMenu;
    }
    if (showMenu) {
        GUI::Render();
    }

    return originalSwapBuffers(hdc);
}

void Hook::Cleanup() {
    GUI::Cleanup();
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

void Hook::CreateDebugWindow() {
    AllocConsole();
    FILE* pCout;
    freopen_s(&pCout, "CONOUT$", "w", stdout);
}