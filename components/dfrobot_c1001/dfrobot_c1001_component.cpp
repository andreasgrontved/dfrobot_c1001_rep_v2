#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

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

bool DFRobotC1001Component::get_sensor_data(uint8_t command, uint8_t *response, uint8_t response_size) {
  uint8_t request[5] = {0x53, 0x59, 0x80, command, 0x54};  // Example request format

  // Send the command
  this->write_array(request, sizeof(request));
  this->flush();  // Ensure all data is sent
  delay(50);      // Wait for the sensor to process the command

  // Check if enough data is available
  if (this->available() < response_size) {
    ESP_LOGW(TAG, "Not enough data received from sensor");
    return false;
  }

  // Read the response
  this->read_array(response, response_size);
  return true;
}

int DFRobotC1001Component::read_human_presence() {
  uint8_t response[10] = {0};
  if (!get_sensor_data(0x81, response, sizeof(response))) return -1;
  return response[6];  // Extract value from response buffer
}

int DFRobotC1001Component::read_human_movement() {
  uint8_t response[10] = {0};
  if (!get_sensor_data(0x82, response, sizeof(response))) return -1;
  return response[6];
}

int DFRobotC1001Component::read_fall_state() {
  uint8_t response[10] = {0};
  if (!get_sensor_data(0x83, response, sizeof(response))) return -1;
  return response[6];
}

int DFRobotC1001Component::read_residency_state() {
  uint8_t response[10] = {0};
  if (!get_sensor_data(0x85, response, sizeof(response))) return -1;
  return response[6];
}

}  // namespace dfrobot_c1001
}  // namespace esphome