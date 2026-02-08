#ifndef LCD_POWER_H_
#define LCD_POWER_H_

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"

namespace esphome
{
    namespace lcd_power
    {

        class LcdPowerComponent : public Component, public i2c::I2CDevice
        {
        public:
            void turn_on();
            void turn_off();

            void dump_config() override;
        };

        template <typename... Ts>
        class TurnOnAction : public Action<Ts...>
        {
        public:
            TurnOnAction(LcdPowerComponent *output) : output_(output) {}

            void play(const Ts &...x) override { this->output_->turn_on(); }

        protected:
            LcdPowerComponent *output_;
        };
        template <typename... Ts>
        class TurnOffAction : public Action<Ts...>
        {
        public:
            TurnOffAction(LcdPowerComponent *output) : output_(output) {}

            void play(const Ts &...x) override { this->output_->turn_off(); }

        protected:
            LcdPowerComponent *output_;
        };
    } // namespace lcd_power
} // namespace esphome

#endif // LCD_POWER_H_
