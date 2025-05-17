// Project1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <thread>
#include <chrono>
#include "TextRPG.h"

using namespace std;

int main()
{
    srand((unsigned int)time(0)); // 랜덤 시드 초기화

    InitCharacter();

    while (true)
    {
        SelectCharacter();
        PrintPlayerStat();

        // 던전입장
        while (true)
        {
            cout << "---------------------------------" << endl;
            cout << "1. 던전 입장, 2. 캐릭터 선택, 3. 종료" << endl;
            cout << "---------------------------------" << endl;
            cout << ">> ";

            int input;
            cin >> input;

            if (input == Menu::Menu_SelectCharacter)
            {
                system("cls"); // 화면 지우기
                break;
            }
            else if (input == Menu::Menu_Quit)
            {
                return 0;
            }

            bool complete = EnterDungeon();
            if (complete)
            {
                // 던전 탐험이 종료되었으니 물어본다.
                bool nextGame = CheckNextGame();
                if (nextGame == false)
                {
                    return 0;
                }
                system("cls"); // 화면 지우기
            }
        }
    }
}
