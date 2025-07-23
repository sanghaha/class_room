#pragma once

#include "Component.h"

class RectCollider : public Component
{
public:
	RectCollider(class Actor* owner, float width, float height);
	virtual ~RectCollider();

	void InitComponent() override;
	void UpdateComponent(float deltaTime) override;
	void RenderComponent(HDC hdc, Vector pos) override;

	MyRect* GetCollisionRect() { return &_collision; }

private:
	class Actor* _owner = nullptr;
	MyRect _collision = {}; // �浹 ����
	int32 _width = 0; // �ʺ�
	int32 _height = 0; // ����
};

