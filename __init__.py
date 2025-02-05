import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["uart", "sensor"]

dfrobot_c1001_ns = cg.esphome_ns.namespace("dfrobot_c1001")
DFRobotC1001Component = dfrobot_c1001_ns.class_(
    "DFRobotC1001Component", cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(DFRobotC1001Component),
    cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    cv.Optional("human_presence"): sensor.sensor_schema(),
    cv.Optional("human_movement"): sensor.sensor_schema(),
    cv.Optional("fall_state"): sensor.sensor_schema(),
    cv.Optional("residency_state"): sensor.sensor_schema(),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config["uart_id"])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if "human_presence" in config:
        sens = await sensor.new_sensor(config["human_presence"])
        cg.add(var.set_human_presence(sens))
    if "human_movement" in config:
        sens = await sensor.new_sensor(config["human_movement"])
        cg.add(var.set_human_movement(sens))
    if "fall_state" in config:
        sens = await sensor.new_sensor(config["fall_state"])
        cg.add(var.set_fall_state(sens))
    if "residency_state" in config:
        sens = await sensor.new_sensor(config["residency_state"])
        cg.add(var.set_residency_state(sens))
