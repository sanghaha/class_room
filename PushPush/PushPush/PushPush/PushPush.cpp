#include <iostream>
#include <Windows.h>
#include "Helper.h"
#include "Map.h"
#include "Player.h"

using namespace std;



int main()
{
	SetCursorOnOff(false);

	while (true)
	{
		// 입력
		HandleKeyInput();

		// 로직
		HandleMove();

		// 출력
		PrintMap1D();

		cout << "X : " << GPlayerX << ", Y : " << GPlayerY << endl;
		if (GGameOver)
		{
			cout << "클리어!!" << endl;
		}
	}
	return 0;
}