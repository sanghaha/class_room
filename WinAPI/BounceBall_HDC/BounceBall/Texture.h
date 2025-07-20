#pragma once

#include "BaseResource.h"

class Sliced3Texture : public BaseResource
{
	using Super = BaseResource;
public:
	Sliced3Texture(wstring key, int32 width, int32 height, int32 left, int32 right);
	virtual ~Sliced3Texture();

	void SetRatio(float ratio) { _ratio = ratio; }
	void Render(HDC renderTarget, Vector pos) override;

private:
	int32 _left = 0;
	int32 _right = 0;
	float _ratio = 0.0f;
};

class Texture : public BaseResource
{
	using Super = BaseResource;
public:
	Texture(wstring key, int32 width = 0, int32 height = 0);
	virtual ~Texture();

	void Render(HDC renderTarget, Vector pos) override;
	
private:
};
