#pragma once

#include "Component.h"


class ImageRenderer : public Component
{
	using Super = Component;
public:
	ImageRenderer(wstring key, int32 width, int32 height);
	virtual ~ImageRenderer();

	void RenderComponent(HDC renderTarget, Pos pos) override;
	Size GetSize();
	void SetCenterAlign(bool center);
	void SetImageIndex(int index) { _index = index; }
	class Texture* GetTexture() { return _texture; }

private:
	class Texture* _texture = nullptr;
	int _index = -1;
};
