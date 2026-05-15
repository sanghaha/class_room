#pragma once
#include "Actor.h"

enum class PlayerState { Idle, Move };

class Player : public Actor
{
	using Super = Actor;
public:
	Player(Vector pos);
	virtual ~Player();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC renderTarget) override;

	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Ball; }
	ActorType GetActorType() override { return ActorType::AT_PLAYER; }
	virtual void LoadActor(std::wistringstream& steam) override;
private:
	static constexpr float MOVE_DURATION = 0.3f;
	static constexpr float IDLE_ANIM_DURATION = 0.6f;

	PlayerState _state = PlayerState::Idle;
	class SpriteRenderer* _sprite = nullptr;
	bool _facingLeft = false;

	Vector _renderPos;
	Vector _startRenderPos;
	float _moveTimer = 0;

	void changeState(PlayerState s);
	void updateIdle(float dt);
	void updateMove(float dt);
};
