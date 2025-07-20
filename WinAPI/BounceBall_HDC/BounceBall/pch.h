#pragma once

#include <iostream>
#define NOMINMAX
#include <windows.h>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>
#include <algorithm>
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


// ����
#include <MMSystem.h>
#include <dsound.h>
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
//#pragma comment(lib, "Shcore.lib")


// sound 
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// Json
#include <fstream>
#include "Json/nlohmann/json.hpp" // nlohmann/json ��� ����
using json = nlohmann::json;

// magic_enum
#include "magic_enum/magic_enum.hpp"

// �޸� ��. json ���̺귯�� ���ʿ��� �ؾ��� new Ű����� �浹���� �ʴ´�.
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif


using namespace std;
#include "Defines.h"


