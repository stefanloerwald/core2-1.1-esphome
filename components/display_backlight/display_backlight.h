#ifndef DISPLAY_BACKLIGHT_H_
#define DISPLAY_BACKLIGHT_H_

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome
{
    namespace display_backlight
    {

        class DisplayBacklightComponent : public PollingComponent, public i2c::I2CDevice
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

            
            void setup() override;
            void dump_config() override;
            float get_setup_priority() const override;
            void update() override;
        private:
            float brightness_;
        };

    } // namespace display_backlight
} // namespace esphome

#endif // DISPLAY_BACKLIGHT_H_
