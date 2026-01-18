
#include "battery.h"

namespace esphome::battery
{
    namespace
    {
        constexpr uint8_t XPOWERS_AXP2101_STATUS1 = 0x00;
        constexpr uint8_t XPOWERS_AXP2101_ADC_DATA_RELUST0 = 0x34;
        constexpr uint8_t XPOWERS_AXP2101_ADC_DATA_RELUST1 = 0x35;
        constexpr uint8_t XPOWERS_AXP2101_BAT_PERCENT_DATA = 0xA4;
        constexpr uint8_t XPOWERS_AXP2101_STATUS2 = 0x01;
    } // namespace

    void BatteryComponent::dump_config()
    {
        ESP_LOGCONFIG(TAG, "AXP2101:");
        LOG_I2C_DEVICE(this);
        LOG_SENSOR("  ", "Battery Voltage", this->batteryvoltage_sensor_);
        LOG_SENSOR("  ", "Battery Level", this->batterylevel_sensor_);
        LOG_BINARY_SENSOR("  ", "Battery Charging", this->batterycharging_bsensor_);
    }

    bool BatteryComponent::isBatteryConnected()
    {
        return getRegisterBit(XPOWERS_AXP2101_STATUS1, 3);
    }

    uint16_t BatteryComponent::getBattVoltage()
    {
        if (!isBatteryConnected())
        {
            return 0;
        }
        return readRegisterH5L8(XPOWERS_AXP2101_ADC_DATA_RELUST0, XPOWERS_AXP2101_ADC_DATA_RELUST1);
    }
    int BatteryComponent::getBatteryPercent()
    {
        if (!isBatteryConnected())
        {
            return -1;
        }
        return readRegister(XPOWERS_AXP2101_BAT_PERCENT_DATA);
    }
    bool BatteryComponent::isCharging()
    {
        return (readRegister(XPOWERS_AXP2101_STATUS2) >> 5) == 0x01;
    }

    void BatteryComponent::update()
    {

        if (this->batterylevel_sensor_ != nullptr)
        {
            float vbat = getBattVoltage();
            ESP_LOGD(TAG, "Got Battery Voltage=%f", vbat);
            this->batteryvoltage_sensor_->publish_state(vbat / 1000.);

            // The battery percentage may be inaccurate at first use, the PMU will automatically
            // learn the battery curve and will automatically calibrate the battery percentage
            // after a charge and discharge cycle
            float batterylevel;
            if (isBatteryConnected())
            {
                batterylevel = getBatteryPercent();
            }
            else
            {
                batterylevel = 100.0 * ((vbat - 3.0) / (4.1 - 3.0));
            }

            ESP_LOGD(TAG, "Got Battery Level=%f", batterylevel);
            if (batterylevel > 100.)
            {
                batterylevel = 100;
            }
            this->batterylevel_sensor_->publish_state(batterylevel);
        }

        if (this->batterycharging_bsensor_ != nullptr)
        {
            bool vcharging = isCharging();

            ESP_LOGD(TAG, "Got Battery Charging=%s", vcharging ? "true" : "false");
            this->batterycharging_bsensor_->publish_state(vcharging);
        }
    }

} // namespace esphome::battery
