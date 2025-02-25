cmake_minimum_required(VERSION 3.16)
project(webview_example LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(FetchContent)

FetchContent_Declare(
        webview
        GIT_REPOSITORY https://github.com/wenbingzhang/webview
        GIT_TAG master)
FetchContent_MakeAvailable(webview)

if (APPLE)
    set(PLATFORM_SOURCES platform/macos/menu.mm platform/macos/menu.h)
    set(MACOSX_LINK_LIBRARIES "-framework Cocoa")
endif ()

if (WIN32)
    if(MSVC)
        add_compile_options(/utf-8)
    else()
        add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
    endif()
    add_definitions(-DUNICODE -D_UNICODE)  # 强制使用宽字符API
    set(PLATFORM_SOURCES platform/windows/menu.cpp platform/windows/menu.h)
endif ()

add_executable(${PROJECT_NAME} main.cpp ${PLATFORM_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE webview::core ${MACOSX_LINK_LIBRARIES})

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE ON
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.zhangwenbing.webview_example"
            MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
            MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
            MACOSX_BUNDLE_ICON_FILE "icon.icns"  # 如果有图标
    )

    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND rm -rf $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
            COMMAND cp -r ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
    )
endif ()