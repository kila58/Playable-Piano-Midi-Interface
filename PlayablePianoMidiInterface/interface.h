#pragma once

#include <Windows.h>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <map>
#include <utility>

#include "midifile/include/Options.h"

struct KeyElement
{
    KeyElement() {};
    KeyElement(uint32_t c, bool s) { scancode = c, shift = s; }

    uint32_t scancode;
    bool shift;
};

static inline std::unordered_map<int /*midi note*/, KeyElement /*scancode*/> keymap = {
{ 36, KeyElement(0x2, false) },
{ 37, KeyElement(0x2, true) },
{ 38, KeyElement(0x3, false) },
{ 39, KeyElement(0x3, true) },
{ 40, KeyElement(0x4, false) },
{ 41, KeyElement(0x5, false) },
{ 42, KeyElement(0x5, true) },
{ 43, KeyElement(0x6, false) },
{ 44, KeyElement(0x6, true) },
{ 45, KeyElement(0x7, false) },
{ 46, KeyElement(0x7, true) },
{ 47, KeyElement(0x8, false) },
{ 48, KeyElement(0x9, false) },
{ 49, KeyElement(0x9, true) },
{ 50, KeyElement(0xA, false) },
{ 51, KeyElement(0xA, true) },
{ 52, KeyElement(0xB, false) },
{ 53, KeyElement(0x10, false) },
{ 54, KeyElement(0x10, true) },
{ 55, KeyElement(0x11, false) },
{ 56, KeyElement(0x11, true) },
{ 57, KeyElement(0x12, false) },
{ 58, KeyElement(0x12, true) },
{ 59, KeyElement(0x13, false) },
{ 60, KeyElement(0x14, false) },
{ 61, KeyElement(0x14, true) },
{ 62, KeyElement(0x15, false) },
{ 63, KeyElement(0x15, true) },
{ 64, KeyElement(0x16, false) },
{ 65, KeyElement(0x17, false) },
{ 66, KeyElement(0x17, true) },
{ 67, KeyElement(0x18, false) },
{ 68, KeyElement(0x18, true) },
{ 69, KeyElement(0x19, false) },
{ 70, KeyElement(0x19, true) },
{ 71, KeyElement(0x1E, false) },
{ 72, KeyElement(0x1F, false) },
{ 73, KeyElement(0x1F, true) },
{ 74, KeyElement(0x20, false) },
{ 75, KeyElement(0x20, true) },
{ 76, KeyElement(0x21, false) },
{ 77, KeyElement(0x22, false) },
{ 78, KeyElement(0x22, true) },
{ 79, KeyElement(0x23, false) },
{ 80, KeyElement(0x23, true) },
{ 81, KeyElement(0x24, false) },
{ 82, KeyElement(0x24, true) },
{ 83, KeyElement(0x25, false) },
{ 84, KeyElement(0x26, false) },
{ 85, KeyElement(0x26, true) },
{ 86, KeyElement(0x2C, false) },
{ 87, KeyElement(0x2C, true) },
{ 88, KeyElement(0x2D, false) },
{ 89, KeyElement(0x2E, false) },
{ 90, KeyElement(0x2E, true) },
{ 91, KeyElement(0x2F, false) },
{ 92, KeyElement(0x2F, true) },
{ 93, KeyElement(0x30, false) },
{ 94, KeyElement(0x30, true) },
{ 95, KeyElement(0x31, false) },
{ 96, KeyElement(0x32, false) },
};

class Interface
{
public:
	static void MidiIn();
	static void MidiParse(int argc, char** argv);
};

inline void KeyDown(uint32_t key)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = key;
    input.ki.dwFlags = KEYEVENTF_SCANCODE;

    SendInput(1, &input, sizeof(INPUT));
}

inline void KeyUp(uint32_t key)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = key;
    input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    SendInput(1, &input, sizeof(INPUT));
}