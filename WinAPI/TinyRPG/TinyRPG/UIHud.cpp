#include "pch.h"
#include "UIHud.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Game.h"
#include "GameScene.h"
#include "Player.h"

UIHud::UIHud()
{
	_isOpen = true;
}

UIHud::~UIHud()
{
}

void UIHud::Init()
{
	_hpBar = dynamic_cast<Sliced3Texture*>(ResourceManager::GetInstance()->GetTexture(L"Health_03"));
	_hpValue = dynamic_cast<Sliced3Texture*>(ResourceManager::GetInstance()->GetTexture(L"Health_03_Bar01"));
	//_texture = dynamic_cast<Sliced3Texture*>(ResourceManager::GetInstance()->GetTexture(L"Ribbon_Red_3Slides"));
}

void UIHud::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (_isOpen == false)
		return;

	Player* player = Game::GetGameScene()->GetPlayer();
	if (nullptr == player)
		return;

	if (_hpBar && _hpValue)
	{
		Vector pos(10, GWinSizeY - 70);
		_hpBar->Render(renderTarget, pos, 150, 40);

		float ratio = player->GetHp() / (float)player->GetMaxHp();
		_hpValue->Render(renderTarget, pos, 150, 40, ratio);
	}

	// 공격력
	{
		//// 폰트 가져오기
		//Gdiplus::Font* font = ResourceManager::GetInstance()->GetFont(20);
		//if (!font)
		//	return;

		//// 텍스트 렌더링
		//wstring str = std::to_wstring(player->GetAttack());
		//Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255)); // 흰색 텍스트
		//Gdiplus::PointF point(150.0f, GWinSizeY - 50);
		//graphics->DrawString(str.c_str(), -1, font, point, &brush);
	}
}
