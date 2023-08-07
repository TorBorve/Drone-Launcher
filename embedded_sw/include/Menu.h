#pragma once

#include "Adafruit_LiquidCrystal.h"
#include "Buttons.h"
#include "MD_Menu.h"

namespace DroneLauncher {

// Class for handling the LCD menu and buttons attached to it
class Menu {
   public:
    Menu(uint8_t backBtnPin, uint8_t enterBtnPin, uint8_t upBtnPin, uint8_t downBtnPin);
    void init();
    void update(uint32_t now);

   private:
    static MD_Menu::userNavAction_t navigation(uint16_t &incDelta);
    static bool display(MD_Menu::userDisplayAction_t action, char *msg);
    static MD_Menu::value_t *cbLoadDroneId(MD_Menu::mnuId_t id, bool bGet);
    static MD_Menu::value_t *cbLoadLaunchUnitId(MD_Menu::mnuId_t id, bool bGet);
    static MD_Menu::value_t *cbLoadConfirm(MD_Menu::mnuId_t id, bool bGet);
    static MD_Menu::value_t *cbUnloadLaunchUnitId(MD_Menu::mnuId_t id, bool bGet);
    static MD_Menu::value_t *cbUnloadConfirm(MD_Menu::mnuId_t id, bool bGet);

    static const MD_Menu::mnuHeader_t mnuHdr[];
    static const MD_Menu::mnuItem_t mnuItm[];
    static const MD_Menu::mnuInput_t mnuInp[];

    MD_Menu _menu;
    uint8_t _loadDroneId;
    uint8_t _loadLaunchUnitId;
    uint8_t _unloadLaunchUnitId;
    MD_Menu::value_t _vbuf;
    Adafruit_LiquidCrystal _lcd;
    Button _backBtn;
    Button _enterBtn;
    Button _upBtn;
    Button _downBtn;
    uint32_t _prevUpdate;
};

extern Menu menu;

}  // namespace DroneLauncher