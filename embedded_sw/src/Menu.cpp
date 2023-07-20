#include "Menu.h"

#define MN_UPDATE_INTERVAL 30
#define PIN_MENU_BACK 28
#define PIN_MENU_UP 29
#define PIN_MENU_DOWN 30
#define PIN_MENU_ENTER 31
#define LCD_COLS 16
#define LCD_ROWS 2

Menu menu{PIN_MENU_BACK, PIN_MENU_ENTER, PIN_MENU_UP, PIN_MENU_DOWN};

// Adafruit_LiquidCrystal lcd{0};

const MD_Menu::mnuHeader_t Menu::mnuHdr[] = {
    {10, "Main Menu", 11, 12, 0},
    {11, "Load Drone", 20, 22, 0},
    {12, "Unload Drone", 30, 31, 0},
    // {11, "Input Data", 20, 21, 0},
};

const MD_Menu::mnuItem_t Menu::mnuItm[] = {
    {11, "Load Drone", MD_Menu::MNU_MENU, 11},
    {12, "Unload Drone", MD_Menu::MNU_MENU, 12},
    {20, "Drone ID", MD_Menu::MNU_INPUT, 20},
    {21, "Launch Unit ID", MD_Menu::MNU_INPUT, 21},
    {22, "Confirm", MD_Menu::MNU_INPUT, 22},
    {30, "Launch Unit ID", MD_Menu::MNU_INPUT, 30},
    {31, "Confirm", MD_Menu::MNU_INPUT, 31},

};

const MD_Menu::mnuInput_t Menu::mnuInp[] = {
    {20, "DroneID", MD_Menu::INP_INT, Menu::cbLoadDroneId, 4, 1, 0, 10, 0, 10, nullptr},
    {21, "LaunchID", MD_Menu::INP_INT, Menu::cbLoadLaunchUnitId, 4, 1, 0, 6, 0, 10, nullptr},
    {22, "Confirm", MD_Menu::INP_RUN, Menu::cbLoadConfirm, 0, 0, 0, 0, 0, 0, nullptr},
    {30, "LaunchID", MD_Menu::INP_INT, Menu::cbUnloadLaunchUnitId, 4, 1, 0, 6, 0, 10, nullptr},
    {31, "Confirm", MD_Menu::INP_RUN, Menu::cbUnloadConfirm, 0, 0, 0, 0, 0, 0, nullptr},

};

Menu::Menu(uint8_t backBtnPin,
           uint8_t enterBtnPin,
           uint8_t upBtnPin,
           uint8_t downBtnPin) : _menu{Menu::navigation, Menu::display,
                                       mnuHdr, ARRAY_SIZE(mnuHdr),
                                       mnuItm, ARRAY_SIZE(mnuItm),
                                       mnuInp, ARRAY_SIZE(mnuInp)},
                                 _loadDroneId{0},
                                 _loadLaunchUnitId{0},
                                 _unloadLaunchUnitId{0},
                                 _lcd{0},
                                 _backBtn{backBtnPin},
                                 _enterBtn{enterBtnPin},
                                 _upBtn{upBtnPin},
                                 _downBtn{downBtnPin},
                                 _prevUpdate{0} {}

void Menu::init() {
    _backBtn.init();
    _upBtn.init();
    _downBtn.init();
    _enterBtn.init();
    if (!_lcd.begin(LCD_COLS, LCD_ROWS)) {
        Serial.println("LCD init failed");
        while (1)
            ;
    }
    display(MD_Menu::DISP_INIT, nullptr);
    _menu.begin();
    _menu.setMenuWrap(true);
    _menu.setTimeout(10000);
    _menu.setAutoStart(true);
    _menu.runMenu(true);
}

void Menu::update(uint32_t now) {
    _backBtn.poll(now);
    _upBtn.poll(now);
    _downBtn.poll(now);
    _enterBtn.poll(now);
    if (now - _prevUpdate > MN_UPDATE_INTERVAL) {
        _menu.runMenu();
        _prevUpdate = now;
    }
}

