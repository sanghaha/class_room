#pragma once
#include "Scene.h"

// �ƹ��͵� ���ϴ� ��
class EmptyScene : public Scene
{
public:
	EmptyScene() {}
	virtual ~EmptyScene() {}

	void loadResources() override {}
	void createObjects() override {}
	void initTimer() override {}
};

