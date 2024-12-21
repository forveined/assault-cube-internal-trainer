#pragma once
#include <windows.h>
#include "imgui.h"
#include <stdio.h>

class GUI {
public:
    static void Init(HDC hdc);
    static void Render();
    static bool IsInitialized() { return g_initialized; }
    static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void Cleanup();

private:
    static bool g_initialized;
    static HWND g_gameWindow;
    static WNDPROC originalWndProc;
};