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

	void Init();
	void Update() override;
	void Render(ID2D1HwndRenderTarget* renderTarget);
	void Open() override;
	bool OnLeftClickEvent(int32 x, int32 y) override;

private:
	bool checkClickSlot(int32 start, int32 maxCount, std::function<void(int32)> onClickSameSlot);

private:
	class PNGTexture*	_equipBG = nullptr;
	class PNGTexture*	_invenBG = nullptr;
	class PNGTexture*	_selectISlotBG = nullptr;

	Sprite*		_invenIconRenderer;

	Vector				_bgEquipPos;
	Vector				_bgInvenPos;
	Vector				_slotPos[MAX_EQUIP_SLOT + MAX_INVEN_SLOT];

	int32				_slotSize = 46;
	int32				_selectedIndex = -1;
};

