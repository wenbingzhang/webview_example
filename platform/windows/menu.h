//
// Created by wenbing on 2025/2/18.
//

#ifndef WEBVIEW_EXAMPLE_MENU_H
#define WEBVIEW_EXAMPLE_MENU_H
#include <windows.h>

#define IDR_MAINFRAME 1

class Menu {
public:
    Menu(HWND hWnd);  // 构造函数
    ~Menu(); // 析构函数

    // 创建菜单
    void createMenu();
    void createSystemTray();
private:
    HWND m_hWnd;
};


#endif //WEBVIEW_EXAMPLE_MENU_H
