#pragma once
#include "Scene.h"

// 아무것도 안하는 씬
class EmptyScene : public Scene
{
public:
	EmptyScene() {}
	virtual ~EmptyScene() {}

	void loadResources() override {}
	void createObjects() override {}
	void initTimer() override {}
};

