import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_CHANNEL, CONF_ID
from . import TLC5940, tlc5940_ns

DEPENDENCIES = ["tlc5940"]

TLC5940Channel = tlc5940_ns.class_(
    "TLC5940Channel", output.FloatOutput, cg.Parented.template(TLC5940)
)

CONF_TLC5940_ID = "tlc5940_id"
CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_TLC5940_ID): cv.use_id(TLC5940),
        cv.Required(CONF_ID): cv.declare_id(TLC5940Channel),
        cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=16),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await output.register_output(var, config)
    await cg.register_parented(var, config[CONF_TLC5940_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))