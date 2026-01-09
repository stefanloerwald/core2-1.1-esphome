#ifndef DISPLAY_BACKLIGHT_H_
#define DISPLAY_BACKLIGHT_H_

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"

namespace esphome
{
    namespace display_backlight
    {

        class DisplayBacklightComponent : public Component, public i2c::I2CDevice
        {
        public:
            void toggle();
            void turn_on();
            void turn_off();
            inline void turn(bool on)
            {
                if (on)
                {
                    turn_on();
                }
                else
                {
                    turn_off();
                }
            }
            void set_brightness(float brightness) { brightness_ = brightness; }

            void dump_config() override;

        private:
            float brightness_;
        };

        template <typename... Ts>
        class ToggleAction : public Action<Ts...>
        {
        public:
            ToggleAction(DisplayBacklightComponent *output) : output_(output) {}

            void play(const Ts &...x) override { this->output_->toggle(); }

        protected:
            DisplayBacklightComponent *output_;
        };
        template <typename... Ts>
        class TurnOnAction : public Action<Ts...>
        {
        public:
            TurnOnAction(DisplayBacklightComponent *output) : output_(output) {}

            void play(const Ts &...x) override { this->output_->turn_on(); }

        protected:
            DisplayBacklightComponent *output_;
        };
        template <typename... Ts>
        class TurnOffAction : public Action<Ts...>
        {
        public:
            TurnOffAction(DisplayBacklightComponent *output) : output_(output) {}

            void play(const Ts &...x) override { this->output_->turn_off(); }

        protected:
            DisplayBacklightComponent *output_;
        };

    } // namespace display_backlight
} // namespace esphome

#endif // DISPLAY_BACKLIGHT_H_
