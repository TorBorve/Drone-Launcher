#pragma once

#include "Adafruit_LiquidCrystal.h"
#include "MD_Menu.h"
#include <array>
#include <string>

#include "Buttons.h"
#include "LaunchSystem.h"

class Menu {
   public:
    Menu(uint8_t backBtnPin, uint8_t enterBtnPin, uint8_t upBtnPin, uint8_t downBtnPin);
   private:
    // enum class BtnType : uint8_t {
    //     BACK,
    //     ENTER,
    //     UP,
    //     DOWN,
    // };
    // enum class MenuScreen : uint8_t {
    //     SLEEP,
    //     MAIN,
    //     LOAD,
    //     UNLOAD,
    //     STATUS,
    //     SETTINGS,
    // };
    void updateLcdScreen();
    // void updateMenuScreen(BtnType event);

    Adafruit_LiquidCrystal lcd{0};
    Button _backBtn;
    Button _enterBtn;
    Button _upBtn;
    Button _downBtn;
    // MenuScreen _currentMenuScreen;
    // const std::array<std::array<std::string, 2>, 5> _menuScreens = {{
    //     {"Drone Launcher", ""},
    //     {"Load", "Unload"},
    //     {"Fire", "Cancel"},
    //     {"Status", "Back"},
    //     {"Settings", "Back"},
    // }};

    const MD_Menu::mnuHeader_t mnuHdr[] = {
        {10, "Drone Launcher", 10, 12, 0},
        {11, "Load", x, x, 0},
        {12, "Unload", x, x, 0}
    };

    const MD_Menu::mnuItem_t mnuItm[] = {
        {10, "Input Test", MD_Menu::MNU_MENU, 10},
        {20, "Drone Id", MD_Menu::MNU_INPUT, x},
        {21, "Launch Unit", MD_Menu::MNU_INPUT, x},
        {22, "Start Load", MD_Menu::MNU_INPUT}
    };

    const MD_Menu::mnuInput_t mnuInp[] = 
        // Load submenu
        {10, "UInt8", MD_Menu::INP_INT, cbfunc, 4, 0, 10, 10, nullptr}, // Select Drone Id
        {11, "UInt8", MD_Menu::INP_INT, cbfunc, 4, 0, 10, 10, nullptr}, // Select Launch Unit
        {12, "Confirm", MD_Menu::INP_RUN, cbfunc, 4, 0, 10, 10, nullptr}, // Confirm Load

    };

};