#include "Clicker.h"
#define OBF_BEGIN try { obf::next_step __crv = obf::next_step::ns_done; std::shared_ptr<obf::base_rvholder> __rvlocal;

float generate_random_float(float start, float end)
{
    // https://github.com/EternalRift/clicker/blob/main/source/utils/utils.hpp#L55
    // mersenne twister engine with a random seed based on the clock (once at system startup)
    static std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
    const std::uniform_real_distribution<float> distribution(start, end);
    return distribution(mersenne);
}

// from b1scoito's clicker
    /// <summary>
    /// Precisely sleep for seconds
    /// </summary>
    /// <param name="seconds"></param>
void precise_timer_sleep(double seconds)
{
    while (seconds > 5e-3)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto end = std::chrono::high_resolution_clock::now();

        auto observed = (end - start).count() / 1e9;
        seconds -= observed;
    }

    /* ~~ Spin lock */
    auto start = std::chrono::high_resolution_clock::now();
    while ((std::chrono::high_resolution_clock::now() - start).count() / 1e9 < seconds);
}

/// <summary>
/// Sleep wrapper
/// </summary>
/// <param name="ms"></param>
void sleep_precisely(float ms)
{
    precise_timer_sleep(static_cast<double>(ms / 1000.f));
}

namespace features
{


    // Left Clicker
    bool CLICKER_TOGGLED = false;
    bool LEFT_CLICK = false;
    int left_key = 0x56; // V

    float CPS = 13;
    int CLICK_CHANCE = 90;
    bool lExtraRand = false;
    bool lDrag = false;

    // Blockhit
    bool BLOCKHIT = false;
    int BLOCK_CHANCE = 6;

    // Right Clicker

    bool rCLICKER_TOGGLED = false;
    bool RIGHT_CLICK = false;
    int right_key = 0x43; // C key

    float rCPS = 13;
    int  rCLICK_CHANCE = 90;
    bool rExtraRand = false;

    void clickerThread() {
        while (true)
        {
            auto hwnd = GetForegroundWindow();

            // LEFT CLICKER
            float lcps_min = CPS - 3;
            float lcps_max = CPS + 1;
            if ((int)CPS <= 15) {
                int CLICK_CHANCE = 90;
            }
            if ((int)CPS >= 16) {
                int CLICK_CHANCE = 85;
            }
            // if your cps is greater or equal to 15 get more rand
            if ((int)CPS <= 15) {
                lcps_min = CPS - 4;
                lcps_max = CPS + 2;
            }

            // RIGHT CLICKER
            float rcps_min = rCPS - 3;
            float rcps_max = rCPS + 1;
            if ((int)rCPS <= 15) {
                int rCLICK_CHANCE = 90;
            }
            if ((int)rCPS >= 16) {
                int rCLICK_CHANCE = 85;
            }
            // if your cps is greater or equal to 15 get more rand
            if ((int)rCPS <= 15) {
                rcps_min = rCPS - 4;
                rcps_max = rCPS + 2;
            }

            if (GetAsyncKeyState(VK_LBUTTON) && CLICKER_TOGGLED) {
                static int lastCps;
                

                // Rand for Left
                if (true) {

                    SendMessageW(hwnd, 0x201, MK_LBUTTON, MAKELPARAM(0, 0));

                    float randCPS = generate_random_float(lcps_min, lcps_max);
                    if ((int)randCPS == (int)lastCps) {
                        randCPS = generate_random_float(lcps_min - 1, lcps_max - 1);
                        lastCps = randCPS;
                    }


                    if ((int)generate_random_float(1, 7) == 3) {
                        randCPS - 1;
                    }
                    if ((int)generate_random_float(1, 5) == 3) {
                        randCPS + 2;
                    }
                    if ((int)generate_random_float(1, 15) == 3) {
                        sleep_precisely(1000 / generate_random_float(lcps_max, lcps_min));
                    }


                    lastCps = randCPS;

                    sleep_precisely(500 / generate_random_float(randCPS - 1, randCPS - 0.1));

                    SendMessageW(hwnd, 0x202, MK_LBUTTON, MAKELPARAM(0, 0));


                    sleep_precisely(300 / generate_random_float(randCPS - 3, randCPS));



                
                }

            }




            if (GetAsyncKeyState(VK_RBUTTON) && rCLICKER_TOGGLED) {
                auto doRightClick = [&]() {
                    SendMessageW(hwnd, 0x204, MK_LBUTTON, MAKELPARAM(0, 0));
                    sleep_precisely(generate_random_float(7, 13));
                    SendMessageW(hwnd, 0x205, MK_LBUTTON, MAKELPARAM(0, 0));

                    
                };

                if (rExtraRand) {
                    if (rand() % 100 < rCLICK_CHANCE) {
                        doRightClick();
                    }
                }
                else if (!rExtraRand) {
                    doRightClick();
                }

                // Rand for Right
                if ((int)CPS <= 15) {
                    sleep_precisely(800 / generate_random_float((int)rcps_min, (int)rcps_max));
                }
                if ((int)CPS >= 16) {
                    sleep_precisely(600 / generate_random_float((int)rcps_min, (int)rcps_max));
                }
            }

        }
    }
}
#define OBF_END } catch(std::shared_ptr<obf::base_rvholder>& r) { return *r; } catch (...) {throw;}
