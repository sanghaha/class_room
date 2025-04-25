#pragma once

#include "Singleton.h"

class BitmapTexture;
class UIManager : public Singleton<UIManager>
{
public:
	void Init();
	void Render(HDC hdc);

private:
};

