import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c
from esphome.const import CONF_BRIGHTNESS, CONF_ID

DEPENDENCIES = ["i2c"]

display_backlight_ns = cg.esphome_ns.namespace("display_backlight")
DisplayBacklightComponent = display_backlight_ns.class_(
    "DisplayBacklightComponent", cg.PollingComponent, i2c.I2CDevice
)

# TurnOffAction = display_backlight_ns.class_("TurnOffAction", automation.Action)
# TurnOnAction = display_backlight_ns.class_("TurnOnAction", automation.Action)
# ToggleAction = display_backlight_ns.class_("ToggleAction", automation.Action)

# TURN_ON_ACTION_SCHEMA = cv.Schema({})
# TURN_OFF_ACTION_SCHEMA = cv.Schema({})
# TOGGLE_ACTION_SCHEMA = cv.Schema({})

# @automation.register_action(
#     "display_backlight.turn_off", TurnOffAction, TURN_OFF_ACTION_SCHEMA
# )
# async def display_backlight_turn_off_to_code(config, action_id, template_arg, args):
#     paren = await cg.get_variable(config[CONF_ID])
#     return cg.new_Pvariable(action_id, template_arg, paren)

# @automation.register_action(
#     "display_backlight.toggle", ToggleAction, TOGGLE_ACTION_SCHEMA
# )
# async def display_backlight_toggle_to_code(config, action_id, template_arg, args):
#     paren = await cg.get_variable(config[CONF_ID])
    # return cg.new_Pvariable(action_id, template_arg, paren)

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
