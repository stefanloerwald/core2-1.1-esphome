import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_BRIGHTNESS, CONF_ID

DEPENDENCIES = ["i2c"]

display_backlight_ns = cg.esphome_ns.namespace("display_backlight")
DisplayBacklightComponent = display_backlight_ns.class_(
    "DisplayBacklightComponent", i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(DisplayBacklightComponent),
            cv.Optional(CONF_BRIGHTNESS, default=1.0): cv.percentage,
        }
    )
    .extend(i2c.i2c_device_schema(0x34))
)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

    if CONF_BRIGHTNESS in config:
        conf = config[CONF_BRIGHTNESS]
        cg.add(var.set_brightness(conf))
