import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID, CONF_UART_ID

# Declare the ESPHome namespace for dfrobot_c1001
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["uart", "sensor"]

dfrobot_c1001_ns = cg.esphome_ns.namespace("dfrobot_c1001")
DFRobotC1001Component = dfrobot_c1001_ns.class_(
    "DFRobotC1001Component", cg.Component, uart.UARTDevice
)

# Configuration schema for dfrobot_c1001
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DFRobotC1001Component),
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Optional("human_presence"): sensor.sensor_schema(),
    cv.Optional("human_movement"): sensor.sensor_schema(),
    cv.Optional("fall_state"): sensor.sensor_schema(),
    cv.Optional("residency_state"): sensor.sensor_schema(),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    # Ensure the UART interface is fully registered before using it
    uart_var = await cg.get_variable(config[CONF_UART_ID])
    if uart_var is None:
        raise cv.Invalid("UART interface not found. Ensure uart_id is declared before dfrobot_c1001.")

    # Create the component
    var = cg.new_Pvariable(config[CONF_ID], uart_var)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    # Register sensors if provided
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