#include "menu.h"
#import <Cocoa/Cocoa.h>
#include  <iostream>

#import <functional>

@interface TrayHandler : NSObject
- (instancetype)initWithLambda:(std::function<void()>*)lambda;
- (void)onClicked:(id)sender;
@end

@implementation TrayHandler {
    std::function<void()> callback;  // 用 std::function 存储 lambda
}

- (instancetype)initWithLambda:(std::function<void()>*)lambda {
    self = [super init];
    if (self) {
        callback = *lambda;  // 保存 lambda
    }
    return self;
}

- (void)onClicked:(id)sender {
    if (callback) {
        callback();  // 调用 lambda
    }
}
@end

Menu::Menu() {
}

Menu::~Menu() {
}

void Menu::createMenu() {
    // 创建主菜单
    NSMenu *mainMenu = [NSMenu new];

    // 创建应用程序菜单
    NSMenuItem *appMenuItem = [NSMenuItem new];
    NSMenu *appMenu = [NSMenu new];
    [appMenu addItemWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
    [appMenuItem setSubmenu:appMenu];
    [mainMenu addItem:appMenuItem];

    // 创建编辑菜单
    NSMenuItem *editMenuItem = [[NSMenuItem alloc] initWithTitle:@"Edit" action:nil keyEquivalent:@""];
    NSMenu *editMenu = [[NSMenu alloc] initWithTitle:@"Edit"];
    [editMenu addItemWithTitle:@"Undo" action:@selector(undo:) keyEquivalent:@"z"];
    [editMenu addItemWithTitle:@"Redo" action:@selector(redo:) keyEquivalent:@"y"];
    [editMenu addItem:[NSMenuItem separatorItem]];
    [editMenu addItemWithTitle:@"Cut" action:@selector(cut:) keyEquivalent:@"x"];
    [editMenu addItemWithTitle:@"Copy" action:@selector(copy:) keyEquivalent:@"c"];
    [editMenu addItemWithTitle:@"Paste" action:@selector(paste:) keyEquivalent:@"v"];
    [editMenu addItemWithTitle:@"Select All" action:@selector(selectAll:) keyEquivalent:@"a"];
    [editMenuItem setSubmenu:editMenu];
    [mainMenu addItem:editMenuItem];

    // 设置应用程序的主菜单
    [NSApp setMainMenu:mainMenu];

    // 确保主菜单已更新
    [NSApp updateWindows];
}

void Menu::createSystemTray() {
    // 获取状态栏实例
    NSStatusBar *statusBar = [NSStatusBar systemStatusBar];

    // 创建状态栏项目
    NSStatusItem *statusItem = [statusBar statusItemWithLength:NSVariableStatusItemLength];

    // 设置状态栏图标的图标
    NSString *bundlePath = [[NSBundle mainBundle] resourcePath];
    NSString *iconPath = [bundlePath stringByAppendingPathComponent:@"tray.png"];
    // NSLog(@"图标路径: %@", iconPath);
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:iconPath];
    [image setTemplate:YES];
    [statusItem setImage:image];

     // 创建菜单并绑定 target
    NSMenu *trayMenu = [[NSMenu alloc] initWithTitle:@""];

    // 添加菜单项并设置 target
    NSMenuItem *customItem = [trayMenu addItemWithTitle:@"Custom Action" action:@selector(onClicked:) keyEquivalent:@""];
    // 创建 lambda 表达式并传递给 TrayHandler
    std::function<void()> customActionLambda = [&]() {
        std::cout << "C++: Custom Action triggered via lambda\n";  // lambda 内容
    };

    TrayHandler *handler = [[TrayHandler alloc] initWithLambda:&customActionLambda];
    [customItem setTarget:handler];  // 设置每个 TrayHandler 实例作为 target


    [trayMenu addItem:[NSMenuItem separatorItem]];
    [trayMenu addItemWithTitle:@"Quit"
                       action:@selector(terminate:)
                keyEquivalent:@"q"];

    // 将菜单设置为状态栏项目的子菜单
    [statusItem setMenu:trayMenu];
}