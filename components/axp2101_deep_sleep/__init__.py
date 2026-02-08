import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]

axp2101_deep_sleep_ns = cg.esphome_ns.namespace("axp2101_deep_sleep")
Axp2101DeepSleepComponent = axp2101_deep_sleep_ns.class_(
    "Axp2101DeepSleepComponent", cg.Component, i2c.I2CDevice
)
Axp2101DeepSleepComponentPtr = Axp2101DeepSleepComponent.operator("ptr")

SleepAction = axp2101_deep_sleep_ns.class_("SleepAction", automation.Action)

SLEEP_ACTION_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(Axp2101DeepSleepComponent),
    }
)

@automation.register_action(
    "axp2101_deep_sleep.sleep", SleepAction, SLEEP_ACTION_SCHEMA
)
async def axp2101_deep_sleep_sleep_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Axp2101DeepSleepComponent),
        }
    )
    .extend(i2c.i2c_device_schema(0x34))
)

def to_code(config):
    cg.add_global(axp2101_deep_sleep_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)
