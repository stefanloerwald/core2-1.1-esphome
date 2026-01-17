import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]

lcd_power_ns = cg.esphome_ns.namespace("lcd_power")
LcdPowerComponent = lcd_power_ns.class_(
    "LcdPowerComponent", cg.Component, i2c.I2CDevice
)
LcdPowerComponentPtr = LcdPowerComponent.operator("ptr")

TurnOffAction = lcd_power_ns.class_("TurnOffAction", automation.Action)
TurnOnAction = lcd_power_ns.class_("TurnOnAction", automation.Action)

TURN_ON_OFF_ACTION_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(LcdPowerComponent),
    }
)

@automation.register_action(
    "lcd_power.turn_on", TurnOnAction, TURN_ON_OFF_ACTION_SCHEMA
)
async def lcd_power_turn_on_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)
@automation.register_action(
    "lcd_power.turn_off", TurnOffAction, TURN_ON_OFF_ACTION_SCHEMA
)
async def lcd_power_turn_off_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(LcdPowerComponent),
        }
    )
    .extend(i2c.i2c_device_schema(0x34))
)

def to_code(config):
    cg.add_global(lcd_power_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)
