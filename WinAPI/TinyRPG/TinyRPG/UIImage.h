#pragma once
#include "UIWidget.h"

class Texture;
class Sprite;

class UIImage: public UIWidget
{
	using Super = UIWidget;

public:
	UIImage(Vector pos, string key, int32 width = 0, int32 height = 0);
	virtual ~UIImage();

private:
};

class UISprite : public UIWidget
{
	using Super = UIWidget;

public:
	UISprite(Vector pos, string spriteName, int32 width = 0, int32 height = 0);
	virtual ~UISprite();

	void SetIndex(int32 x, int32 y);

private:
	Sprite* _sprite = nullptr;
};

class UISliced3 : public UIWidget
{
	using Super = UIWidget;
public:
	UISliced3(Vector pos, string key, int32 width, int32 height, int32 left, int32 right);
	virtual ~UISliced3();

	void SetRatio(float ratio) { _ratio = ratio; }
	void Render(ID2D1RenderTarget* renderTarget) override;

private:
	Texture* _texture = nullptr;
	int32 _left = 0;
	int32 _right = 0;
	float _ratio = 1.0f;
};

class UINumber : public UIWidget
{
	using Super = UIWidget;
public:
	UINumber(Vector pos, string key, int32 width, int32 height);
	virtual ~UINumber();

	void SetNumber(int8 number);
	void Render(ID2D1RenderTarget* renderTarget) override;

private:
	Sprite* _sprite = nullptr;
	int8 _number = 0;
	std::vector<Vector> _numberPos;
};
