#ifndef AXP2101_DEEP_SLEEP_H_
#define AXP2101_DEEP_SLEEP_H_

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"

namespace esphome
{
    namespace axp2101_deep_sleep
    {

        class Axp2101DeepSleepComponent : public Component, public i2c::I2CDevice
        {
        public:
            void wake();
            void sleep();

            void dump_config() override;

        private:
            void disableBattDetection();
            void disableMeasurements();
            void enableBattDetection();
            void enableMeasurements();
            void enableSleep();
            void disablePower();
            void enablePower();
            void enableWakeup();
            
            void clrRegisterBit(uint8_t register_id, int bit);
            void setRegisterBit(uint8_t register_id, int bit);

        };

        template <typename... Ts>
        class SleepAction : public Action<Ts...>
        {
        public:
            SleepAction(Axp2101DeepSleepComponent *output) : output_(output) {}

            void play(const Ts &...x) override { this->output_->sleep(); }

        protected:
            Axp2101DeepSleepComponent *output_;
        };
    } // namespace axp2101_deep_sleep
} // namespace esphome

#endif // AXP2101_DEEP_SLEEP_H_
