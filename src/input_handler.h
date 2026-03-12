#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "config.h"
#include "types.h"
#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

class InputHandler {
  public:
    InputHandler();
    void begin();
    void loadSettings(ColorMode &mode);
    void update(ColorMode &mode, float &user_y_offset, float &angle_y);
    uint8_t getBrightness() const;
    bool isDebugMode() const { return show_debug; }
    bool isWireframeMode() const { return wireframe_mode; }
    int getVerticalDir() const {
        return vertical_dir;
    } // -1: UP, 1: DOWN, 0: NONE

  private:
    SPIClass touchSpi;
    XPT2046_Touchscreen ts;
    Preferences prefs;
    uint8_t brightness_idx;
    bool last_btn_state;
    bool last_touch_state;
    bool show_debug;
    bool wireframe_mode;
    int vertical_dir;
    unsigned long last_touch_time;
};

#endif
