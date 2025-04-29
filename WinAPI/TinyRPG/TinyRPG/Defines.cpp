#include "pch.h"
#include "Defines.h"

void PrintLog(wstring log)
{
	OutputDebugStringW((log + L"\n").c_str());
}

int32 RandRange(int32 min, int32 max)
{
    static std::random_device rd; // �ϵ���� ��Ʈ���� �ҽ�
    static std::mt19937 gen(rd()); // Mersenne Twister ����

    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}
