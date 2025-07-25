#include "include/data_processor.hpp"
#include "include/can.hpp"
#include "include/g24_wheel_buttons.hpp"
#include "include/led_strip.hpp"
#include "include/crowpanel_controller.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

DataProcessor dataProcessor;
CAN canController;
G24WheelButtons wheelButtons;
LedStrip ledStrip;
CrowPanelController crowPanelController;

// Screen rotation variables
unsigned long lastScreenChange = 0;
int currentScreen = 1;
int screenCycle = 0; // 0 = screen1, 1 = screen2, 2 = screen3, 3 = screen4

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }
    Serial.println("Starting setup...");
    canController.set_data_proccessor(&dataProcessor);
    dataProcessor.set_led_strip(&ledStrip);
    dataProcessor.set_crow_panel_controller(&crowPanelController);
    // wheelButtons.set_led_strip(&ledStrip);
    // wheelButtons.set_can_controller(&canController);
    // wheelButtons.set_data_processor(&dataProcessor);
    // ledStrip.set_mutex(canController.get_mutex());

    canController.start();
    canController.start_listening_task();
    

    // wheelButtons.begin();
   
    // xTaskCreate(wheelButtons.updateTask, "updateTask", 4096, &wheelButtons, 1, NULL);
    
    // Initialize with screen 1
    lastScreenChange = millis();
}

void loop(){ 
    unsigned long currentTime = millis();
    
    if (currentScreen == 1) {
        if (currentTime - lastScreenChange >= 10000) {
            currentScreen++;
            crowPanelController.change_screen(ui_Screen2);
            lastScreenChange = currentTime;
            Serial.printf("Switched to Screen %d\n", currentScreen);
        }
    } else {
        // Screens 2-4 stay for 1 second then back to screen 1
        if (currentTime - lastScreenChange >= 2000) {
            currentScreen++;
            switch(currentScreen) {
                case 2:
                    crowPanelController.change_screen(ui_Screen2);
                    break;
                case 3:
                    crowPanelController.change_screen(ui_Screen3);
                    break;
                case 4:
                    crowPanelController.change_screen(ui_Screen4);
                    break;
            }
            
            lastScreenChange = currentTime;
            Serial.printf("Switched to Screen %d\n", currentScreen);
        }
        if(currentScreen > 4){
            currentScreen = 1;
            crowPanelController.change_screen(ui_Screen1);
            lastScreenChange = currentTime;
        }
    }

    lv_timer_handler();
    vTaskDelay(5);
}