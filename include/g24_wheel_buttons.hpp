#ifndef G24WHEELBUTTONS_HPP
#define G24WHEELBUTTONS_HPP

#include "common/common_libraries.hpp"
#include "led_strip.hpp"
#include "display_manager.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "can.hpp"

// CrowPanel 5.0" pin assignments
#include "common/crowpanel_pins.h"

// Use CrowPanel pin definitions for wheel buttons
#define B1_PIN WHEEL_B1_PIN
#define B2_PIN WHEEL_B2_PIN  
#define B3_PIN WHEEL_B3_PIN
#define B4_PIN WHEEL_B4_PIN

#define B1_LED_PIN WHEEL_B1_LED_PIN
#define B2_LED_PIN WHEEL_B2_LED_PIN
#define B3_LED_PIN WHEEL_B3_LED_PIN
#define B4_LED_PIN WHEEL_B4_LED_PIN

#define LEVA_IZQ_PIN PADDLE_LEFT_PIN 
#define LEVA_DER_PIN PADDLE_RIGHT_PIN

#define E1_PIN_A ENCODER1_A_PIN
#define E1_PIN_B ENCODER1_B_PIN
#define E2_PIN_A ENCODER2_A_PIN
#define E2_PIN_B ENCODER2_B_PIN

#define E1_BUTTON_PIN ENCODER1_BTN_PIN
#define E2_BUTTON_PIN ENCODER2_BTN_PIN

class G24WheelButtons {
public:
    G24WheelButtons();
    void begin();
    void update();
    static void updateTask(void *arg);
    void set_can_controller(CAN *canController);
    void set_led_strip(LedStrip *ledStrip);
    void set_display_manager(DisplayManager *displayManager);

private:
    LedStrip *_led_strip;
    DisplayManager *_display_manager;
    void handleButtonPress(gpio_num_t buttonPin);
    void handleButtonRelease(gpio_num_t buttonPin);
    static void IRAM_ATTR handleEncoderInterrupt(void* arg);
    void handleClockWise(gpio_num_t encoderPin);
    void handleCounterClockWise(gpio_num_t encoderPin);
    void checkButtonState(gpio_num_t buttonPin, volatile bool &buttonState, volatile unsigned long &lastPressTime, int ledPin);

    static const unsigned long debounceTime = 50; // milliseconds
    volatile unsigned long lastPressTimeB1;
    volatile unsigned long lastPressTimeB2;
    volatile unsigned long lastPressTimeB3;
    volatile unsigned long lastPressTimeB4;
    volatile unsigned long lastPressTimeLevaIzq;
    volatile unsigned long lastPressTimeLevaDer;
    volatile unsigned long lastPressTimeE1;
    volatile unsigned long lastPressTimeE2;
    volatile unsigned long lastTurnTimeE1;
    volatile unsigned long lastTurnTimeE2;

    volatile bool buttonStateB1;
    volatile bool buttonStateB2;
    volatile bool buttonStateB3;
    volatile bool buttonStateB4;
    volatile bool buttonStateLevaIzq;
    volatile bool buttonStateLevaDer;
    volatile int encoderCounterE1;
    volatile int encoderCounterE2;
    volatile bool buttonStateE1;
    volatile bool buttonStateE2;

    volatile int displayCounter;
    volatile int lastDispayCounter;

    volatile int brightnessCounter;
    volatile int lastBrightnessCounter;

    volatile int lastPin_A_StateE1;
    volatile int lastPin_A_StateE2;

    CAN* canController;
};

#endif