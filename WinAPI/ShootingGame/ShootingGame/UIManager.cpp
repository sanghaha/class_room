#include "pch.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "GameScene.h"
#include "Player.h"
#include "Texture.h"
#include "Game.h"

void UIManager::Init()
{
	Player* player = Game::GetGameScene()->GetPlayer();
	if (player)
	{
		//int32 hp = player->GetHp();
		_hpTexture = new Texture(L"PlayerHP", 0, 0);
	}
}

void UIManager::Render(HDC hdc)
{
	// score
	{
		HFONT hFont = CreateFont(
			-16,			   // 글자 높이 (픽셀 단위로 변환). 음수는 실제 사이즈. 
			0,                 // 글자 너비 (0이면 자동 설정)
			0,                 // 글자 기울기 각도
			0,                 // 글자 베이스라인 각도
			FW_BOLD,           // 글자 굵기 (FW_NORMAL, FW_BOLD 등)
			FALSE,             // 이탤릭 여부
			FALSE,             // 밑줄 여부
			FALSE,             // 취소선 여부
			DEFAULT_CHARSET,   // 문자 집합
			OUT_DEFAULT_PRECIS,// 출력 정밀도
			CLIP_DEFAULT_PRECIS,// 클리핑 정밀도
			DEFAULT_QUALITY,   // 출력 품질
			DEFAULT_PITCH | FF_SWISS, // 글꼴 피치와 가족
			L"Arial"           // 글꼴 이름
		);

		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

		// 글자 색상 설정
		SetTextColor(hdc, RGB(255, 255, 255)); // 흰색
		SetBkMode(hdc, TRANSPARENT);           // 배경 투명

		// 텍스트 출력
		wstring str = std::format(L"Score : {0}", Game::GetGameScene()->GetScore());

		// 텍스트 크기 계산
		SIZE textSize;
		GetTextExtentPoint32(hdc, str.c_str(), static_cast<int32>(str.size()), &textSize);

		// 화면 중앙 좌표 계산
		int x = (GWinSizeX - textSize.cx) / 2;
		int y = 10;

		TextOut(hdc, x, y, str.c_str(), static_cast<int32>(str.size()));

		// 이전 폰트 복원 및 새 폰트 삭제
		SelectObject(hdc, oldFont);
		DeleteObject(hFont);
	}

	Player* player = Game::GetGameScene()->GetPlayer();
	if (player)
	{
		int32 hp = player->GetHp();

		// HP 텍스쳐 그리기
		for (int32 i = 0; i < hp; ++i)
		{
			if (_hpTexture == nullptr)
				continue;
			_hpTexture->Render(hdc, Pos{ (float)(i * (_hpTexture->GetSize().w + 5)) + 10, (float)GWinSizeY - _hpTexture->GetSize().h - 30});
		}
	}
	else
	{
		// 이전 폰트를 저장하고 새 폰트를 선택
		// 원하는 폰트 생성
		{
			HFONT hFont = CreateFont(
				-60,			   // 글자 높이 (픽셀 단위로 변환). 음수는 실제 사이즈. 
				0,                 // 글자 너비 (0이면 자동 설정)
				0,                 // 글자 기울기 각도
				0,                 // 글자 베이스라인 각도
				FW_BOLD,           // 글자 굵기 (FW_NORMAL, FW_BOLD 등)
				FALSE,             // 이탤릭 여부
				FALSE,             // 밑줄 여부
				FALSE,             // 취소선 여부
				DEFAULT_CHARSET,   // 문자 집합
				OUT_DEFAULT_PRECIS,// 출력 정밀도
				CLIP_DEFAULT_PRECIS,// 클리핑 정밀도
				DEFAULT_QUALITY,   // 출력 품질
				DEFAULT_PITCH | FF_SWISS, // 글꼴 피치와 가족
				L"Arial"           // 글꼴 이름
			);

			HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

			// 글자 색상 설정
			SetTextColor(hdc, RGB(255, 255, 255)); // 흰색
			SetBkMode(hdc, TRANSPARENT);           // 배경 투명

			// 텍스트 출력
			wstring str = std::format(L"GAME OVER!");

			// 텍스트 크기 계산
			SIZE textSize;
			GetTextExtentPoint32(hdc, str.c_str(), static_cast<int32>(str.size()), &textSize);

			// 화면 중앙 좌표 계산
			int x = (GWinSizeX - textSize.cx) / 2;
			int y = (GWinSizeY - textSize.cy) / 2;

			TextOut(hdc, x, y, str.c_str(), static_cast<int32>(str.size()));

			// 이전 폰트 복원 및 새 폰트 삭제
			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
		}
	}
}

void UIManager::Destroy()
{
	SAFE_DELETE(_hpTexture);
}
