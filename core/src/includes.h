#define OBF_BEGIN try { obf::next_step __crv = obf::next_step::ns_done; std::shared_ptr<obf::base_rvholder> __rvlocal;

// for the hide window
namespace features {
    extern int window_key;
    extern bool hideWindow;
}

#pragma once
#define DIRECTINPUT_VERSION 0x0800

// Lib
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"

#include "xorstr.hpp"
#include "instr.h"

#include <d3d9.h>
#include <dinput.h>

// C++
#include <tchar.h>
#include <math.h>
#include <thread>

#include <thread>
#include <random>
#include <future>
#include <iostream>
#include <random>
#include <fstream>

// Includes
#include "../Ubuntu-Light.h"
#include "../Ubuntu_Regular.h"

#include "Clicker.h"

#define OBF_END } catch(std::shared_ptr<obf::base_rvholder>& r) { return *r; } catch (...) {throw;}
