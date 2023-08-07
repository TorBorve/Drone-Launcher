#include "LaunchUnit.h"

#include "Log.h"
// #include "Communicator.h"

namespace DroneLauncher {

#define TRIGGER_SERVO_LOADED_ANGLE 179
#define TRIGGER_SERVO_RELEASED_ANGLE 90
#define TRIGGER_SERVO_LOADED_ANGLE_MIRROR 0
#define TRIGGER_SERVO_RELEASED_ANGLE_MIRROR 90
#define SAFETY_SERVO_ON_ANGLE 170
#define SAFETY_SERVO_OFF_ANGLE 130

LaunchUnit::LaunchUnit(uint8_t triggerServoPin,
                       uint8_t safetyServoPin,
                       uint8_t rearSwitchPin,
                       uint8_t safetySwitchPin,
                       CRGB& statusLed,
                       bool mirrored) : _state{State::FIRED},
                                        _safetyServoPin{safetyServoPin},
                                        _triggerServoPin{triggerServoPin},
                                        _rearSwitch{rearSwitchPin},
                                        _safetySwitch{safetySwitchPin},
                                        _statusLed{statusLed},
                                        _isArmed{false},
                                        _mirrored{mirrored} {}

void LaunchUnit::init() {
    _rearSwitch.init();
    _safetySwitch.init();
    _safetyServo.attach(_safetyServoPin);
    _triggerServo.attach(_triggerServoPin);
    _safetyServo.write(SAFETY_SERVO_ON_ANGLE);
    if (_mirrored) {
        _triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE_MIRROR);
    } else {
        _triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE);
    }
    updateLed();
}

void LaunchUnit::update(uint32_t now) {
    Threads::Scope lock(_mutex);
    _rearSwitch.poll(now);
    _safetySwitch.poll(now);
    _statusLed.update(now);
}

void LaunchUnit::fire() {
    Threads::Scope lock(_mutex);
    if (_state == State::LOADED && _isArmed && _rearSwitch.getState() && _safetySwitch.getState()) {
        threads.addThread(fireThread, this);
    } else {
        LOG_WARN("Fire acceptance test failed")
    }
}

void LaunchUnit::load() {
    Threads::Scope lock(_mutex);
    if (_state == State::FIRED && !_isArmed && !_rearSwitch.getState()) {
        threads.addThread(LaunchUnit::loadThread, this);
    } else {
        LOG_WARN("Load acceptance test failed")
        LOG_WARN("State: %d, armed: %d, rearsw: %d", (uint8_t)_state, _isArmed, _rearSwitch.getState());
    }
}

void LaunchUnit::unload() {
    Threads::Scope lock(_mutex);
    if (_state == State::LOADED && !_isArmed && _rearSwitch.getState() && _safetySwitch.getState()) {
        threads.addThread(unloadThread, this);
    } else {
        LOG_WARN("Unload acceptance test failed")
    }
}

void LaunchUnit::manualControl(bool safetyOn, bool triggerOn) {
    Threads::Scope lock(_mutex);
    _state = State::MANUAL;
    if (safetyOn) {
        _safetyServo.write(SAFETY_SERVO_ON_ANGLE);
    } else {
        _safetyServo.write(SAFETY_SERVO_OFF_ANGLE);
    }
    if (triggerOn) {
        if (_mirrored) {
            _triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE_MIRROR);
        } else {
            _triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE);
        }
    } else {
        if (_mirrored) {
            _triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE_MIRROR);
        } else {
            _triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE);
        }
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
        case State::MANUAL:
            _statusLed.setColor(CRGB::Purple);
            _statusLed.setMode(RGBLed::Mode::BLINK);
            break;
        case State::ERROR:
            _statusLed.setColor(CRGB::Orange);
            _statusLed.setMode(RGBLed::Mode::BLINK);
            break;
        default:
            LOG_ERROR("invalid state in launchunit updateLed");
            break;
    }
}

void LaunchUnit::loadThread(void* arg) {
    LOG_INFO("Load thread started");
    LaunchUnit* lu = (LaunchUnit*)arg;
    lu->_mutex.lock();
    lu->_state = State::LOADING;
    lu->updateLed();
    lu->_safetyServo.write(SAFETY_SERVO_OFF_ANGLE);
    if (lu->_mirrored) {
        lu->_triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE_MIRROR);
    } else {
        lu->_triggerServo.write(TRIGGER_SERVO_LOADED_ANGLE);
    }
    while (!(volatile bool)lu->_rearSwitch.getState()) {  // carefull about optimizer. Need to use volatile
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_safetyServo.write(SAFETY_SERVO_ON_ANGLE);
    while (!(volatile bool)lu->_safetySwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_mutex.unlock();
    delay(1000);
    lu->_mutex.lock();
    lu->_state = State::LOADED;
    lu->updateLed();
    lu->_mutex.unlock();
    LOG_INFO("Load thread completed");
}

void LaunchUnit::fireThread(void* arg) {
    LOG_INFO("Fire thread started");
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
    delay(1000);  // ensure that safety pin is completely off.
    if (lu->_mirrored) {
        lu->_triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE_MIRROR);
    } else {
        lu->_triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE);
    }
    while ((volatile bool)lu->_rearSwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_state = State::FIRED;
    lu->updateLed();
    lu->_mutex.unlock();
    LOG_INFO("Fire thread completed");
}

void LaunchUnit::unloadThread(void* arg) {
    LOG_INFO("Unload thread started");
    LaunchUnit* lu = (LaunchUnit*)arg;
    lu->_mutex.lock();
    lu->_state = State::UNLOADING;
    lu->updateLed();
    if (lu->_mirrored) {
        lu->_triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE_MIRROR);
    } else {
        lu->_triggerServo.write(TRIGGER_SERVO_RELEASED_ANGLE);
    }
    while ((volatile bool)lu->_rearSwitch.getState()) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    delay(500);
    lu->_safetyServo.write(SAFETY_SERVO_OFF_ANGLE);
    while (((volatile bool)lu->_safetySwitch.getState())) {
        lu->_mutex.unlock();
        delay(100);
        lu->_mutex.lock();
    }
    lu->_state = State::FIRED;
    lu->updateLed();
    lu->_mutex.unlock();
    LOG_INFO("Unload thread completed");
}

}  // namespace DroneLauncher