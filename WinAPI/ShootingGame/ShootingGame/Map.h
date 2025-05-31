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
	float _moveSpeed = 300;		// 초당 100픽셀만큼
	int32 _textureHeight = 0;	// 텍스쳐의 세로 길이

	// 두개의 Map Texture를 로테이션한다
	Pos _pos2 = {};
};

