#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

// Constructor: Store UART reference
DFRobotC1001Component::DFRobotC1001Component(uart::UARTComponent *uart)
    : uart::UARTDevice(uart) {}

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");
}

void DFRobotC1001Component::update() {
  ESP_LOGI(TAG, "Reading DFRobot C1001 sensor data...");

  int presence = read_human_presence();
  int movement = read_human_movement();
  int fall_state = read_fall_state();
  int residency_state = read_residency_state();

  ESP_LOGI(TAG, "Presence: %d, Movement: %d, Fall: %d, Residency: %d",
           presence, movement, fall_state, residency_state);

  if (human_presence_sensor) human_presence_sensor->publish_state(presence);
  if (human_movement_sensor) human_movement_sensor->publish_state(movement);
  if (fall_state_sensor) fall_state_sensor->publish_state(fall_state);
  if (residency_state_sensor) residency_state_sensor->publish_state(residency_state);
}

// Function to send a command and receive response
bool DFRobotC1001Component::get_sensor_data(uint8_t command, uint8_t *response, uint8_t response_size) {
  uint8_t request[5] = {0x53, 0x59, 0x80, command, 0x54};  // Example request format

  this->write_array(request, 5);  // Send the command
  delay(50);

  if (this->available() < response_size) {
    ESP_LOGW(TAG, "Not enough data received from sensor");
    return false;
  }

  this->read_array(response, response_size);
  return true;
}

// Read Human Presence
int DFRobotC1001Component::read_human_presence() {
  uint8_t response[10] = {0};
  if (!get_sensor_data(0x81, response, 10)) return -1;
  return response[6];  // Extract value from response buffer
}

// Read Human Movement
int DFRobotC1001Component::read_human_movement() {
  uint8_t response[10] = {0};
  if (!get_sensor_data(0x82, response, 10)) return -1;
  return response[6];
}

// Read Fall State
int DFRobotC1001Component::read_fall_state() {
  uint8_t response[10] = {0};
  if (!get_sensor_data(0x83, response, 10)) return -1;
  return response[6];
}

// Read Residency State
int DFRobotC1001Component::read_residency_state() {
  uint8_t response[10] = {0};
  if (!get_sensor_data(0x85, response, 10)) return -1;
  return response[6];
}

}  // namespace dfrobot_c1001
}  // namespace esphome
