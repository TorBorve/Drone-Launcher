#include "Navigator.h"

#define BNO08X_RESET -1
#define UPDATE_INTERVAL 1000

// IMU imu;
Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;
euler_t ypr;

IMU::IMU() : _prevUpdate(0) {
}

void IMU::init() {
    if (!bno08x.begin_I2C()) {
        // if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300 byte
        // UART buffer! if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
        Serial.println("Failed to find BNO08x chip");
    }
    enableAll();
}

void IMU::enableAll() {
    if (!bno08x.enableReport(SH2_ARVR_STABILIZED_RV, 5000)) {
        Serial.println("Could not enable rotation vector");
    }

    /*
        if (!bno08x.enableReport(SH2_GEOMAGNETIC_ROTATION_VECTOR)) {
        Serial.println("Could not enable geomagnetic rotation vector");
    }
    if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
        Serial.println("Could not enable game rotation vector");
    }
    if (!bno08x.enableReport(sh2_Quaternion)) {
        Serial.println("Could not enable game rotation vector");
    }
    if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
  }
  if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED)) {
    Serial.println("Could not enable gyroscope");
  }
  if (!bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED)) {
    Serial.println("Could not enable magnetic field calibrated");
  }
  if (!bno08x.enableReport(SH2_LINEAR_ACCELERATION)) {
    Serial.println("Could not enable linear acceleration");
  }
  if (!bno08x.enableReport(SH2_GRAVITY)) {
    Serial.println("Could not enable gravity vector");
  }

  if (!bno08x.enableReport(SH2_STABILITY_CLASSIFIER)) {
    Serial.println("Could not enable stability classifier");
  }
  if (!bno08x.enableReport(SH2_RAW_ACCELEROMETER)) {
    Serial.println("Could not enable raw accelerometer");
  }
  if (!bno08x.enableReport(SH2_RAW_GYROSCOPE)) {
    Serial.println("Could not enable raw gyroscope");
  }
  if (!bno08x.enableReport(SH2_RAW_MAGNETOMETER)) {
    Serial.println("Could not enable raw magnetometer");
  }*/
}

void IMU::update(uint32_t now) {
    if (now - _prevUpdate < UPDATE_INTERVAL) {
        return;
    }
    _prevUpdate = now;

    if (bno08x.wasReset()) {
        Serial.print("IMU sensor was reset");
        enableAll();
    }

    if (!bno08x.getSensorEvent(&sensorValue)) {
        return;
    }
}

void IMU::quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
        ypr->yaw *= RAD_TO_DEG;
        ypr->pitch *= RAD_TO_DEG;
        ypr->roll *= RAD_TO_DEG;
    }
}

void IMU::quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}

euler_t IMU::getEuler(bool degrees) {
    quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr, degrees);
    return ypr;
}

quaternion_t IMU::getQuat() { return quaternion_t{sensorValue.un.arvrStabilizedRV.real,
                                                  sensorValue.un.arvrStabilizedRV.i,
                                                  sensorValue.un.arvrStabilizedRV.j,
                                                  sensorValue.un.arvrStabilizedRV.k}; }

bool IMU::status(uint32_t now) {
    // todo fix this
    return true;
}

#include <SoftwareSerial.h>
#define UPDATE_INTERVAL 1000
#define GPSBaud 38400
#define GPS_SERIAL Serial4
#define MAX_GPS_TIMEOUT 10000

GPS gps;

GPS::GPS() : _prevUpdate(0), _lat(0), _long(0), _last_loc_update(0), _alt(0), _last_alt_update(0), tiny_gps(), _date(), _last_date_update(0), _time(), _last_time_update(0), _speed(), _last_speed_update(0) {
}

void GPS::init() {
    GPS_SERIAL.begin(GPSBaud);
}

void GPS::update(uint32_t now) {
    if (now - _prevUpdate < UPDATE_INTERVAL) {
        return;
    }
    _prevUpdate = now;
    if (GPS_SERIAL.available() > 0) {
        if (tiny_gps.encode(GPS_SERIAL.read())) {
            if (tiny_gps.location.isValid()) {
                _lat = tiny_gps.location.lat();
                _long = tiny_gps.location.lng();
                _last_loc_update = now;
            }

            if (tiny_gps.altitude.isValid()) {
                _alt = tiny_gps.altitude.meters();
                _last_alt_update = now;
            }

            if (tiny_gps.date.isValid()) {
                _date = tiny_gps.date;
                _last_date_update = now;
            }

            if (tiny_gps.time.isValid()) {
                _time = tiny_gps.time;
                _last_time_update = now;
            }

            if (tiny_gps.speed.isValid()) {
                _speed = tiny_gps.speed;
                _last_speed_update = now;
            }
        }
    }
}

bool GPS::status(uint32_t now) {
    LastUpdateArray lastUpdates = getUpdateTimes(now);
    uint32_t* ptr = &lastUpdates.time_since_last_loc_update;
    int numMembers = sizeof(LastUpdateArray) / sizeof(uint32_t);

    for (int i = 0; i < numMembers; i++) {
        if (*ptr > MAX_GPS_TIMEOUT) {
            return false;
        }
        ptr++;
    }
    return true;
}

// GuidanceSystem::GuidanceSystem() : _gps(), _imu() {
//     }

void GuidanceSystem::init() {
    _imu.init();
    _gps.init();
}

void GuidanceSystem::update(uint32_t now) {
    _imu.update(now);
    _gps.update(now);
}

bool GuidanceSystem::status(uint32_t now) {
    return _imu.status(now) && _gps.status(now);
}