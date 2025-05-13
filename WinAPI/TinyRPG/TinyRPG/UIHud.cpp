#include "pch.h"
#include "UIHud.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "Game.h"
#include "GameScene.h"
#include "Player.h"
#include "UIImage.h"

UIHud::UIHud()
{
	_isOpen = true;

	// Hp bar
	{
		Vector pos(10, GWinSizeY - 76);
		CreateSliced3(pos, "Health_03", 20, 20);
		_hpValue = CreateSliced3(pos, "Health_03_Bar01", 20, 20);
	}
	{
		Vector pos(200, GWinSizeY - 76);
		CreateSprite(pos, "HUD_AttackIcon", 40, 40);
		_attackValue = CreateNumber(pos + Vector(40, 10), "HUD_Number", 20, 20);
	}
}

UIHud::~UIHud()
{
}

void UIHud::Init()
{
}

void UIHud::Update(float deltaTime)
{
	if (_isOpen == false)
		return;

	Super::Update(deltaTime);

	Player* player = Game::GetGameScene()->GetPlayer();
	if (nullptr == player)
		return;

	if (_hpValue)
	{
		float ratio = player ? player->GetHp() / (float)player->GetMaxHp() : 0;
		_hpValue->SetRatio(ratio);
	}
	if (_attackValue)
	{
		_attackValue->SetNumber(player ? player->GetAttack() : 0);
	}
}

void UIHud::Render(ID2D1RenderTarget* renderTarget)
{
	if (_isOpen == false)
		return;

	Super::Render(renderTarget);
}
