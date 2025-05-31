#pragma once
#include "Actor.h"

class Texture;

class Map : public Actor
{
	using Super = Actor;
public:
	Map(Pos pos);
	virtual ~Map();
	
	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Background; }
	class ColliderCircle* GetCollider() override { return nullptr; }

private:
	Texture* _texture = nullptr;
	float _moveSpeed = 300;		// �ʴ� 100�ȼ���ŭ
	int32 _textureHeight = 0;	// �ؽ����� ���� ����

	// �ΰ��� Map Texture�� �����̼��Ѵ�
	Pos _pos2 = {};
};