MD_Menu::userNavAction_t Menu::navigation(uint16_t &incDelta) {
    incDelta = 1;
    Menu *globalMenuPtr = &menu;
    if (globalMenuPtr->_backBtn.hasEvent()) {
        globalMenuPtr->_backBtn.clearEvent();
        return (MD_Menu::NAV_ESC);
    }
    if (globalMenuPtr->_downBtn.hasEvent()) {
        globalMenuPtr->_downBtn.clearEvent();
        return (MD_Menu::NAV_DEC);
    }
    if (globalMenuPtr->_upBtn.hasEvent()) {
        globalMenuPtr->_upBtn.clearEvent();
        return (MD_Menu::NAV_INC);
    }
    if (globalMenuPtr->_enterBtn.hasEvent()) {
        globalMenuPtr->_enterBtn.clearEvent();
        return (MD_Menu::NAV_SEL);
    }
    return (MD_Menu::NAV_NULL);
}

bool Menu::display(MD_Menu::userDisplayAction_t action, char *msg) {
    static char szLine[LCD_COLS + 1] = {'\0'};

    Menu *globalMenuPtr = &menu;
    switch (action) {
        case MD_Menu::DISP_INIT:
            globalMenuPtr->_lcd.begin(LCD_COLS, LCD_ROWS);
            globalMenuPtr->_lcd.clear();
            globalMenuPtr->_lcd.noCursor();
            memset(szLine, ' ', LCD_COLS);
            break;

        case MD_Menu::DISP_CLEAR:
            globalMenuPtr->_lcd.clear();
            globalMenuPtr->_lcd.setBacklight(LOW);
            break;

        case MD_Menu::DISP_L0:
            globalMenuPtr->_lcd.setBacklight(HIGH);
            globalMenuPtr->_lcd.setCursor(0, 0);
            globalMenuPtr->_lcd.print(szLine);
            globalMenuPtr->_lcd.setCursor(0, 0);
            globalMenuPtr->_lcd.print(msg);
            break;

        case MD_Menu::DISP_L1:
            globalMenuPtr->_lcd.setBacklight(HIGH);
            globalMenuPtr->_lcd.setCursor(0, 1);
            globalMenuPtr->_lcd.print(szLine);
            globalMenuPtr->_lcd.setCursor(0, 1);
            globalMenuPtr->_lcd.print(msg);
            break;
    }
    return (true);
}

MD_Menu::value_t *Menu::cbLoadDroneId(MD_Menu::mnuId_t id, bool bGet) {
    Menu *globalMenuPtr = &menu;
    if (bGet) {
        globalMenuPtr->_vbuf.value = globalMenuPtr->_loadDroneId;
    } else {
        globalMenuPtr->_loadDroneId = globalMenuPtr->_vbuf.value;
    }
    return (&globalMenuPtr->_vbuf);
}
MD_Menu::value_t *Menu::cbLoadLaunchUnitId(MD_Menu::mnuId_t id, bool bGet) {
    Menu *globalMenuPtr = &menu;
    if (bGet) {
        globalMenuPtr->_vbuf.value = globalMenuPtr->_loadLaunchUnitId;
    } else {
        globalMenuPtr->_loadLaunchUnitId = globalMenuPtr->_vbuf.value;
    }
    return (&globalMenuPtr->_vbuf);
}

MD_Menu::value_t *Menu::cbLoadConfirm(MD_Menu::mnuId_t id, bool bGet) {
    Menu *globalMenuPtr = &menu;
    Serial.println("Confirm Load");
    Serial.print("Drone ID: ");
    Serial.println(globalMenuPtr->_loadDroneId);
    Serial.print("Launch Unit ID: ");
    Serial.println(globalMenuPtr->_loadLaunchUnitId);
    return (nullptr);
}

MD_Menu::value_t *Menu::cbUnloadLaunchUnitId(MD_Menu::mnuId_t id, bool bGet) {
    Menu *globalMenuPtr = &menu;
    if (bGet) {
        globalMenuPtr->_vbuf.value = globalMenuPtr->_unloadLaunchUnitId;
    } else {
        globalMenuPtr->_unloadLaunchUnitId = globalMenuPtr->_vbuf.value;
    }
    return (&globalMenuPtr->_vbuf);
}

MD_Menu::value_t *Menu::cbUnloadConfirm(MD_Menu::mnuId_t id, bool bGet) {
    Menu *globalMenuPtr = &menu;
    Serial.println("Confirm Unload");
    Serial.print("Launch Unit ID: ");
    Serial.println(globalMenuPtr->_unloadLaunchUnitId);
    return (nullptr);
}