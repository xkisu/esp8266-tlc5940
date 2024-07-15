#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/output/float_output.h"
#include "esphome/core/hal.h"


#define SIN_PIN D7
#define SCLK_PIN D5
#define XLAT_PIN D6
#define BLANK_PIN D8
#define GSCLK_PIN D3

#define TLC5940_N 1

namespace esphome {
    namespace tlc5940 {
        class TLC5940;

        class TLC5940Channel : public output::FloatOutput, public Parented<TLC5940> {
        public:
          void set_channel(uint8_t channel) { channel_ = channel; }

         protected:
          friend class TLC5940;

          void write_state(float state) override;

          uint8_t channel_;
        };

        class TLC5940: public Component {
            uint8_t gsData[192 * TLC5940_N] = {
              // MSB                         LSB
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // channel 15
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // channel 14
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // channel 13
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  // channel 12
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,  // channel 11
              0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,  // channel 10
              0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,  // channel 9
              0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,  // channel 8
              0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,  // channel 7
              0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,  // channel 6
              0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,  // channel 5
              0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,  // channel 4
              0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // channel 3
              0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // channel 2
              1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // channel 1
              1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // channel 0
            };
        protected:
            friend TLC5940Channel;

            GPIOPin *sin_pin_;
            GPIOPin *sclk_pin_;
            GPIOPin *xlat_pin_;
            GPIOPin *gsclk_pin_;
            GPIOPin *blank_pin_;

            void set_channel_value_(uint8_t channel, uint16_t value) {
                ESP_LOGCONFIG("tlc5940", "Setting channel value channel=%d value=%d", channel, value);

                // convert number to binary components
                uint8_t arr[12]; // 12 bits
                int i = 0;
                while(value != 0) {
                  arr[i++] = value%2;
                  value /= 2;
                }

                // zero-padding at start.
                int channel_index = (192-(12*(channel+1)))-1;
                for (int p = 12-i; p >= 0; p--) {
                    gsData[channel_index++] = 0;
//                    ESP_LOGCONFIG("tlc5940", "idx=%d val=0", channel_index);
                }

                for(int j=i-1;j>=0;j--){
                  gsData[channel_index++] = arr[j];
//                  ESP_LOGCONFIG("tlc5940", "idx=%d val=%d", channel_index, arr[j]);
                }
            }
        public:
            void set_sin_pin(GPIOPin *sin_pin) { sin_pin_ = sin_pin; }
            void set_sclk_pin(GPIOPin *sclk_pin) { sclk_pin_ = sclk_pin; }
            void set_xlat_pin(GPIOPin *xlat_pin) { xlat_pin_ = xlat_pin; }
            void set_gsclk_pin(GPIOPin *gsclk_pin) { gsclk_pin_ = gsclk_pin; }
            void set_blank_pin(GPIOPin *blank_pin) { blank_pin_ = blank_pin; }

            void setup() override;
            void dump_config() override;
            float get_setup_priority() const override { return setup_priority::HARDWARE; }
            void loop() override;
        };
    }

}