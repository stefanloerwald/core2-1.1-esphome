
#include "axp2101_deep_sleep.h"
#include "esp_sleep.h"

namespace esphome::axp2101_deep_sleep
{

    namespace
    {
        static const char *TAG = "axp2101_deep_sleep.sensor";

        // Registers
        constexpr uint8_t XPOWERS_AXP2101_ADC_CHANNEL_CTRL = 0x30;
        constexpr uint8_t XPOWERS_AXP2101_BAT_DET_CTRL = 0x68;
        constexpr uint8_t XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL = 0x26;
        constexpr uint8_t XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL = 0x80;
        constexpr uint8_t XPOWERS_AXP2101_LDO_ONOFF_CTRL0 = 0x90;
        constexpr uint8_t XPOWERS_AXP2101_LDO_ONOFF_CTRL1 = 0x91;

        constexpr uint8_t _bit_0 = 1u;
        constexpr uint8_t Bit(int which_bit)
        {
            return _bit_0 << which_bit;
        }

    } // anonymous namespace

    void Axp2101DeepSleepComponent::clrRegisterBit(uint8_t register_id, int bit)
    {
        uint8_t value = 0;
        read_register(register_id, &value, 1);
        value &= ~Bit(bit);
        write_register(register_id, &value, 1);
    }
    void Axp2101DeepSleepComponent::setRegisterBit(uint8_t register_id, int bit)
    {
        uint8_t value = 0;
        read_register(register_id, &value, 1);
        value |= Bit(bit);
        write_register(register_id, &value, 1);
    }

    void Axp2101DeepSleepComponent::disableBattDetection()
    {
        clrRegisterBit(XPOWERS_AXP2101_BAT_DET_CTRL, 0);
    }
    void Axp2101DeepSleepComponent::disableMeasurements()
    {
        clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 0);
        clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 2);
        clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 3);
        clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 4);
    }
    void Axp2101DeepSleepComponent::enableSleep()
    {
        setRegisterBit(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL, 0);
    }
    void Axp2101DeepSleepComponent::disablePower()
    {
        clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 1);
        clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 2);
        clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 3);
        clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 4);
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 0);
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 1);
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 2);
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 3);
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 4);
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 5);
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 6);
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 7);
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL1, 0);
    }
    void Axp2101DeepSleepComponent::enableWakeup()
    {
        setRegisterBit(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL, 1);
    }

    void Axp2101DeepSleepComponent::sleep()
    {

            // Turn off ADC data monitoring to save power
            disableMeasurements();
            // Enable internal ADC detection
            disableBattDetection();


            // Enable PMU sleep
            enableSleep();

            // Turn off the power output of other channels
            disablePower();

            // Send IRQ wakeup command
            enableWakeup();
            constexpr int HIGH = 1;
            // esp_sleep_enable_ext0_wakeup((gpio_num_t )39, HIGH); // Core2 V1.1 uses GPIO39 for wakeup on touch.
            esp_deep_sleep_start();
    }

    void Axp2101DeepSleepComponent::dump_config()
    {
        ESP_LOGCONFIG(TAG, "Axp2101DeepSleep");
        LOG_I2C_DEVICE(this);
    }

} // esphome::axp2101_deep_sleep
