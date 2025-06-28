#pragma once
#include "ResourceBase.h"

class LineMesh : public ResourceBase
{
public:
	void Save(wstring path);
	void Load(wstring path);
	void Render(HDC hdc, Vector pos, float ratioX = 1.0f, float ratioY = 1.0f) const;

	vector<pair<POINT, POINT>>& GetLines() { return _lines; }

protected:
	vector<pair<POINT, POINT>> _lines;
	int32 _width = 0;
	int32 _height = 0;
};
