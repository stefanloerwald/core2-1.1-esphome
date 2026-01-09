
#include "display_backlight.h"

namespace esphome::display_backlight
{

    namespace
    {
        static const char *TAG = "display_backlight.sensor";

        // Register
        constexpr uint8_t XPOWERS_AXP2101_LDO_ONOFF_CTRL0 = 0x90;
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

    void DisplayBacklightComponent::dump_config()
    {
        ESP_LOGCONFIG(TAG, "DisplayBacklight");
        LOG_I2C_DEVICE(this);
    }

} // esphome::display_backlight
