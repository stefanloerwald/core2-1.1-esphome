#ifndef VIBRATION_MOTOR_H_
#define VIBRATION_MOTOR_H_

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"

#include <vector>

namespace esphome
{
    namespace vibration_motor
    {

        class VibrationMotorComponent : public Component, public i2c::I2CDevice
        {
        public:
            void vibrate(const std::vector<int32_t>& duration_ms);
            void set_level(float level);

            void dump_config() override;

        private:
            float level_;

            inline uint8_t ReadByte(uint8_t address)
            {
                uint8_t result;
                read_byte(address, &result);
                return result;
            }
            void turn_on();
            void turn_off();
        };

        template <typename... Ts>
        class VibrateAction : public Action<Ts...>
        {
        public:
            VibrateAction(VibrationMotorComponent *output) : output_(output) {}

            TEMPLATABLE_VALUE(std::vector<int32_t>, durations_ms)

            void play(const Ts &...x) override { this->output_->vibrate(this->durations_ms_.value(x...)); }

        protected:
            VibrationMotorComponent *output_;
        };

        template <typename... Ts>
        class SetLevelAction : public Action<Ts...>
        {
        public:
            SetLevelAction(VibrationMotorComponent *output) : output_(output) {}

            TEMPLATABLE_VALUE(float, level)

            void play(const Ts &...x) override { this->output_->set_level(this->level_.value(x...)); }

        protected:
            VibrationMotorComponent *output_;
        };

    } // namespace vibration_motor
} // namespace esphome

#endif // VIBRATION_MOTOR_H_
