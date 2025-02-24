//
// Created by 张文兵 on 2025/2/14.
//

#ifndef APP_MENU_H
#define APP_MENU_H

class Menu {
public:
    Menu();  // 构造函数
    ~Menu(); // 析构函数

    // 创建菜单
    void createMenu();
    void createSystemTray();
    // id mainMenu;  // 存储主菜单（作为成员变量）
};

#endif // APP_MENU_H
