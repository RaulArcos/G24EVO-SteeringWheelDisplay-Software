#ifndef CAN_HPP
#define CAN_HPP

// CrowPanel 5.0" pin definitions
#include "common/crowpanel_pins.h"

// Use CrowPanel CAN pins (via external transceiver)
#define RX_PIN CAN_RX_PIN  // GPIO 44 (UART1 RX)
#define TX_PIN CAN_TX_PIN  // GPIO 43 (UART1 TX)

#define POLLING_RATE_MS 5
#define TRANSMIT_RATE_MS 20

#include "driver/twai.h"
#include "common/common_libraries.hpp"
#include "display_manager.hpp"  // Changed from data_processor.hpp
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class CAN {
public:
    CAN(): _mutex(xSemaphoreCreateMutex()) {}
    ~CAN();
    void start();
    void listen();
    void send_frame(twai_message_t message);
    twai_message_t createBoolMessage(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7);

    // Updated to use DisplayManager instead of DataProcessor
    void set_display_manager(DisplayManager *display_manager) {
        _display_manager = display_manager;
    }

    SemaphoreHandle_t get_mutex() {
        return _mutex;
    }

    static void listenTask(void *arg) {
        CAN *controller = static_cast<CAN*>(arg);
        controller->listen();
    }

private:
    twai_message_t _rx_message;
    DisplayManager *_display_manager;  // Changed from DataProcessor
    SemaphoreHandle_t _mutex;
};

#endif
