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

### LCD Power

While the display works just fine with just the [components/display.yaml](https://github.com/stefanloerwald/core2-1.1-esphome/blob/main/components/display.yaml) configuration, by default the LCD stays powered when deep sleep is entered. This is not desirable, as it keeps draining the battery. The `lcd_power` component solves this issue.

Warning: turning the `lcd_power` off while the device otherwise expects the display to show something results in an error state that can only be recovered from with a device reset.

```yaml
external_components:
  - source: github://stefanloerwald/core2-1.1-esphome
    components: [ lcd_power ]

i2c:
  - id: i2c_bus
    sda: GPIO21
    scl: GPIO22
    scan: True

lcd_power:
  id: lcd_power_id
  i2c_id: i2c_bus

esphome:
  on_boot:
    - priority: 600
      then:
        # Turn on display power on_boot
        - lcd_power.turn_on:
            id: lcd_power_id

lvgl:
  touchscreens:
    touchscreen_id: touch_screen
  on_idle:
    - timeout: 2min
      then:
        - lvgl.pause:
            show_snow: true
        - display_backlight.turn_off:
            id: display_backlight_id
        - deep_sleep.enter:
            id: deep_sleep_id
        - lcd_power.turn_off:
            id: lcd_power_id
```
