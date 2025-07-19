#include "include/data_processor.hpp"
#include "include/can.hpp"
#include "include/g24_wheel_buttons.hpp"
#include "include/led_strip.hpp"
#include "include/crowpanel_controller.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

DataProcessor dataProcessor;
// CAN canController;
G24WheelButtons wheelButtons;
LedStrip ledStrip;
CrowPanelController crowPanelController;
int a = 0;

void setup() {
    Serial.begin(115200);
    // canController.set_data_proccessor(&dataProcessor);
    // dataProcessor.set_led_strip(&ledStrip);
    // wheelButtons.set_led_strip(&ledStrip);
    // wheelButtons.set_can_controller(&canController);
    // wheelButtons.set_data_processor(&dataProcessor);
    // ledStrip.set_mutex(canController.get_mutex());
    

    wheelButtons.begin();
    // canController.start();
    xTaskCreate(wheelButtons.updateTask, "updateTask", 4096, &wheelButtons, 1, NULL);
}

void loop(){ 
    // canController.listen();
    crowPanelController.set_value_to_label(ui_rpm, a);
    a++;
    lv_timer_handler();
    vTaskDelay(5);
}