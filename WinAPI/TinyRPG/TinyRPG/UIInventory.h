#pragma once
#include "UIBase.h"
#include "Sprite.h"

class PNGTexture;

class UIInventory : public UIBase
{
	using Super = UIBase;
public:
	UIInventory();
	virtual ~UIInventory();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget);
	void Open() override;
	bool OnLeftClickEvent(int32 x, int32 y) override;

private:
	bool checkClickSlot(int32 start, int32 maxCount, std::function<void(int32)> onClickSameSlot);

private:
	class UIImage*		_selectISlotBG = nullptr;
	class UISprite*		_itemIconRenderer[MAX_EQUIP_SLOT + MAX_INVEN_SLOT];

	Vector				_bgEquipPos;
	Vector				_bgInvenPos;
	Vector				_slotPos[MAX_EQUIP_SLOT + MAX_INVEN_SLOT];

	int32				_slotSize = 46;
	int32				_selectedIndex = -1;
};

