# M5Stack Core2 v1.1 integration into esphome

This respository contains components and config snippets for the [M5Stack Core2 v1.1](https://docs.m5stack.com/en/core/Core2%20v1.1) device on [esphome](http://esphome.io/).

## Example config

See [config.yaml](https://github.com/stefanloerwald/core2-1.1-esphome/blob/main/config.yaml) for a full example of the device config.

## Helper component snippets

Yaml files in [the components directory](https://github.com/stefanloerwald/core2-1.1-esphome/tree/main/components) simplify the device configuration, but can also be configured in the main config yaml file. This is for convenience only.

## Components

### Battery

The battery component offers battery state indicators: voltage, battery percentage, charging indicator.

Usage:

```yaml

external_components:
  - source: github://stefanloerwald/core2-1.1-esphome
    components: [ battery ]

i2c:
  - id: i2c_bus
    sda: GPIO21
    scl: GPIO22
    scan: True

i2c_device:
  - id: backlight_power
    address: 0x34
    i2c_id: i2c_bus

sensor:
  - platform: battery
    i2c_id: i2c_bus
    update_interval: 60s
    battery_voltage:
      name: "Battery Voltage"
      id: battery_voltage
      device_class: Voltage
      unit_of_measurement: "V"
      state_class: measurement
    battery_level:
      name: "Battery Level"
      id: battery_level_id
      device_class: battery
      unit_of_measurement: "%"
      state_class: measurement
      entity_category: diagnostic
      on_value:
        - lvgl.label.update:
            id: battery_status_label
            text:
              format: "%0.0f%%"
              args: [id(battery_level_id).state]
    battery_charging:
      name: "Battery Charging"
      entity_category: diagnostic
```

### Vibration motor

The vibration motor can be used the same way the [Vibration Web API](https://developer.mozilla.org/en-US/docs/Web/API/Vibration_API) works: a pattern of alternating on and off durations is specified.
For example, touch feedback can be implemented as 

```yaml
- vibration_motor.vibrate:
        id: vibration_motor_id
        pattern: [40ms]
```

Full usage:

```yaml

external_components:
  - source: github://stefanloerwald/core2-1.1-esphome
    components: [ vibration_motor ]


i2c:
  - id: i2c_bus
    sda: GPIO21
    scl: GPIO22
    scan: True

i2c_device:
  - id: backlight_power
    address: 0x34
    i2c_id: i2c_bus

vibration_motor:
  id: vibration_motor_id
  i2c_id: i2c_bus

# in any action, e.g. on button press
on_press:
  then:
    - vibration_motor.vibrate:
        id: vibration_motor_id
        pattern: [40ms, 200ms, 60ms] # 40ms vibration, 200ms off, 60ms vibration
```

Note: the intensity setting is so far untested.

### Display backlight

The device has a display backlight. Just enabling the display is not enough..
With this component, the backlight can be turned on and off, toggled, and the brightness can be adjusted.

```
external_components:
  - source: github://stefanloerwald/core2-1.1-esphome
    components: [ display_backlight ]

i2c:
  - id: i2c_bus
    sda: GPIO21
    scl: GPIO22
    scan: True

i2c_device:
  - id: backlight_power
    address: 0x34
    i2c_id: i2c_bus

display_backlight:
  id: display_backlight_id
  i2c_id: i2c_bus
  brightness: 25%

# Actions: use these, e.g. in on_boot, on_press, etc.
- display_backlight.turn_on:
    id: display_backlight_id
- display_backlight.turn_off:
    id: display_backlight_id            
- display_backlight.toggle:
    id: display_backlight_id
- display_backlight.set_brightness:
    id: display_backlight_id
    brightness: 25%;
```
