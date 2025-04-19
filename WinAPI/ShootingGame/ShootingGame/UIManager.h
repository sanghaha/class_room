#pragma once

#include "Singleton.h"

class Texture;
class UIManager : public Singleton<UIManager>
{
public:
	void Init();
	void Render(HDC hdc);

private:
	vector<Texture*> _hpTexture;
};

