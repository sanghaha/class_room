#pragma once

#include <iostream>
#include <windows.h>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>
#include <queue>


// random
#include <random>

// C++ 20
#include <format>
#include <filesystem>
namespace fs = std::filesystem;

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

// transparent Blt
#pragma comment(lib, "msimg32.lib")

// GDI+
//#include <gdiplus.h>
//#pragma comment(lib, "gdiplus.lib")
//using namespace Gdiplus;

// Direct 2D
#include <d2d1.h>
#include <dwrite_1.h>
#include <dwrite_3.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

// Json
#include <fstream>
#include "Json/nlohmann/json.hpp" // nlohmann/json 헤더 파일
using json = nlohmann::json;

using namespace std;
#include "Defines.h"


