#pragma once

#include "esphome.h"
#include <utility>
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace uart_readline {

class UartReadLine : public Component, public uart::UARTDevice, public text_sensor::TextSensor {
 public:
  //UartReadLine(uart::UARTComponent *parent) : uart::UARTDevice(parent) {}
  //UartReadLine() : uart::UARTDevice() {}

  void setup() override {
    // nothing to do here
  }

  int readline(int readch, char *buffer, int len)
  {
    static int pos = 0;
    int rpos;

    if (readch > 0) {
      switch (readch) {
        case '\n':
        case '\r': // Return on CR or newline
          buffer[pos] = 0; // Just to be sure, set last character 0
          rpos = pos;
          pos = 0;  // Reset position index ready for next time
          return rpos;
        default:
          if ((pos < len-1) && ( readch < 127 )) { // Filter on <127 to make sure it is a character
            buffer[pos++] = readch;
            buffer[pos] = 0;
          }
          else
          {
            buffer[pos] = 0; // Just to be sure, set last character 0
            rpos = pos;
            pos = 0;  // Reset position index ready for next time
            return rpos;
          }
      }
    }
    // No end of line has been found, so return -1.
    return -1;
  }

  void loop() override {
    const int max_line_length = 80;
    static char buffer[max_line_length];
    while (available()) {
      if(readline(read(), buffer, max_line_length) > 0) {
        publish_state(buffer);
      }
    }
  }
};

}
}