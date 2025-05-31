#include "pch.h"
#include "Actor.h"
#include "Scene.h"
#include "Game.h"
#include "Texture.h"
#include "Sprite.h"
#include "ColliderCircle.h"

Actor::Actor(Pos pos) : _pos(pos)
{
}

Actor::~Actor()
{
	for (auto iter : _components)
	{
		SAFE_DELETE(iter);
	}
	_components.clear();
}

void Actor::Init()
{
	Game::GetScene()->UpdateGrid(this, Pos{ 0,0 }, _pos);
}

void Actor::Destroy()
{
	// 사라져야함
	Game::GetScene()->ReserveRemove(this);
}

void Actor::Update(float deltaTime)
{
	for (auto iter : _components)
	{
		iter->Update(deltaTime);
	}
}

void Actor::Render(HDC hdc)
{
	for (auto iter : _components)
	{
		iter->Render(hdc, GetPos());
	}
}

void Actor::AddPosDelta(float x, float y, bool notifyScene)
{
	Pos prevPos = _pos;

	_pos.x += x;
	_pos.y += y;

	if (notifyScene)
	{
		// Scene에 알려준다.
		Game::GetScene()->UpdateGrid(this, prevPos, _pos);
	}
}

void Actor::SetPos(Pos pos, bool notifyScene)
{
	Pos prevPos = _pos;

	_pos = pos;

	if (notifyScene)
	{
		// Scene에 알려준다.
		Game::GetScene()->UpdateGrid(this, prevPos, _pos);
	}
}

Sprite* Actor::CreateSpriteComponent(wstring bitmapKey, int32 indexX, int32 indexY, int32 width, int32 height)
{
	Sprite* sprite = new Sprite(bitmapKey, width, height, indexX, indexY);
	_components.emplace_back(sprite);
	return sprite;
}

Texture* Actor::CreateTextureComponent(wstring bitmapKey, int32 width, int32 height)
{
	Texture* texture = new Texture(bitmapKey, width, height);
	_components.emplace_back(texture);
	return texture;
}

ColliderCircle* Actor::CreateColliderCircleComponent(Size size, bool checkCell)
{
	int32 radius = (int32)(size.w * 0.8f); // 반지름

	ColliderCircle* collider = new ColliderCircle(checkCell);
	collider->Init(this, size, GetPos(), radius);
	_components.emplace_back(collider);
	return collider;
}

/// <summary>
/// 
/// </summary>
AnimSpriteActor::AnimSpriteActor(Pos pos, wstring bitmapKey, float frameTime)
	: Super(pos), _durtaion(frameTime)
{
	_sprite = CreateSpriteComponent(bitmapKey, 0, 0);
	if (_sprite)
	{
		// 텍스쳐의 크기를 가져온다
		Size size = _sprite->GetSize();
		AddPosDelta(-size.w / 2.0f, size.h / 2.0f);
	}
}

AnimSpriteActor::~AnimSpriteActor()
{
}

void AnimSpriteActor::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (_durtaion == 0)
		return;

	if (_isEnd)
		return;

	if (_sprite == nullptr)
		return;

	const HBitmapInfo* bitmapInfo = _sprite->GetBitmapInfo();
	if (bitmapInfo == nullptr)
		return;

	_sumTime += deltaTime;

	int32 totalCount = bitmapInfo->countX * bitmapInfo->countY;
	float delta = _durtaion / totalCount;

	// 일정 시간이 지나면 다음 프레임 이동
	if (_sumTime >= delta && _isEnd == false)
	{
		_animIndexX = ((_animIndexX + 1) % bitmapInfo->countX);

		// x 개수가 한바퀴 돌아서 끝까지 도착
		if (bitmapInfo->loop == false)
		{
			if (_animIndexX == bitmapInfo->countX - 1)
			{
				// 루프가 아닌 애니메이션은 마지막 프레임에 멈춰있게 한다.
				_isEnd = true;
			}
		}

		_sumTime -= delta;
	}

	_sprite->SetIndex(_animIndexX, _animIndexY);
}
