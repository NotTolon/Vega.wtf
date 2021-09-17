#include "includes.h"
#pragma once

namespace features {
    // Threads
    extern void clickerThread();

    // Left Clicker

    extern int left_key;
    extern bool CLICKER_TOGGLED;
    extern bool LEFT_CLICK;
    extern bool lDrag;

    extern float CPS;
    extern int CLICK_CHANCE;
    extern bool lExtraRand;

    // Blockhit
    extern int BLOCK_CHANCE;
    extern bool BLOCKHIT;

    // Right Clicker

    extern bool rCLICKER_TOGGLED;
    extern bool RIGHT_CLICK;
    extern int right_key;

    extern float rCPS;
    extern int rCLICK_CHANCE;
    extern bool rExtraRand;
}
