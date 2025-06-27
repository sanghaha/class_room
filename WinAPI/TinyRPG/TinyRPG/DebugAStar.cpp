#include "pch.h"
#include "DebugAStar.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Game.h"

void DebugAStar::Update(float deltaTime)
{
	if (InputManager::GetInstance()->GetButtonDown(KeyType::F1))
	{
		_drawDebug = !_drawDebug;
	}
}

void DebugAStar::Render(ID2D1RenderTarget* renderTarget)
{
	if (_drawDebug)
	{
		auto brush = ResourceManager::GetInstance()->GetBrush(BrushColor::Red);
		auto font = ResourceManager::GetInstance()->GetFont(FontSize::FONT_12);

		for (auto iter : best)
		{
			Cell pos = iter.first;
			int32 cost = iter.second;

			Vector worldPos = pos.ConvertToPos();
			Vector screenPos = Game::ConvertScreenPos(worldPos);

			int32 g = -1;
			int32 h = -1;
			auto find = openList.find(pos);
			if (find != openList.end())
			{
				g = find->second.g;
				h = find->second.h;
			}

			wstring str = std::format(L"f:{0}\ng:{1}h:{2}", cost, g, h);

			renderTarget->DrawTextW(
				str.c_str(),
				(uint32)str.size(),
				font,
				D2D1::RectF(screenPos.x, screenPos.y, screenPos.x + GTileSize, screenPos.y + GTileSize),
				brush
			);
		}
	}
}
