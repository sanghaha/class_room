#pragma once
#include "UIBase.h"

class UIMessage : public UIBase
{
public:
	UIMessage();
	virtual ~UIMessage();

	void Init() override;
	void Render(ID2D1RenderTarget* renderTarget) override;

	void SetText(const wstring& msg) { _message = msg; }
private:
	class Sliced3Texture* _texture = nullptr;
	wstring _message;
};

