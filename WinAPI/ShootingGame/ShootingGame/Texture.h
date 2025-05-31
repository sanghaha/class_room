#pragma once
#include "ResourceBase.h"

class Texture : public ResourceBase
{
	using Super = ResourceBase;

public:
	Texture(wstring key, int32 width, int32 height);
	virtual ~Texture();

	void Render(HDC hdc, Pos pos) override;	
private:
	
};

