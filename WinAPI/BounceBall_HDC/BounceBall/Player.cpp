#include "pch.h"
#include "Player.h"
#include "SpriteRenderer.h"
#include "InputManager.h"
#include "Game.h"
#include "Scene.h"

Player::Player(Vector pos)
	: Super(pos)
{
}

Player::~Player()
{
}

void Player::Init()
{
	Super::Init();
	_sprite = CreateSpriteRenderer(L"Player", PLAYER_SIZE, PLAYER_SIZE);
	_sprite->SetFullFrame(false);

	_renderPos = GetPos();
	changeState(PlayerState::Idle);
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	switch (_state)
	{
	case PlayerState::Idle: updateIdle(deltaTime); break;
	case PlayerState::Move: updateMove(deltaTime); break;
	}
}

void Player::Render(HDC renderTarget)
{
	if (_sprite)
		_sprite->RenderComponent(renderTarget, _renderPos);
}

void Player::LoadActor(std::wistringstream& steam)
{
	Super::LoadActor(steam);
	_renderPos = GetPos();
}

void Player::changeState(PlayerState s)
{
	_state = s;
	switch (s)
	{
	case PlayerState::Idle:
		_sprite->ResetAnim(0, true, IDLE_ANIM_DURATION);
		break;
	case PlayerState::Move:
		_sprite->ResetAnim(1, false, MOVE_DURATION);
		break;
	}
}

void Player::updateIdle(float dt)
{
	InputManager* input = InputManager::GetInstance();
	Vector dir(0, 0);

	if (input->GetButtonDown(KeyType::Up))
		dir = Vector(0, -BLOCK_SIZE);
	else if (input->GetButtonDown(KeyType::Down))
		dir = Vector(0, BLOCK_SIZE);
	else if (input->GetButtonDown(KeyType::Left))
		dir = Vector(-BLOCK_SIZE, 0);
	else if (input->GetButtonDown(KeyType::Right))
		dir = Vector(BLOCK_SIZE, 0);

	if (dir.x == 0.f && dir.y == 0.f)
		return;

	Vector nextPos = GetPos() + dir;

	// 경계면 무시
	if (nextPos.x < 0 || nextPos.y < 0 ||
		nextPos.x + BLOCK_SIZE > GWinSizeX ||
		nextPos.y + BLOCK_SIZE > GWinSizeY)
		return;

	// 이동 가능한 셀인지 확인
	Cell nextCell = Cell::ConvertToCell(nextPos, BLOCK_SIZE);
	if(Game::GetScene()->CanMoveCell(nextCell) == false)
	{
		return;
	}

	_startRenderPos = _renderPos;
	_moveTimer = 0;
	SetPos(nextPos);
	changeState(PlayerState::Move);
}

void Player::updateMove(float dt)
{
	_moveTimer += dt;
	float t = min(_moveTimer / MOVE_DURATION, 1.0f);

	_renderPos = _startRenderPos + (GetPos() - _startRenderPos) * t;

	if (t >= 1.0f)
	{
		_renderPos = GetPos();
		changeState(PlayerState::Idle);
	}
}
