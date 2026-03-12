#include "input_handler.h"

InputHandler::InputHandler()
    : touchSpi(VSPI), ts(XPT2046_CS, XPT2046_IRQ), last_btn_state(true),
      last_touch_state(false), show_debug(false), wireframe_mode(false),
      vertical_dir(0), last_touch_time(0), brightness_idx(3) {}

void InputHandler::begin() {
    pinMode(BTN_PIN, INPUT_PULLUP);
    touchSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    ts.begin(touchSpi);
    ts.setRotation(0);
    prefs.begin("jellyfish", false);
}

void InputHandler::loadSettings(ColorMode &mode) {
    mode = (ColorMode)prefs.getInt("color_mode", (int)PURPLE);
    wireframe_mode = prefs.getBool("wire_mode", false);
    brightness_idx = prefs.getUChar("bright_idx", 3);
}

uint8_t InputHandler::getBrightness() const {
    const uint8_t levels[] = {40, 100, 180, 255};
    return levels[brightness_idx % 4];
}

void InputHandler::update(ColorMode &mode, float &user_y_offset,
                          float &angle_y) {
    bool btn_state = digitalRead(BTN_PIN);
    bool touch_trigger = false;
    bool is_touched = ts.touched();
    vertical_dir = 0;

    if(is_touched) {
        TS_Point p = ts.getPoint();
        int tx = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, SCREEN_WIDTH);
        int ty = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, SCREEN_HEIGHT);

        // Corner/Edge Squares (Priority)
        if(tx > SCREEN_WIDTH - 40) {
            if(ty < 45) { // Top Right: Solid/Wireframe Toggle
                if(!last_touch_state) {
                    wireframe_mode = !wireframe_mode;
                    prefs.putBool("wire_mode", wireframe_mode);
                }
            } else if(ty > SCREEN_HEIGHT - 45) { // Bottom Right: Debug Toggle
                if(!last_touch_state)
                    show_debug = !show_debug;
            }
        }

        // Horizontal Strips (Move UP/DOWN)
        if(tx <= SCREEN_WIDTH - 40) {
            if(ty < 45) {
                user_y_offset -= 4.0f;
                vertical_dir = -1;
            } else if(ty > SCREEN_HEIGHT - 45) {
                user_y_offset += 4.0f;
                vertical_dir = 1;
            }
        }

        // Middle area (45px to 275px)
        if(ty >= 45 && ty <= SCREEN_HEIGHT - 45) {
            // Zone 3 & 4: Left/Right for Camera Rotation
            if(tx < 80) {
                angle_y -= 0.04f;
            } else if(tx > SCREEN_WIDTH - 80) {
                angle_y += 0.04f;
            }
            // Zone 5: Center square for Color Rotation
            else if(tx >= 80 && tx <= SCREEN_WIDTH - 80) {
                if(!last_touch_state)
                    touch_trigger = true;
            }
        }

        // Clamp Y offset
        if(user_y_offset < -220.0f)
            user_y_offset = -220.0f;
        if(user_y_offset > 220.0f)
            user_y_offset = 220.0f;
    }

    // BOOT Button: Cycle Brightness
    if(btn_state == LOW && last_btn_state == HIGH) {
        brightness_idx = (brightness_idx + 1) % 4;
        prefs.putUChar("bright_idx", brightness_idx);
    }

    // Touch Trigger (MC): Cycle Color
    if(touch_trigger) {
        mode = (ColorMode)((mode + 1) % NUM_MODES);
        prefs.putInt("color_mode", (int)mode);
    }

    last_btn_state = btn_state;
    last_touch_state = is_touched;
}
