
#include "display_backlight.h"

namespace esphome::display_backlight
{

    namespace
    {
        static const char *TAG = "display_backlight.sensor";

        // Registers
        constexpr uint8_t XPOWERS_AXP2101_LDO_ONOFF_CTRL0 = 0x90; // For display power
        constexpr uint8_t XPOWERS_AXP2101_LDO_VOL4_CTRL = 0x96; // For display brightness

        constexpr uint8_t _bit_0 = 1u;
        constexpr uint8_t Bit(int which_bit)
        {
            return _bit_0 << which_bit;
        }

    } // anonymous namespace

    void DisplayBacklightComponent::turn_on()
    {
        uint8_t value = 0;
        read_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
        value |= Bit(4);
        write_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
    }

    void DisplayBacklightComponent::turn_off()
    {
        uint8_t value = 0;
        read_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
        value &= ~Bit(4);
        write_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
    }

    void DisplayBacklightComponent::toggle()
    {
        uint8_t value = 0;
        read_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
        value ^= Bit(4);
        write_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
    }

    void DisplayBacklightComponent::set_brightness(float brightness)
    {
        if (brightness_ == brightness)
        {
            return;
        }
        brightness_ = brightness;
        if (brightness_ <= 0)
        {
            // TODO turn power off entirely.
        }
        const uint8_t min_vis_step = 20;
        const uint8_t max_step = 30;
        uint8_t step = static_cast<uint8_t>(brightness_ * (max_step - min_vis_step) + 0.5f) + min_vis_step;
        if (step > max_step)
            step = max_step;

        uint8_t reg = (ReadByte(XPOWERS_AXP2101_LDO_VOL4_CTRL) & 0xE0) | (step & 0x1F);
        ESP_LOGD(TAG, "Setting BLDO1 step %u for brightness=%f", step, brightness_);
        write_byte(XPOWERS_AXP2101_LDO_VOL4_CTRL, reg);
    }

    void DisplayBacklightComponent::dump_config()
    {
        ESP_LOGCONFIG(TAG, "DisplayBacklight");
        LOG_I2C_DEVICE(this);
    }

} // esphome::display_backlight
