#include "../include/crowpanel_controller.hpp"

uint32_t CrowPanelController::screenWidth;
uint32_t CrowPanelController::screenHeight;
lv_disp_draw_buf_t CrowPanelController::draw_buf;
lv_color_t CrowPanelController::disp_draw_buf[800 * 480 / 10];
lv_disp_drv_t CrowPanelController::disp_drv;
lv_indev_drv_t CrowPanelController::indev_drv;

CrowPanelController::CrowPanelController()
{
    Wire.begin(19, 20);
    pinMode(38, OUTPUT);
    digitalWrite(38, LOW);
    lcd.begin();
    lcd.setRotation(2);  // Rotate display 180 degrees (upside down)
    lcd.fillScreen(TFT_BLACK);
    delay(200);

    lv_init();

    screenWidth = lcd.width();
    screenHeight = lcd.height();
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight / 10);
    lv_disp_drv_init(&disp_drv);

    disp_drv.user_data = this;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;

    lv_indev_drv_register(&indev_drv);
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    ui_init();
    lv_timer_handler();
}

void CrowPanelController::my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    CrowPanelController* controller = static_cast<CrowPanelController*>(disp->user_data);
    LGFX& lcd = controller->lcd;

    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    lcd.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t*)&color_p->full);
#else
    lcd.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t*)&color_p->full);
#endif

    lv_disp_flush_ready(disp);
}

void CrowPanelController::set_value_to_label(lv_obj_t *label, double value)
{
    if (value == (int)value) {
        lv_label_set_text_fmt(label, "%d", (int)value);
    } else {
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%.2f", value);
        lv_label_set_text(label, buffer);
    }
}

void CrowPanelController::set_string_to_label(lv_obj_t *label, const char *string){
    lv_label_set_text(label, string);
}

void CrowPanelController::change_screen(lv_obj_t *screen){
    if (screen != NULL) {
        lv_disp_load_scr(screen);
    }
}

// New color management methods
void CrowPanelController::set_label_color(lv_obj_t *label, uint32_t color) {
    if (label != NULL) {
        lv_obj_set_style_text_color(label, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void CrowPanelController::set_panel_color(lv_obj_t *panel, uint32_t bg_color) {
    if (panel != NULL) {
        lv_obj_set_style_bg_color(panel, lv_color_hex(bg_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void CrowPanelController::set_conditional_colors() {
    // This method will be called from data_processor.cpp to update colors based on conditions
    // Implementation will be added when specific conditions are defined
}

void CrowPanelController::set_panel_default_style(lv_obj_t *panel) {
    if (panel != NULL) {
        lv_obj_set_style_bg_color(panel, lv_color_hex(COLOR_PANEL_DEFAULT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(panel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(panel, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);  // Slightly lighter border
    }
}