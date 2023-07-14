#include "LaunchUnit.h"

#define TRIGGER_SERVO_LOADED_ANGLE 0
#define TRIGGER_SERVO_RELEASED_ANGLE 179
#define SAFETY_SERVO_ON_ANGLE 150
#define SAFETY_SERVO_OFF_ANGLE 163


LaunchUnit::LaunchUnit(uint8_t triggerServoPin,
                       uint8_t safetyServoPin,
                       uint8_t rearSwitchPin,
                       uint8_t safetySwitchPin,
                       uint8_t frontSwitchPin,
                       CRGB& statusLed) : _state{State::FIRED},
                                          _rearSwitch{rearSwitchPin},
                                          _safetySwitch{safetySwitchPin},
                                          _frontSwitch{frontSwitchPin},
                                          _statusLed{statusLed},
                                          _isArmed{false} {
    _safetyServo.attach(safetyServoPin);
    _triggerServo.attach(triggerServoPin);
}

void LaunchUnit::init() {
    _rearSwitch.init();
    _safetySwitch.init();
    _frontSwitch.init();
    _safetyServo.write(SAFETY_SERVO_ON_ANGLE);
    _triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE);
    updateLed();
}

void LaunchUnit::update(uint32_t now) {
    Threads::Scope lock(_mutex);
    _rearSwitch.poll(now);
    _safetySwitch.poll(now);
    _frontSwitch.poll(now);
    _statusLed.update(now);
}

void LaunchUnit::fire() {
    Threads::Scope lock(_mutex);
    if (_state == State::LOADED && _isArmed && _rearSwitch.getState() && _safetySwitch.getState() && !_frontSwitch.getState()) {
        threads.addThread(fireThread, this);
    }
}

void LaunchUnit::load() {
    Threads::Scope lock(_mutex);
    if (_state == State::FIRED && !_isArmed && !_rearSwitch.getState() && _frontSwitch.getState()) {
        threads.addThread(LaunchUnit::loadThread, this);
    }
}

void LaunchUnit::unload() {
    Threads::Scope lock(_mutex);
    if (_state == State::LOADED && !_isArmed && _rearSwitch.getState() && _safetySwitch.getState() && !_frontSwitch.getState()) {
        threads.addThread(unloadThread, this);
    }
}

LaunchUnit::State LaunchUnit::getState() const {
    Threads::Scope lock(_mutex);
    return _state;
}

void LaunchUnit::setArmed(bool isArmed) {
    Threads::Scope lock(_mutex);
    _isArmed = isArmed;
}

void LaunchUnit::updateLed() {
    switch (_state) {
        case State::FIRED:
            _statusLed.setColor(CRGB::Green);
            _statusLed.setMode(RGBLed::Mode::ON);
            break;
        case State::LOADING:
            _statusLed.setColor(CRGB::Green);
            _statusLed.setMode(RGBLed::Mode::BLINK);
            break;
        case State::LOADED:
            _statusLed.setColor(CRGB::Red);
            if (_isArmed) {
                _statusLed.setMode(RGBLed::Mode::BLINK);
            } else {
                _statusLed.setMode(RGBLed::Mode::ON);
            }
            break;
        case State::FIRING:
            _statusLed.setColor(CRGB::Red);
            _statusLed.setMode(RGBLed::Mode::BLINK);
            break;
        case State::UNLOADING:
            _statusLed.setColor(CRGB::Blue);
            _statusLed.setMode(RGBLed::Mode::BLINK);
            break;
        case State::ERROR:
            _statusLed.setColor(CRGB::Orange);
            _statusLed.setMode(RGBLed::Mode::BLINK);
            break;
        default:
            Serial.println("ERROR: Invalid state in LaunchUnit::updateLed");
            break;
    }
}

void LaunchUnit::loadThread(void* arg) {
    LaunchUnit* lu = (LaunchUnit*)arg;
    lu->_mutex.lock();
    Serial.println("Loading thread started");
    lu->_state = State::LOADING;
    lu->updateLed();
    lu->_safetyServo.write(SAFETY_SERVO_OFF_ANGLE);
    lu->_triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE);
    Serial.println("Waiting for rear switch");
    while (!(volatile bool)lu->_rearSwitch.getState()) {  // carefull about optimizer. Should use volatile??
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_safetyServo.write(SAFETY_SERVO_ON_ANGLE);
    Serial.println("Waiting for safety switch");
    while (!(volatile bool)lu->_safetySwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE);
    Serial.println("closing trigger arm");
    lu->_mutex.unlock();
    delay(1000);
    lu->_mutex.lock();
    lu->_state = State::LOADED;
    lu->updateLed();
    Serial.println("Loading thread finished");
    lu->_mutex.unlock();
}

void LaunchUnit::fireThread(void* arg) {
    LaunchUnit* lu = (LaunchUnit*)arg;
    lu->_mutex.lock();
    lu->_state = State::FIRING;
    lu->updateLed();
    lu->_safetyServo.write(SAFETY_SERVO_OFF_ANGLE);
    while ((volatile bool)lu->_safetySwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE);
    while (!(volatile bool)lu->_frontSwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_state = State::FIRED;
    lu->updateLed();
    lu->_mutex.unlock();
}

void LaunchUnit::unloadThread(void* arg) {
    LaunchUnit* lu = (LaunchUnit*)arg;
    lu->_mutex.lock();
    lu->_state = State::UNLOADING;
    lu->updateLed();
    lu->_triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE);
    while ((volatile bool)lu->_rearSwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_safetyServo.write(SAFETY_SERVO_OFF_ANGLE);
    while ((volatile bool)lu->_safetySwitch.getState() || !(volatile bool)lu->_frontSwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE);
    lu->_safetyServo.write(SAFETY_SERVO_ON_ANGLE);
    lu->_state = State::FIRED;
    lu->updateLed();
    lu->_mutex.unlock();
}