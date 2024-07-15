#include "esphome/core/log.h"

#include "tlc5940.h"

static const char *const TAG = "tlc5940";

namespace esphome {
    namespace tlc5940 {
        void TLC5940Channel::write_state(float state) {
            auto amount = static_cast<uint16_t>(state * 4095);
          this->parent_->set_channel_value_(this->channel_, amount);
        }

        void TLC5940::setup() {
          ESP_LOGCONFIG(TAG, "Setting up TLC5940...");

            /*pinMode(GSCLK_PIN, OUTPUT);
              pinMode(SCLK_PIN, OUTPUT);
              pinMode(XLAT_PIN, OUTPUT);
              pinMode(BLANK_PIN, OUTPUT);
              pinMode(SIN_PIN, OUTPUT);

              digitalWrite(GSCLK_PIN, LOW);
              digitalWrite(SCLK_PIN, LOW);
              digitalWrite(XLAT_PIN, LOW);
              digitalWrite(BLANK_PIN, HIGH);*/

            this->gsclk_pin_->setup();
            this->gsclk_pin_->pin_mode(gpio::Flags::FLAG_OUTPUT);
            this->gsclk_pin_->digital_write(false);

            this->sclk_pin_->setup();
            this->sclk_pin_->pin_mode(gpio::Flags::FLAG_OUTPUT);
            this->sclk_pin_->digital_write(false);

            this->xlat_pin_->setup();
            this->xlat_pin_->pin_mode(gpio::Flags::FLAG_OUTPUT);
            this->xlat_pin_->digital_write(false);

            this->blank_pin_->setup();
            this->blank_pin_->pin_mode(gpio::Flags::FLAG_OUTPUT);
            this->blank_pin_->digital_write(true);

            this->sin_pin_->pin_mode(gpio::Flags::FLAG_OUTPUT);
            this->sin_pin_->setup();

            ESP_LOGCONFIG(TAG, "Done setting up TLC5940...");

          this->loop();
        }

        void TLC5940::dump_config() {
          ESP_LOGCONFIG(TAG, "TLC5940:");

if (this->is_failed()) {
            ESP_LOGE(TAG, "Setting up TLC5940 failed!");
          }

        LOG_PIN("  SIN Pin: ", this->sin_pin_);
        LOG_PIN("  XLAT Pin: ", this->xlat_pin_);
        LOG_PIN("  BLANK Pin: ", this->blank_pin_);
        LOG_PIN("  SCLK Pin: ", this->sclk_pin_);
        LOG_PIN("  GSCLK Pin: ", this->gsclk_pin_);

        ESP_LOGCONFIG(TAG, "  Number of chips: %u", TLC5940_N);
        }

        void TLC5940::loop() {
            uint8_t firstCycleFlag = 0;

              /*if (outputState(VPRG_PORT, VPRG_PIN)) {
                setLow(VPRG_PORT, VPRG_PIN);
                firstCycleFlag = 1;
              }*/

              uint16_t GSCLK_Counter = 0;
              uint8_t Data_Counter = 0;

//              digitalWrite(BLANK_PIN, LOW);
            this->blank_pin_->digital_write(false);

              for (;;) {
                if (GSCLK_Counter > 4095) {
                this->blank_pin_->digital_write(true);

                this->xlat_pin_->digital_write(true);
                this->xlat_pin_->digital_write(false);

                //  digitalWrite(BLANK_PIN, HIGH);
                  //digitalWrite(XLAT_PIN, HIGH);
                  //digitalWrite(XLAT_PIN, LOW);
                  if (firstCycleFlag) {
                    this->sclk_pin_->digital_write(true);
                    this->sclk_pin_->digital_write(false);
                    //digitalWrite(SCLK_PIN, HIGH);
                    //digitalWrite(SCLK_PIN, LOW);

                    firstCycleFlag = 0;
                  }
                  break;
                } else {
                  if (!(Data_Counter > TLC5940_N * 192 - 1)) {
                    if (gsData[Data_Counter]) {
                    this->sin_pin_->digital_write(true);
//                      digitalWrite(SIN_PIN, HIGH);
                    } else{
                    this->sin_pin_->digital_write(false);
//                      digitalWrite(SIN_PIN, LOW);
                    }

                    this->sclk_pin_->digital_write(true);
                    this->sclk_pin_->digital_write(false);
                    //digitalWrite(SCLK_PIN, HIGH);
                    //digitalWrite(SCLK_PIN, LOW);

                    Data_Counter++;
                  }
                }

                this->gsclk_pin_->digital_write(true);
                this->gsclk_pin_->digital_write(false);

                //digitalWrite(GSCLK_PIN, HIGH);
                //digitalWrite(GSCLK_PIN, LOW);

                GSCLK_Counter++;

              this->status_clear_warning();
//              this->update_ = false;
            }
        }
    }
}