#include "includes.h"

#define OBF_BEGIN try { obf::next_step __crv = obf::next_step::ns_done; std::shared_ptr<obf::base_rvholder> __rvlocal;

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool frameRounding = true;
float GUI_Color[4] = { 1.000f, 0.137f, 1.000f, 1.000f };

bool otherBG = true;

namespace features {
    int window_key = 45; // insert
    bool hideWindow = false;
}

namespace helpers {
    // Keymap

    const char* keys[256] = {
    "",
    "",
    "",
    "",
    "",
    "MOUSE4",
    "MOUSE5",
    "",
    "BACKSPACE",
    "TAB",
    "",
    "",
    "CLEAR",
    "ENTER",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "SPACE",
    "PGUP",
    "PGDOWN",
    "END",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "INSERT",
    "DELETE",
    "",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "",
    "",
    "",
    "",
    "",
    "NUM0",
    "NUM1",
    "NUM2",
    "NUM3",
    "NUM4",
    "NUM5",
    "NUM6",
    "NUM7",
    "NUM8",
    "NUM9",
    "MULTIPLY",
    "ADD",
    "SEPARATOR",
    "SUBTRACT",
    "DECIMAL",
    "DIVIDE",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "LSHIFT",
    "RSHIFT",
    "LCONTROL",
    "RCONTROL",
    "LMENU",
    "RMENU",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "COMMA",
    "MINUS",
    "PERIOD",
    "SLASH",
    "TILDA",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "[",
    "|",
    "]",
    "QUOTE",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    };

