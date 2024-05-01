#pragma once

#include <Arduino.h>
#define PATTERNS_COUNT 8
// 1 - R green; 0 - L green
const uint8_t PATTERNS[][10] PROGMEM = {
    {1, 1, 0, 1, 0, 1, 0, 0, 0, 1}, // 0
    {1, 1, 1, 0, 0, 1, 0, 0, 0, 1}, // 1
    {0, 1, 0, 0, 1, 1, 1, 0, 1, 1}, // 2
    {0, 0, 1, 0, 0, 0, 1, 0, 1, 1}, // 3
    {1, 1, 1, 0, 1, 0, 1, 0, 0, 0}, // 4
    {0, 1, 0, 1, 1, 0, 1, 1, 0, 1}, // 5
    {0, 0, 0, 0, 1, 1, 0, 1, 0, 1}, // 6
    {1, 0, 1, 0, 1, 0, 0, 1, 1, 1}, // 7
};