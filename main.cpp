#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include <cstdlib> 

#include "resource.h"
#include "config/config.hpp"

using namespace std;

void init()
{
    Config* cfg = Config::getInstance();
    auto all_hk = cfg->getHotkeys();

    // console shit
    char title[100];
    snprintf(title, sizeof(title), "[HK-Manager] - v %s", cfg->getVersion().c_str());
    SetConsoleTitleA(title);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,
        FOREGROUND_GREEN);
     
    for (const Hotkey& hotkey : all_hk) {
        if (cfg->registerHotkey(hotkey)) {
            cout << "Hotkey registered [" << hotkey.name << "]" << " -> " << hotkey.button << endl;
        }
    }

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        if (msg.message == WM_HOTKEY) {
            int hotkeyID = static_cast<int>(msg.wParam);

            for (const Hotkey& hotkey : all_hk) {
                if (hotkey.id == hotkeyID) {
                    hotkey.action();
                }
            }
        }
    }
}

int main()
{
	init();
	return 0;
}