    //
    // Gets current key down and returns it
    //
    int getKeybind()
    {
        int i = 0;
        int n = 0;

        while (n == 0) {
            for (i = 3; i < 256; i++)
            {
                if (GetAsyncKeyState((i)))
                {
                    if (i < 256) {
                        if (!(*keys[i] == 0)) {
                            n = 1;
                            return i;
                            std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        }
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    //
    // check if the key is down.
    //
    bool keyCheck(int key)
    {
        for (int t = 3; t < 256; t++)
        {
            if (GetAsyncKeyState(t))
            {
                if ((keys[key] == keys[t]) & (keys[key] != 0 & keys[t] != 0))
                {
                    return true;
                    //std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }
            }
        }
        return false;
    }

    static auto generate_random_float = [](float min, float max) {
        float random = ((float)rand()) / (float)RAND_MAX;
        float diff = max - min;
        float r = random * diff;
        return min + r;
    };

    //
    // Implement this as a thread
    // takes in a key and a reference to a bool to toggle on or off
    // if they key you put in is true it toggles the bool you passed in.
    //
    void checkThread(int key, bool& toggle)
    {
        auto future = std::async(helpers::keyCheck, key);

        if (future.get()) {
            toggle = !toggle;
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }
}

// Side Panel State
int sidePanel = 0;
// Main code

int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T(" "), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T(" "), WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, 500, 335, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //io.Fonts->AddFontFromMemoryCompressedTTF(Ubuntu_Light_compressed_data, Ubuntu_Light_compressed_size, 20);
    io.Fonts->AddFontFromMemoryCompressedTTF(Ubuntu_Regular_compressed_data, Ubuntu_Regular_compressed_size, 18);

    std::thread clickerThread(features::clickerThread);

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (!features::hideWindow) {
            // Show the window
            ::ShowWindow(hwnd, SW_SHOWDEFAULT);
            ::UpdateWindow(hwnd);
            ::ShowWindow(GetConsoleWindow(), SW_HIDE);
        }
        else {
            // Show the window
            ::ShowWindow(hwnd, SW_HIDE);
            ::ShowWindow(GetConsoleWindow(), SW_HIDE);
        }

        {
            // press the - on Visual Studio to Ignore
            {
                ImColor mainColor = ImColor(GUI_Color[0], GUI_Color[1], GUI_Color[2], GUI_Color[3]);
                ImColor bodyColor = ImColor(int(24), int(24), int(24), 255);
                ImColor fontColor = ImColor(int(255), int(255), int(255), 255);

                ImGuiStyle& style = ImGui::GetStyle();

                ImVec4 mainColorHovered = ImVec4(mainColor.Value.x + 0.1f, mainColor.Value.y + 0.1f, mainColor.Value.z + 0.1f, mainColor.Value.w);
                ImVec4 mainColorActive = ImVec4(mainColor.Value.x + 0.2f, mainColor.Value.y + 0.2f, mainColor.Value.z + 0.2f, mainColor.Value.w);
                ImVec4 menubarColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w - 0.8f);
                ImVec4 frameBgColor = ImVec4(bodyColor.Value.x + 0.1f, bodyColor.Value.y + 0.1f, bodyColor.Value.z + 0.1f, bodyColor.Value.w + .1f);
                ImVec4 tooltipBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .05f);

                style.Alpha = 1.0f;
                style.WindowPadding = ImVec2(8, 8);
                style.WindowMinSize = ImVec2(32, 32);
                style.WindowRounding = 0.0f;
                style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
                style.ChildRounding = 0.0f;
                style.FramePadding = ImVec2(4, 3);
                style.FrameRounding = 0.0f;
                style.ItemSpacing = ImVec2(4, 3);
                style.ItemInnerSpacing = ImVec2(4, 4);
                style.TouchExtraPadding = ImVec2(0, 0);
                style.IndentSpacing = 21.0f;
                style.ColumnsMinSpacing = 3.0f;
                style.ScrollbarSize = 8.f;
                style.ScrollbarRounding = 0.0f;
                style.GrabMinSize = 1.0f;
                style.GrabRounding = 0.0f;
                style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
                style.DisplayWindowPadding = ImVec2(22, 22);
                style.DisplaySafeAreaPadding = ImVec2(4, 4);
                style.AntiAliasedLines = true;
                style.CurveTessellationTol = 1.25f;

                style.Colors[ImGuiCol_Text] = fontColor;

                style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                style.Colors[ImGuiCol_WindowBg] = bodyColor;
                style.Colors[ImGuiCol_ChildBg] = ImVec4(.0f, .0f, .0f, .0f);
                style.Colors[ImGuiCol_PopupBg] = tooltipBgColor;
                style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
                style.Colors[ImGuiCol_FrameBg] = frameBgColor;
                style.Colors[ImGuiCol_FrameBgHovered] = mainColorHovered;
                style.Colors[ImGuiCol_FrameBgActive] = mainColorActive;
                style.Colors[ImGuiCol_TitleBg] = mainColor;
                style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
                style.Colors[ImGuiCol_TitleBgActive] = mainColor;
                style.Colors[ImGuiCol_MenuBarBg] = menubarColor;
                style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .05f, frameBgColor.y + .05f, frameBgColor.z + .05f, frameBgColor.w);
                style.Colors[ImGuiCol_ScrollbarGrab] = mainColor;
                style.Colors[ImGuiCol_ScrollbarGrabHovered] = mainColorHovered;
                style.Colors[ImGuiCol_ScrollbarGrabActive] = mainColorActive;
                style.Colors[ImGuiCol_CheckMark] = mainColor;
                style.Colors[ImGuiCol_SliderGrab] = mainColorHovered;
                style.Colors[ImGuiCol_SliderGrabActive] = mainColorActive;
                style.Colors[ImGuiCol_Button] = mainColor;
                style.Colors[ImGuiCol_ButtonHovered] = mainColorHovered;
                style.Colors[ImGuiCol_ButtonActive] = mainColorActive;
                style.Colors[ImGuiCol_Header] = mainColor;
                style.Colors[ImGuiCol_HeaderHovered] = mainColorHovered;
                style.Colors[ImGuiCol_HeaderActive] = mainColorActive;

                style.Colors[ImGuiCol_ResizeGrip] = mainColor;
                style.Colors[ImGuiCol_ResizeGripHovered] = mainColorHovered;
                style.Colors[ImGuiCol_ResizeGripActive] = mainColorActive;
                style.Colors[ImGuiCol_PlotLines] = mainColor;
                style.Colors[ImGuiCol_PlotLinesHovered] = mainColorHovered;
                style.Colors[ImGuiCol_PlotHistogram] = mainColor;
                style.Colors[ImGuiCol_PlotHistogramHovered] = mainColorHovered;
                style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
            }

            // check if the keys down for keybinds
            helpers::checkThread(features::left_key, features::CLICKER_TOGGLED);
            helpers::checkThread(features::right_key, features::rCLICKER_TOGGLED);

            helpers::checkThread(features::window_key, features::hideWindow);

            // check for frame rounding
            if (frameRounding) {
                ImGui::GetStyle().FrameRounding = 4.0f;
                ImGui::GetStyle().GrabRounding = 3.0f;
            }
            else { // we do da java
                ImGui::GetStyle().FrameRounding = 0.0f;
                ImGui::GetStyle().GrabRounding = 0.0f;
            }

            // Actuall stuff

            // Setup tings
            ImGui::Begin(xorstr_("                                             "), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs);
            ImGui::SetWindowSize({ 500, 0 });
            ImGui::SetWindowPos({ 0, 0 });
            ImGui::End();

            // Left Shit
            ImGui::Begin(xorstr_("SidePanel"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration); {
                ImGui::SetWindowPos({ -5, -5 });
                ImGui::SetWindowSize({ 125, 515 });
                ImGui::Text(xorstr_("Vega - Clicker"));
                if (ImGui::Button(xorstr_("Clicker"), { 100, 50 })) sidePanel = 0;
                if (ImGui::Button(xorstr_("Misc"), { 100, 50 })) sidePanel = 1;
                //if (ImGui::Button(xorstr_("Config"), { 100, 50 })) sidePanel = 2;
                if (ImGui::Button(xorstr_("Settings"), { 100, 50 })) sidePanel = 3;
                ImGui::End();
            }

            // For drawing the gui
            ImGui::Begin(xorstr_("Canvas"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration); {
                ImGui::SetWindowPos({ 115, 5 });
                ImGui::SetWindowSize({ 450, 350 });

                if (sidePanel == 0) {
                    // Left
                    ImGui::Checkbox(xorstr_("Left Toggle"), &features::CLICKER_TOGGLED); ImGui::SameLine(); if (ImGui::Button(xorstr_("Left Bind"))) { features::left_key = helpers::getKeybind(); }
                    ImGui::SameLine(); ImGui::Text(helpers::keys[features::left_key]);

                    // drag clicker not done
                    //if (!features::lDrag) {
                    //    ImGui::SliderFloat(xorstr_("L CPS"), &features::CPS, 10.0f, 20.0f, "%.1f");
                    //    ImGui::Checkbox(xorstr_("Extra Rand"), &features::lExtraRand); ImGui::SameLine();
                    //}
                    //                     ImGui::Checkbox(xorstr_("Drag Click"), &features::lDrag);


                    ImGui::SliderFloat(xorstr_("L CPS"), &features::CPS, 10.0f, 20.0f, "%.1f");
                    ImGui::Checkbox(xorstr_("Extra Rand"), &features::lExtraRand); 


                    ImGui::NewLine();

                    // Right
                    ImGui::Checkbox(xorstr_("Right Toggle"), &features::rCLICKER_TOGGLED); ImGui::SameLine(); if (ImGui::Button(xorstr_("Right Bind"))) { features::right_key = helpers::getKeybind(); }
                    ImGui::SameLine(); ImGui::Text(helpers::keys[features::right_key]);

                    ImGui::SliderFloat(xorstr_("R CPS"), &features::rCPS, 10.0f, 20.0f, "%.1f");
                    ImGui::Checkbox(xorstr_("Extra Rand "), &features::rExtraRand);
                }
                if (sidePanel == 1) {
                    ImGui::Checkbox(xorstr_("Block Hit"), &features::BLOCKHIT); ImGui::SameLine();                     ImGui::BulletText(xorstr_("Note: This is only for Left Click"));
                    ImGui::SliderInt(xorstr_("          "), &features::BLOCK_CHANCE, 1, 25, "%d Chance");
                }
                if (sidePanel == 2) {
                    ImGui::Text(xorstr_("NOTHING YET - Configs"));
                }
                // Settings Tab
                if (sidePanel == 3) {
                    ImGui::ColorEdit4(xorstr_("                       "), GUI_Color, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoPicker);
                    ImGui::Checkbox(xorstr_("Other Background"), &otherBG); ImGui::SameLine(); ImGui::Checkbox(xorstr_("Rounding"), &frameRounding);
                    ImGui::Text(xorstr_("Application average %.3f ms/frame (%.1f FPS)"),
                        1000.0 / double(ImGui::GetIO().Framerate), double(ImGui::GetIO().Framerate));

                    ImGui::Text(xorstr_("Hide Window: ")); ImGui::SameLine();
                    if (ImGui::Button(xorstr_("Bind"))) { features::window_key = helpers::getKeybind(); }
                    ImGui::SameLine(); ImGui::Text(helpers::keys[features::window_key]);

                    ImGui::NewLine();
                    ImGui::Text(xorstr_("Vega - Clicker V2.5\nLGBTQHIV+-123456789 Edition"));
                }
                ImGui::End();
            }

            ImGui::Begin(xorstr_("DrawList"), NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

            ImGui::SetWindowPos({ 0, 0 });
            ImGui::SetWindowSize({ 800, 800 });
            ImGuiWindow* window = ImGui::GetCurrentWindow();

            window->DrawList->AddLine({ 107,  0 }, { 107,  400 }, ImGui::GetColorU32(ImVec4{ GUI_Color[0], GUI_Color[1], GUI_Color[2], GUI_Color[3] }), 3);
            ImGui::End();
        }

        // Rendering (dont touch)
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx;

        if (otherBG)    clear_col_dx = D3DCOLOR_RGBA((int)(12), (int)(22), (int)(28), (int)(255));
        else            clear_col_dx = D3DCOLOR_RGBA((int)(27), (int)(22), (int)(22), (int)(255));

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    clickerThread.detach();

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
#define OBF_END } catch(std::shared_ptr<obf::base_rvholder>& r) { return *r; } catch (...) {throw;}
