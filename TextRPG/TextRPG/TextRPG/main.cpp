// Project1.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#include <iostream>
#include <thread>
#include <chrono>
#include "TextRPG.h"

using namespace std;

int main()
{
    srand((unsigned int)time(0)); // ���� �õ� �ʱ�ȭ

    InitCharacter();

    while (true)
    {
        SelectCharacter();
        PrintPlayerStat();

        // ��������
        while (true)
        {
            cout << "---------------------------------" << endl;
            cout << "1. ���� ����, 2. ĳ���� ����, 3. ����" << endl;
            cout << "---------------------------------" << endl;
            cout << ">> ";

            int input;
            cin >> input;

            if (input == Menu::Menu_SelectCharacter)
            {
                system("cls"); // ȭ�� �����
                break;
            }
            else if (input == Menu::Menu_Quit)
            {
                return 0;
            }

            bool complete = EnterDungeon();
            if (complete)
            {
                // ���� Ž���� ����Ǿ����� �����.
                bool nextGame = CheckNextGame();
                if (nextGame == false)
                {
                    return 0;
                }
                system("cls"); // ȭ�� �����
            }
        }
    }
}
