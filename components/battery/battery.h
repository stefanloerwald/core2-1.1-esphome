#ifndef BATTERY_H_
#define BATTERY_H_

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome
{
    namespace battery
    {
        class BatteryComponent : public PollingComponent, public i2c::I2CDevice
        {
        public:
            void dump_config() override;
            void update() override;

        private:

            inline int readRegister(uint8_t reg)
            {
                uint8_t val = 0;
                return read_register(reg, &val, 1) == -1 ? -1 : val;
            }
            inline bool getRegisterBit(uint8_t registers, uint8_t bit)
            {
                int val = readRegister(registers);
                if (val == -1)
                {
                    return false;
                }
                return val & Bit(bit);
            }
            constexpr uint8_t _bit_0 = 1u;
            constexpr uint8_t Bit(int which_bit)
            {
                return _bit_0 << which_bit;
            }
            inline uint16_t readRegisterH5L8(uint8_t highReg, uint8_t lowReg)
            {
                int h5 = readRegister(highReg);
                int l8 = readRegister(lowReg);
                if (h5 == -1 || l8 == -1)
                    return 0;
                return ((h5 & 0x1F) << 8) | l8;
            }

            uint16_t getBattVoltage();
            bool isBatteryConnected();
            bool isCharging();
        };

    } // namespace battery
} // namespace esphome

#endif // BATTERY_H_
