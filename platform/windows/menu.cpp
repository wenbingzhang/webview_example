//
// Created by wenbing on 2025/2/18.
//

#include "menu.h"

WNDPROC originalWndProc = nullptr;
// 自定义窗口过程
LRESULT CALLBACK SubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_USER: // 自定义消息（由uCallbackMessage定义）
            if (lParam == WM_RBUTTONDOWN) {
                // 创建右键菜单
                HMENU hMenu = CreatePopupMenu();
                AppendMenuW(hMenu, MF_STRING, 1002, L"退出");

                // 显示菜单
                POINT pt;
                GetCursorPos(&pt);
                SetForegroundWindow(hWnd); // 确保菜单能正确关闭
                TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, nullptr);
                DestroyMenu(hMenu);
            }
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1001:
                    MessageBoxW(hWnd, L"content", L"title", MB_OK);
                    return 0;
                case 1002:
                    PostQuitMessage(0);
                    return 0;
            }
            break;
        case WM_CLOSE:
        {
            NOTIFYICONDATA nid = {0};
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = hWnd;      // 必须与添加图标时的hWnd一致
            nid.uID = IDR_MAINFRAME;          // 必须与添加图标时的uID一致
            Shell_NotifyIcon(NIM_DELETE, &nid);
            break;
        }
        default:
            break;
    }
    return CallWindowProc(originalWndProc, hWnd, msg, wParam, lParam);
}

Menu::Menu(HWND hWnd) {
    m_hWnd = hWnd;
}

Menu::~Menu() {
    m_hWnd = nullptr;
}

void Menu::createMenu() {
    auto hMenu = CreateMenu();
    auto hFileMenu = CreatePopupMenu();
    AppendMenuW(hFileMenu, MF_STRING, 1001,L"open");
    AppendMenuW(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,L"file");
    SetMenu(m_hWnd, hMenu);

    originalWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)SubclassProc);

    // 1. 在窗口创建后修改背景色（参考 ）
    SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(255, 255, 255)));
    // 2. 强制重绘窗口
    InvalidateRect(m_hWnd, nullptr, TRUE);
    UpdateWindow(m_hWnd);
}

void Menu::createSystemTray() {
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = m_hWnd;                // 关联的窗口句柄
    nid.uID = IDR_MAINFRAME;                    // 图标唯一标识符
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; // 启用图标、消息、提示
    nid.uCallbackMessage = WM_USER; // 自定义消息标识（用于响应事件）
    nid.hIcon = LoadIcon(nullptr, IDI_APPLICATION); //LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_SMALL)); // 加载图标资源
    lstrcpyW(nid.szTip, L"webview"); // 悬停提示文本

    Shell_NotifyIcon(NIM_ADD, &nid); // 添加图标到系统托盘
}