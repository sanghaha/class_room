#include "pch.h"
#include "Actor.h"
#include "Scene.h"
#include "Game.h"
#include "GameScene.h"
#include "ResourceManager.h"
#include "Component.h"
#include "SpriteRenderer.h"
#include "ImageRenderer.h"
#include "RectCollider.h"

Actor::Actor(Vector pos)
{
	_pos = pos;
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
	Game::GetScene()->UpdateGrid(this, Vector(0, 0), _pos);

	for (auto iter : _components)
	{
		iter->InitComponent();
	}
}

void Actor::Update(float deltaTime)
{
	for (auto iter : _components)
	{
		iter->UpdateComponent(deltaTime);
	}
}

void Actor::Render(HDC renderTarget)
{
	for (auto iter : _components)
	{
		iter->RenderComponent(renderTarget, GetPos());
	}
}

void Actor::Destroy()
{
	for (auto iter : _components)
	{
		SAFE_DELETE(iter);
	}
	_components.clear();

	// 사라져야함
	Game::GetScene()->ReserveRemove(this);
}

void Actor::SetPos(Vector pos, bool notifyScene)
{
	Vector prevPos = _pos;
	_pos = pos;

	if (notifyScene)
	{
		// Scene에 알려준다.
		Game::GetScene()->UpdateGrid(this, prevPos, pos);
	}
}

void Actor::SaveActor(std::wofstream& file)
{
	SpriteRenderer* sprite = GetComponent<SpriteRenderer>();
	if (nullptr == sprite)
		return;

	int32 indexX = 0, indexY = 0;
	sprite->GetIndex(indexX, indexY);

	Vector pos = GetPos();
	char comma = ',';
	file << (int32)GetActorType() << comma;
	file << indexX << comma << indexY << comma;
	file << (int32)pos.x << comma << (int32)pos.y << comma;
	file << std::endl;
}

void Actor::LoadActor(std::wistringstream& steam)
{
	SpriteRenderer* sprite = GetComponent<SpriteRenderer>();
	if (nullptr == sprite)
		return;

	wchar_t comma = ',';
	steam >> comma;

	int32 indexX = 0, indexY = 0;
	steam >> indexX >> comma >> indexY >> comma;

	int32 x = 0, y = 0;
	steam >> x >> comma >> y >> comma;

	SetPos(Vector((float)x, (float)y), true);
	sprite->SetSpriteIndex(indexX, indexY);
}

RectCollider* Actor::GetCollisionRect()
{
	for (auto iter : _components)
	{
		if (RectCollider* collider = dynamic_cast<RectCollider*>(iter))
		{
			return collider;
		}
	}

	return nullptr;
}

SpriteRenderer* Actor::CreateSpriteComponent(string spriteInfo, int32 width, int32 height)
{
	const SpriteInfo* info = ResourceManager::GetInstance()->GetSpriteInfo(spriteInfo);
	if (nullptr == info)
		return nullptr;

	SpriteRenderer* sprite = new SpriteRenderer(info->textureKey, width, height);
	sprite->SetSpriteIndex(info->startX, info->endY);
	sprite->SetSpriteDuration((float)info->dur);
	sprite->SetLoop(info->loop);

	_components.emplace_back(sprite);
	return sprite;
}

ImageRenderer* Actor::CreateTextureComponent(wstring bitmapKey, int32 width, int32 height)
{
	ImageRenderer* sprite = new ImageRenderer(bitmapKey, width, height);
	_components.emplace_back(sprite);
	return sprite;
}

RectCollider* Actor::CreateRectCollider(float width, float height)
{
	RectCollider* collider = new RectCollider(this, width, height);
	_components.emplace_back(collider);
	return collider;
}

//
////--------------------------------------------------------------------------
//SpriteActor::SpriteActor(Vector pos) : Super(pos)
//{
//}
//
//SpriteActor::~SpriteActor()
//{
//	_sprite = nullptr;
//}
//
//void SpriteActor::Init()
//{
//	Super::Init();
//}
//
//void SpriteActor::Update(float deltaTime)
//{
//	Super::Update(deltaTime);
//}
//
//void SpriteActor::Render(HDC renderTarget)
//{
//	Super::Render(renderTarget);
//
//	bool debug = false;
//	if (debug)
//	{
//		MyRect* rect = GetCollisionRect();
//	}
//}
//
//void SpriteActor::SetSpriteIndex(int32 x, int32 y)
//{
//	if (_sprite)
//	{
//		_sprite->SetIndex(x, y);
//	}
//}
//
//void SpriteActor::SaveActor(std::wofstream& file)
//{
//	int32 indexX = 0, indexY = 0;
//	if (_sprite)
//	{
//		_sprite->GetIndex(indexX, indexY);
//	}
//
//	Vector pos = GetPos();
//	char comma = ',';
//	file << (int32)GetActorType() << comma;
//	file << indexX << comma << indexY << comma;
//	file << (int32)pos.x << comma << (int32)pos.y << comma;
//	file << std::endl;
//}
//
//void SpriteActor::LoadActor(std::wistringstream& steam)
//{
//	wchar_t comma = ',';
//	steam >> comma;
//
//	int32 indexX = 0, indexY = 0;
//	steam >> indexX >> comma >> indexY >> comma;
//
//	int32 x = 0, y = 0;
//	steam >> x >> comma >> y >> comma;
//
//	SetPos(Vector((float)x, (float)y), true);
//	SetSpriteIndex(indexX, indexY);
//}
//
////--------------------------------------------------------------------------
//AnimSpriteActor::AnimSpriteActor(Vector pos) : Super(pos)
//{
//}
//
//AnimSpriteActor::~AnimSpriteActor()
//{
//}
//
//void AnimSpriteActor::Update(float deltaTime)
//{
//	Super::Update(deltaTime);
//
//	if (_isEnd)
//		return;
//
//	if (_sprite == nullptr)
//		return;
//
//	_sumTime += deltaTime;
//
//	int32 frameCountX = 0;
//	int32 frameCountY = 0;
//	_sprite->GetFrameCount(frameCountX, frameCountY);
//
//	int32 totalCount = frameCountX * frameCountY;
//	float delta = _durtaion / totalCount;
//
//	// 일정 시간이 지나면 다음 프레임 이동
//	if (_sumTime >= delta && _isEnd == false)
//	{
//		_animIndexX = ((_animIndexX + 1) % frameCountX);
//
//		// x 개수가 한바퀴 돌아서 끝까지 도착
//		if (_loop == false)
//		{
//			if (_animIndexX == frameCountX - 1)
//			{
//				// 루프가 아닌 애니메이션은 마지막 프레임에 멈춰있게 한다.
//				_isEnd = true;
//			}
//		}
//
//		_sumTime -= delta;
//	}
//
//	SetSpriteIndex(_animIndexX, _animIndexY);
//}
