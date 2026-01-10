
#include "vibration_motor.h"

namespace esphome::vibration_motor
{

    namespace
    {
        static const char *TAG = "vibration_motor.sensor";

        // Registers
        constexpr uint8_t XPOWERS_AXP2101_LDO_ONOFF_CTRL0 = 0x90; // For power
        constexpr uint8_t XPOWERS_AXP2101_LDO_VOL4_CTRL = 0x96; // For display level

        constexpr uint8_t _bit_0 = 1u;
        constexpr uint8_t Bit(int which_bit)
        {
            return _bit_0 << which_bit;
        }

    } // anonymous namespace

    void VibrationMotorComponent::vibrate(int32_t[] durations_ms)
    {
        bool is_vibrate_duration = true;
        for (const int32_t duration_ms : duration_ms)
        {
            if (is_vibrate_duration)
            {
                turn_on();
                delay(duration_ms);
                turn_off();
            }
            else
            {
                delay(duration_ms);
            }
            is_vibrate_duration = !is_vibrate_duration;
        }
    }

    void VibrationMotorComponent::turn_off()
    {
        uint8_t value = 0;
        read_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
        value &= ~Bit(7);
        write_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
    }

    void VibrationMotorComponent::turn_on()
    {
        uint8_t value = 0;
        read_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
        value |= Bit(7);
        write_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
    }

    void VibrationMotorComponent::set_level(float level)
    {
        if (level_ == level)
        {
            return;
        }
        level_ = level;
        ESP_LOGD(TAG, "NOT IMPLEMENTED");
        // const uint8_t min_vis_step = 20;
        // const uint8_t max_step = 30;
        // uint8_t step = static_cast<uint8_t>(level_ * (max_step - min_vis_step) + 0.5f) + min_vis_step;
        // if (step > max_step)
        //     step = max_step;

        // uint8_t reg = (ReadByte(XPOWERS_AXP2101_LDO_VOL4_CTRL) & 0xE0) | (step & 0x1F);
        // ESP_LOGD(TAG, "Setting BLDO1 step %u for level=%f", step, level_);
        // write_byte(XPOWERS_AXP2101_LDO_VOL4_CTRL, reg);
    }

    void VibrationMotorComponent::dump_config()
    {
        ESP_LOGCONFIG(TAG, "VibrationMotor");
        LOG_I2C_DEVICE(this);
    }

} // esphome::vibration_motor
