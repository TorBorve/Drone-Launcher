#include "LaunchUnit.h"

#define TRIGGER_SERVO_LOADED_ANGLE 0
#define TRIGGER_SERVO_RELEASED_ANGLE 180
#define SAFETY_SERVO_ON_ANGLE 0
#define SAFETY_SERVO_OFF_ANGLE 180

#define LU_UPDATE_INTERVAL 10

LaunchUnit::LaunchUnit(uint8_t triggerServoPin,
                       uint8_t safetyServoPin,
                       uint8_t rearSwitchPin,
                       uint8_t safetySwitchPin,
                       uint8_t frontSwitchPin,
                       CRGB& statusLed) : _state{State::FIRED}, _statusLed{statusLed}, _rearSwitch{rearSwitchPin}, _safetySwitch{safetySwitchPin}, _frontSwitch{frontSwitchPin}, _prevUpdate{0}, _isArmed{false} {
    _safetyServo.attach(safetyServoPin);
    _safetyServo.write(SAFETY_SERVO_ON_ANGLE);
    _triggerServo.attach(triggerServoPin);
    _triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE);
}

void LaunchUnit::update(uint32_t now) {
    Threads::Scope lock(_mutex);
    _rearSwitch.poll(now);
    _safetySwitch.poll(now);
    _frontSwitch.poll(now);

    if (now - _prevUpdate < LU_UPDATE_INTERVAL) {
        return;
    }
    _prevUpdate = now;

    switch (_state) {
        case State::FIRED:
            break;
        case State::LOADING:
            // if (_rearSwitch.hasEvent() && _rearSwitch.getState()) {
            //     _rearSwitch.clearEvent();
            //     _safetyServo.write(SAFETY_SERVO_ON_ANGLE);
            //     _triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE);
            // } else if (_safetySwitch.hasEvent() && _safetySwitch.getState() && _rearSwitch.getState()) {
            //     _safetySwitch.clearEvent();
            //     _state = State::LOADED;
            // }
            break;
        case State::LOADED:
            break;
        case State::FIRING:
            // if (_frontSwitch.hasEvent() && _frontSwitch.getState()) {
            //     _frontSwitch.clearEvent();
            //     _state = State::FIRED;
            // }
            break;
        case State::ERROR:
            break;
        default:
            Serial.println("ERROR: Invalid state in LaunchUnit::update");
            break;
    }
}

void LaunchUnit::setArmed(bool isArmed) {
    Threads::Scope lock(_mutex);
    _isArmed = isArmed;
}

bool LaunchUnit::isLoaded() const {
    return true;
}

void LaunchUnit::launch() {
    // _triggerServo.write(LU_SERVO_TRIGGERED);
}

void LaunchUnit::updateLed() {
    switch (_state) {
        case State::FIRED:
            _statusLed = CRGB::Red;
            break;
        case State::LOADING:
            _statusLed = CRGB::Yellow;
            break;
        case State::LOADED:
            _statusLed = CRGB::Green;
            break;
        case State::FIRING:
            _statusLed = CRGB::Blue;
            break;
        case State::ERROR:
            _statusLed = CRGB::Red;
            break;
        default:
            Serial.println("ERROR: Invalid state in LaunchUnit::updateLed");
            break;
    }
}

void LaunchUnit::loadThread(void* arg) {
    LaunchUnit* lu = (LaunchUnit*)arg;
    lu->_mutex.lock();
    lu->_state = State::LOADING;
    lu->_safetyServo.write(SAFETY_SERVO_OFF_ANGLE);
    lu->_triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE);
    while (!(volatile bool)lu->_rearSwitch.getState()) { // carefull about optimizer. Should use volatile??
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_safetyServo.write(SAFETY_SERVO_ON_ANGLE);
    while(!(volatile bool)lu->_safetySwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE);
    lu->_mutex.unlock();
    delay(1000);
    lu->_mutex.lock();
    lu->_state = State::LOADED;
    lu->_mutex.unlock();
}

void LaunchUnit::fireThread(void* arg) {
    LaunchUnit* lu = (LaunchUnit*)arg;
    lu->_mutex.lock();
    lu->_state = State::FIRING;
    lu->_safetyServo.write(SAFETY_SERVO_OFF_ANGLE);
    while((volatile bool)lu->_safetySwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE);
    while(!(volatile bool)lu->_frontSwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_state = State::FIRED;
    lu->_mutex.unlock();
}