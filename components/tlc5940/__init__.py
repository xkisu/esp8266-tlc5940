import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import (
    CONF_ID,
    CONF_NUM_CHIPS,
)

CONF_SIN_PIN = "sin_pin"
CONF_SCLK_PIN = "sclk_pin"
CONF_XLAT_PIN = "xlat_pin"
CONF_GSCLK_PIN = "gsclk_pin"
CONF_BLANK_PIN = "blank_pin"

CODEOWNERS = ["@keithcodes"]

tlc5940_ns = cg.esphome_ns.namespace("tlc5940")
TLC5940 = tlc5940_ns.class_("TLC5940", cg.Component)

MULTI_CONF = True
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TLC5940),
        cv.Required(CONF_SIN_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_SCLK_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_XLAT_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_GSCLK_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_BLANK_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_NUM_CHIPS, default=1): cv.int_range(min=1, max=85),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    data = await cg.gpio_pin_expression(config[CONF_SIN_PIN])
    cg.add(var.set_sin_pin(data))
    data = await cg.gpio_pin_expression(config[CONF_SCLK_PIN])
    cg.add(var.set_sclk_pin(data))
    data = await cg.gpio_pin_expression(config[CONF_XLAT_PIN])
    cg.add(var.set_xlat_pin(data))
    data = await cg.gpio_pin_expression(config[CONF_GSCLK_PIN])
    cg.add(var.set_gsclk_pin(data))
    data = await cg.gpio_pin_expression(config[CONF_BLANK_PIN])
    cg.add(var.set_blank_pin(data))


    # cg.add(var.set_num_chips(config[CONF_NUM_CHIPS]))