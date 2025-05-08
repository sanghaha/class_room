#include "pch.h"
#include "UIHud.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "Game.h"
#include "GameScene.h"
#include "Player.h"

UIHud::UIHud()
{
	_isOpen = true;
}

UIHud::~UIHud()
{
	SAFE_DELETE(_hpBar);
	SAFE_DELETE(_hpValue);
	SAFE_DELETE(_attackIcon);
	SAFE_DELETE(_attackValue);
}

void UIHud::Init()
{
	_hpBar = new Sliced3Texture(L"Health_03", 20, 20);
	_hpValue = new Sliced3Texture(L"Health_03_Bar01", 20, 20);
	_attackIcon = new Sprite(L"HudIcons");

	SpriteRenderInfo info;
	info.indexX = 1;
	info.indexY = 0;
	info.alignCenter = false;
	info.applyCamera = false;
	_attackIcon->SetInfo(info);

	_attackValue = new NumberSprite(L"Numbers");
	info.width = 15;
	info.height = 20;
	_attackValue->SetInfo(info);
}

void UIHud::Render(ID2D1RenderTarget* renderTarget)
{
	if (_isOpen == false)
		return;

	Player* player = Game::GetGameScene()->GetPlayer();
	if (_hpBar && _hpValue)
	{
		Vector pos(10, GWinSizeY - 70);
		_hpBar->Render(renderTarget, pos, 150, 40);

		float ratio = player ? player->GetHp() / (float)player->GetMaxHp() : 0;
		_hpValue->Render(renderTarget, pos, 150, 40, ratio);
	}

	// °ø°Ý·Â
	{
		Vector pos(180, GWinSizeY - 70);
		_attackIcon->Render(renderTarget, pos);

		_attackValue->SetNumber(player ? player->GetAttack() : 0);
		_attackValue->Render(renderTarget, pos + Vector(35, 5));

		//auto brush = ResourceManager::GetInstance()->GetBrush(BrushColor::White);
		//auto font = ResourceManager::GetInstance()->GetFont(FontSize::FONT_30);

		//wstring str = std::to_wstring(player ? player->GetAttack() : 0);
		//renderTarget->DrawTextW(
		//	str.c_str(),
		//	(uint32)str.size(),
		//	font,
		//	D2D1::RectF(pos.x + 36, pos.y, pos.x + 136, pos.y + 50),
		//	brush
		//);
	}
}
