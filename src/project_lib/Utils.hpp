/**
 * Project Utils - Functions shared across all 3 ECUs
 * - Stores CAN msgs IDs
 * - Ensure gear, velocity and RPM values are meeting specs
 */
#pragma once
#include "stdint.h"

const uint32_t IdMsgRPM = 0x0CF00400;
const uint32_t IdMsgGear = 0x18F00503;
const uint32_t IdMsgVelocity = 0x18FEF100;

/**
 * Restrict gear to allowed values
 */
uint8_t eval_gear(const uint8_t gear) {
  const uint8_t maxAllowedGear = 5;
  const uint8_t selectedGear = (gear <= maxAllowedGear) ? gear : maxAllowedGear;

  return selectedGear;
}

/**
 * Restrict RPM to allowed values for each gear
 */
uint16_t eval_rpm(const uint8_t gear, const uint16_t rpm) {
  // Max allowed rpm values per gear
  const uint16_t max_rpm[] = {
      8000, // no gear
      4000, // 1st gear
      4800, // 2nd gear
      5600, // 3rd gear
      6400, // 4th gear
      7200  // 5th gear
  };

  // Valid input values for gear
  const uint8_t selectedGear = eval_gear(gear);

  // Restrict high values to max rpm allowed
  if (rpm > max_rpm[selectedGear]) {
    return max_rpm[selectedGear];
  }

  return rpm;
}

/**
 * Calculate Car velocity considering rpm and velocity values and restrictions
 */
float calc_velocity(const uint8_t gear, const uint16_t rpm) {
  const uint8_t selectedGear = eval_gear(gear);     // Validated gear value
  const uint16_t allowed_rpm = eval_rpm(gear, rpm); // Validated rpm value

  const float tire_radius = 0.326;      // Tire Effective Radius [meters]
  const float diferential_ratio = 3.55; // Differential Transmition Ratio
  const float maxVelocity = 250.00;     // Max Velocity Allowed

  // Gearbox Transmition Ratio for each gear
  const float gearbox_ratio[] = {
      0.00, // no gear
      3.81, // 1st gear
      2.36, // 2nd gear
      1.69, // 3rd gear
      1.31, // 4th gear
      1.00  // 5th gear
  };

  // Set velocity to 0.0 whenever the gear is set to 0
  if ((gear == 0) || (selectedGear == 0) ||
      (gearbox_ratio[selectedGear] == 0)) {
    return 0.0;
  }

  // Calculate velocity
  const float velocity = (tire_radius * allowed_rpm) /
                         (gearbox_ratio[selectedGear] * diferential_ratio);

  if (velocity > maxVelocity) {
    return maxVelocity;
  }

  return velocity;
}

uint8_t encode_gearData(const uint8_t gear) {
  const uint8_t encodedGear = gear + (-127);
  return encodedGear;
}

uint8_t decode_gearData(const uint8_t gear) {
  const uint8_t decodedGear = gear - (-127);
  return decodedGear;
}

uint16_t encode_velocityData(const float velocity) {
  const uint16_t encodedVelocity = (uint16_t)velocity;
  return encodedVelocity;
}

float decode_velocityData(const uint16_t velocity) {
  const float decodedVelocity = (float)velocity;
  return decodedVelocity;
}

/**
 * Encode RPM Data
 *  - resolution = 0.125 bit/RPM
 *  - maxValue = 8032 RPM
 */
uint16_t encode_rpmData(const uint16_t rpm) {
  const uint16_t encodedVelocity = rpm >> 3;
  return encodedVelocity;
}

/**
 * Decode RPM Data
 *  - resolution = 0.125 bit/RPM
 *  - maxValue = 8032 RPM
 */
uint16_t decode_rpmData(const uint16_t rpm) {
  const uint16_t decodedVelocity = rpm << 3;
  return decodedVelocity;
}