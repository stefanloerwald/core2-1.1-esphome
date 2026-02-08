
#include "axp2101_deep_sleep.h"

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

    void Axp2101DeepSleepComponent::disableTemperatureMeasure()
    {
        clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 4);
    }
    void Axp2101DeepSleepComponent::disableBattDetection()
    {
        clrRegisterBit(XPOWERS_AXP2101_BAT_DET_CTRL, 0);
    }
    void Axp2101DeepSleepComponent::disableVbusVoltageMeasure()
    {
        clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 2);
    }
    void Axp2101DeepSleepComponent::disableBattVoltageMeasure()
    {
        clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 0);
    }
    void Axp2101DeepSleepComponent::disableSystemVoltageMeasure()
    {
        clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 3);
    }
    void Axp2101DeepSleepComponent::enableSleep()
    {
        setRegisterBit(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL, 0);
    }
    void Axp2101DeepSleepComponent::disableDC2()
    {
        clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 1);
    }
    void Axp2101DeepSleepComponent::disableDC3()
    {
        clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 2);
    }
    void Axp2101DeepSleepComponent::disableDC4()
    {
        clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 3);
    }
    void Axp2101DeepSleepComponent::disableDC5()
    {
        clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 4);
    }
    void Axp2101DeepSleepComponent::disableALDO1()
    {
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 0);
    }
    void Axp2101DeepSleepComponent::disableALDO2()
    {
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 1);
    }
    void Axp2101DeepSleepComponent::disableALDO3()
    {
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 2);
    }
    void Axp2101DeepSleepComponent::disableALDO4()
    {
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 3);
    }
    void Axp2101DeepSleepComponent::disableBLDO1()
    {
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 4);
    }
    void Axp2101DeepSleepComponent::disableBLDO2()
    {
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 5);
    }
    void Axp2101DeepSleepComponent::disableCPUSLDO()
    {
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 6);
    }
    void Axp2101DeepSleepComponent::disableDLDO1()
    {
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 7);
    }
    void Axp2101DeepSleepComponent::disableDLDO2()
    {
        clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL1, 0);
    }
    void Axp2101DeepSleepComponent::enableWakeup()
    {
        setRegisterBit(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL, 1);
    }

    void Axp2101DeepSleepComponent::sleep()
    {

            // Turn off ADC data monitoring to save power
            disableTemperatureMeasure();
            // Enable internal ADC detection
            disableBattDetection();
            disableVbusVoltageMeasure();
            disableBattVoltageMeasure();
            disableSystemVoltageMeasure();


            // Enable PMU sleep
            enableSleep();

            // Turn off the power output of other channels
            disableDC2();
            disableDC3();
            disableDC4();
            disableDC5();
            disableALDO1();
            disableALDO2();
            disableALDO3();
            disableALDO4();
            disableBLDO1();
            disableBLDO2();
            disableCPUSLDO();
            disableDLDO1();
            disableDLDO2();

            // Send IRQ wakeup command
            enableWakeup();

            esp_sleep_enable_ext0_wakeup((gpio_num_t )39, LOW); // Core2 V1.1 uses GPIO39 for wakeup on touch.
            esp_deep_sleep_start();
    }

    void Axp2101DeepSleepComponent::dump_config()
    {
        ESP_LOGCONFIG(TAG, "Axp2101DeepSleep");
        LOG_I2C_DEVICE(this);
    }

} // esphome::axp2101_deep_sleep
