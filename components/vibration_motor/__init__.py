import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c
from esphome.const import CONF_LEVEL, CONF_ID

DEPENDENCIES = ["i2c"]
CONF_PATTERN = "pattern"

vibration_motor_ns = cg.esphome_ns.namespace("vibration_motor")
VibrationMotorComponent = vibration_motor_ns.class_(
    "VibrationMotorComponent", cg.Component, i2c.I2CDevice
)
VibrationMotorComponentPtr = VibrationMotorComponent.operator("ptr")

VibrateAction = vibration_motor_ns.class_("VibrateAction", automation.Action)
SetLevelAction = vibration_motor_ns.class_("SetLevelAction", automation.Action)

VIBRATE_ACTION_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(VibrationMotorComponent),
            cv.Required(CONF_PATTERN): cv.ensure_list(cv.positive_time_period_milliseconds),
    }
)

@automation.register_action(
    "vibration_motor.vibrate", VibrateAction, VIBRATE_ACTION_SCHEMA
)
async def vibration_motor_vibrate_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_PATTERN], args, cg.std_vector.template(cg.int32))
    cg.add(var.set_pattern(template_))
    return var


@automation.register_action(
    "vibration_motor.set_level",
    SetLevelAction,
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(VibrationMotorComponent),
            cv.Required(CONF_LEVEL): cv.templatable(cv.percentage),
        }
    ),
)
async def vibration_motor_set_level_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_LEVEL], args, float)
    cg.add(var.set_level(template_))
    return var

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(VibrationMotorComponent),
            cv.Optional(CONF_LEVEL, default=1.0): cv.percentage,
        }
    )
    .extend(i2c.i2c_device_schema(0x34))
)

def to_code(config):
    cg.add_global(vibration_motor_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

    if CONF_LEVEL in config:
        conf = config[CONF_LEVEL]
        cg.add(var.set_level(conf))
