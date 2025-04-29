#include "pch.h"
#include "Defines.h"

void PrintLog(wstring log)
{
	OutputDebugStringW((log + L"\n").c_str());
}

int32 RandRange(int32 min, int32 max)
{
    static std::random_device rd; // 하드웨어 엔트로피 소스
    static std::mt19937 gen(rd()); // Mersenne Twister 엔진

    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}
