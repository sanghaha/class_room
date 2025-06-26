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

// C++ 20
#include <format>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

// Json
#include <fstream>
#include "Json/nlohmann/json.hpp" // nlohmann/json 헤더 파일
using json = nlohmann::json;

// magic_enum
#include "magic_enum/magic_enum.hpp"

// transparent Blt
#pragma comment(lib, "msimg32.lib")

#include "Defines.h"

// 메모리 릭
#define _CRTDBG_MAP_ALLOC  
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
