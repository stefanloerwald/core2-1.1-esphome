
#include "lcd_power.h"

namespace esphome::lcd_power
{

    namespace
    {
        static const char *TAG = "lcd_power.sensor";

        // Registers
        constexpr uint8_t XPOWERS_AXP2101_LDO_ONOFF_CTRL0 = 0x90; // For lcd power
        constexpr uint8_t  XPOWERS_AXP2101_LDO_VOL3_CTRL = 0x95; // For lcd voltage

        constexpr uint8_t XPOWERS_AXP2101_ALDO4_VOL_MIN = 500u;
        constexpr uint8_t XPOWERS_AXP2101_ALDO4_VOL_MAX = 3500u;
        constexpr uint8_t XPOWERS_AXP2101_ALDO4_VOL_STEPS = 100;

        constexpr uint8_t _bit_0 = 1u;
        constexpr uint8_t Bit(int which_bit)
        {
            return _bit_0 << which_bit;
        }

    } // anonymous namespace

    void LcdPowerComponent::turn_on()
    {
        uint8_t value = 0;
        read_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
        value |= Bit(4);
        write_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
        // Setting voltage to 3300mV
        constexpr uint8_t targetVoltage_mV = 3300u;
        value = 0;
        read_register(XPOWERS_AXP2101_LDO_VOL3_CTRL, &value, 1);
        value &= 0xE0;
        value |= (targetVoltage_mV - XPOWERS_AXP2101_ALDO4_VOL_MIN) / XPOWERS_AXP2101_ALDO4_VOL_STEPS;
        write_register(XPOWERS_AXP2101_LDO_VOL3_CTRL, &value, 1);
    }

    void LcdPowerComponent::turn_off()
    {
        uint8_t value = 0;
        read_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
        value &= ~Bit(4);
        write_register(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, &value, 1);
    }

    void LcdPowerComponent::dump_config()
    {
        ESP_LOGCONFIG(TAG, "LcdPower");
        LOG_I2C_DEVICE(this);
    }

} // esphome::lcd_power
