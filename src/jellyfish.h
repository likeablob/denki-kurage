#ifndef JELLYFISH_H
#define JELLYFISH_H

#include "config.h"
#include "types.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

void drawJellyfish(TFT_eSprite &canvas, Point2D bell[NUM_BELL_VERTICES],
                   Point2D tentacles[NUM_TENTACLES][TENTACLE_SEGMENTS],
                   ColorMode mode, bool wireframe);
uint16_t getJellyfishColor(ColorMode mode, float brightness = 1.0f);

#endif
