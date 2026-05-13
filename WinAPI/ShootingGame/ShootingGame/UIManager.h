#pragma once

#include "Singleton.h"

class Texture;

class UIManager : public Singleton<UIManager>
{
public:
	void Init();
	void Render(HDC hdc);
	void Destroy() override;
	wstring GetName() override { return L"UIManager"; };

private:
	Texture* _hpTexture = nullptr;
};

