import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c, sensor
from esphome.const import (
    CONF_BATTERY_LEVEL,
    CONF_BATTERY_VOLTAGE,
    CONF_ID,
    DEVICE_CLASS_BATTERY_CHARGING,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_VOLTAGE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    UNIT_PERCENT,
)

DEPENDENCIES = ["i2c"]

axp2101_ns = cg.esphome_ns.namespace("battery")
BatteryComponent = axp2101_ns.class_(
    "BatteryComponent", cg.PollingComponent, i2c.I2CDevice
)

CONF_BATTERY_CHARGING = "battery_charging"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BatteryComponent),
            cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=3,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_BATTERY,
            ),
            cv.Optional(CONF_BATTERY_CHARGING): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_BATTERY_CHARGING,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x34))
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

    if CONF_BATTERY_VOLTAGE in config:
        conf = config[CONF_BATTERY_VOLTAGE]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_batteryvoltage_sensor(sens))

    if CONF_BATTERY_LEVEL in config:
        conf = config[CONF_BATTERY_LEVEL]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_batterylevel_sensor(sens))

    if CONF_BATTERY_CHARGING in config:
        conf = config[CONF_BATTERY_CHARGING]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_batterycharging_bsensor(sens))
