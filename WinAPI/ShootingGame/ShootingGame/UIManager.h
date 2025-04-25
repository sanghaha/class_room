#pragma once

#include "Singleton.h"

class Texture;
class UIManager : public Singleton<UIManager>
{
public:
	void Init();
	void Render(HDC hdc);

	wstring GetName() override { return L"UIManager"; };

private:
	vector<Texture*> _hpTexture;
};

