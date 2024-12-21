#include "gui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include "offsets.h"

bool GUI::g_initialized = false;
HWND GUI::g_gameWindow = nullptr;
WNDPROC GUI::originalWndProc = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void GUI::Init(HDC hdc) {
    if (!g_initialized) {
        g_gameWindow = WindowFromDC(hdc);
        originalWndProc = (WNDPROC)SetWindowLongPtr(g_gameWindow, GWLP_WNDPROC, (LONG_PTR)HookWndProc);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(g_gameWindow);
        ImGui_ImplOpenGL3_Init();

        g_initialized = true;
    }
}

void GUI::Render() {
    static int value = 2147483647;
    static char valueBuffer[16] = "2147483647";
    static DWORD_PTR localPlayer = 0;
    static bool infiniteHealth = false;
    static bool infiniteAmmo = false;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("AssaultHax", nullptr, ImGuiWindowFlags_NoCollapse);
    if (ImGui::InputText("Value", valueBuffer, sizeof(valueBuffer), ImGuiInputTextFlags_CharsDecimal)) {
        value = atoi(valueBuffer);
    }
    ReadProcessMemory(GetCurrentProcess(), (LPCVOID)((DWORD_PTR)GetModuleHandle(NULL) + offsets::local_player),
        &localPlayer, sizeof(localPlayer), 0);

    if (ImGui::Checkbox("Infinite Health", &infiniteHealth) && infiniteHealth && localPlayer) {
        WriteProcessMemory(GetCurrentProcess(), (LPVOID)(localPlayer + offsets::health_off), &value, sizeof(value), 0);
    }

    if (ImGui::Checkbox("Infinite Ammo", &infiniteAmmo) && infiniteAmmo && localPlayer) {
        WriteProcessMemory(GetCurrentProcess(), (LPVOID)(localPlayer + offsets::assault_rifle_off), &value, sizeof(value), 0);
        WriteProcessMemory(GetCurrentProcess(), (LPVOID)(localPlayer + offsets::pistol_off), &value, sizeof(value), 0);
    }

    if (localPlayer) {
        if (infiniteHealth) {
            WriteProcessMemory(GetCurrentProcess(), (LPVOID)(localPlayer + offsets::health_off), &value, sizeof(value), 0);
        }
        if (infiniteAmmo) {
            WriteProcessMemory(GetCurrentProcess(), (LPVOID)(localPlayer + offsets::assault_rifle_off), &value, sizeof(value), 0);
            WriteProcessMemory(GetCurrentProcess(), (LPVOID)(localPlayer + offsets::pistol_off), &value, sizeof(value), 0);
        }
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK GUI::HookWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;
    return CallWindowProc(originalWndProc, hwnd, uMsg, wParam, lParam);
}

void GUI::Cleanup() {
    if (g_initialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        if (originalWndProc && g_gameWindow) {
            SetWindowLongPtr(g_gameWindow, GWLP_WNDPROC, (LONG_PTR)originalWndProc);
        }
    }
}
