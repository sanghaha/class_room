#pragma once
#include "Scene.h"


class EditScene : public Scene
{
public:
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

private:
	void save_basic(std::wstring fileName);
	void load_basic(std::wstring fileName);

	void save_dialog();
	void load_dialog();

private:
	vector<pair<POINT, POINT>> _lines;

	bool _drawing = false;
	POINT _startPos;

};

