import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor, uart

DEPENDENCIES = ["uart"]

uart_readline_ns = cg.esphome_ns.namespace("uart_readline")
UartReadlineComponent = uart_readline_ns.class_("UartReadLine", cg.Component, uart.UARTDevice, text_sensor.TextSensor)

CONFIG_SCHEMA = (
    text_sensor
        .text_sensor_schema(UartReadlineComponent)
        .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = await text_sensor.new_text_sensor(config)
    await cg.register_component(var, config)
    cg.add(var)
    await uart.register_uart_device(var, config)

