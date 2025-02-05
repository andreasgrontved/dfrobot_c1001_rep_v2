#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public uart::UARTDevice {
 public:
  DFRobotC1001Component(uart::UARTComponent *uart);

  void setup() override;
  void update() override;
  void loop() override {}

  // Sensor objects
  sensor::Sensor *human_presence_sensor{nullptr};
  sensor::Sensor *human_movement_sensor{nullptr};
  sensor::Sensor *fall_state_sensor{nullptr};
  sensor::Sensor *residency_state_sensor{nullptr};

  void set_human_presence(sensor::Sensor *sensor) { human_presence_sensor = sensor; }
  void set_human_movement(sensor::Sensor *sensor) { human_movement_sensor = sensor; }
  void set_fall_state(sensor::Sensor *sensor) { fall_state_sensor = sensor; }
  void set_residency_state(sensor::Sensor *sensor) { residency_state_sensor = sensor; }

 private:
  // Internal function for communicating with the sensor
  bool get_sensor_data(uint8_t command, uint8_t *response, uint8_t response_size);

  // Extracted functions from DFRobot_HumanDetection
  int read_human_presence();
  int read_human_movement();
  int read_fall_state();
  int read_residency_state();
};

}  // namespace dfrobot_c1001
}  // namespace esphome
