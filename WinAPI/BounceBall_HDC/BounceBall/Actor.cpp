#include "pch.h"
#include "Actor.h"
#include "Scene.h"
#include "Game.h"
#include "GameScene.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Texture.h"

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
}

void Actor::Update(float deltaTime)
{
	for (auto iter : _components)
	{
		iter->Update(deltaTime);
	}
}

void Actor::Render(HDC renderTarget)
{
	for (auto iter : _components)
	{
		iter->Render(renderTarget, GetPos());
	}
}

void Actor::Destroy()
{
	for (auto iter : _components)
	{
		SAFE_DELETE(iter);
	}
	_components.clear();

	// ���������
	Game::GetScene()->ReserveRemove(this);
}

void Actor::SetPos(Vector pos, bool notifyScene)
{
	Vector prevPos = _pos;
	_pos = pos;

	if (notifyScene)
	{
		// Scene�� �˷��ش�.
		Game::GetScene()->UpdateGrid(this, prevPos, pos);
	}
}

void Actor::SaveActor(std::wofstream& file)
{
}

void Actor::LoadActor(std::wistringstream& steam)
{
}

Sprite* Actor::CreateSpriteComponent(string spriteInfo, int32 width, int32 height)
{
	const SpriteInfo* info = ResourceManager::GetInstance()->GetSpriteInfo(spriteInfo);
	if (nullptr == info)
		return nullptr;

	Sprite* sprite = new Sprite(info->bitmapKey, width, height, info->alignCenter);
	_components.emplace_back(sprite);
	return sprite;
}

Texture* Actor::CreateTextureComponent(wstring bitmapKey, int32 width, int32 height)
{
	Texture* sprite = new Texture(bitmapKey, width, height);
	_components.emplace_back(sprite);
	return sprite;
}


//--------------------------------------------------------------------------
SpriteActor::SpriteActor(Vector pos) : Super(pos)
{
}

SpriteActor::~SpriteActor()
{
	_sprite = nullptr;
}

void SpriteActor::Init()
{
	Super::Init();
}

void SpriteActor::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void SpriteActor::Render(HDC renderTarget)
{
	Super::Render(renderTarget);

	bool debug = false;
	if (debug)
	{
		MyRect* rect = GetCollisionRect();
	}
}

void SpriteActor::SetSpriteIndex(int32 x, int32 y)
{
	if (_sprite)
	{
		_sprite->SetIndex(x, y);
	}
}

void SpriteActor::SaveActor(std::wofstream& file)
{
	int32 indexX = 0, indexY = 0;
	if (_sprite)
	{
		_sprite->GetIndex(indexX, indexY);
	}

	Vector pos = GetPos();
	char comma = ',';
	file << (int32)GetActorType() << comma;
	file << indexX << comma << indexY << comma;
	file << (int32)pos.x << comma << (int32)pos.y << comma;
	file << std::endl;
}

void SpriteActor::LoadActor(std::wistringstream& steam)
{
	wchar_t comma = ',';
	steam >> comma;

	int32 indexX = 0, indexY = 0;
	steam >> indexX >> comma >> indexY >> comma;

	int32 x = 0, y = 0;
	steam >> x >> comma >> y >> comma;

	SetPos(Vector((float)x, (float)y), true);
	SetSpriteIndex(indexX, indexY);
}

//--------------------------------------------------------------------------
AnimSpriteActor::AnimSpriteActor(Vector pos) : Super(pos)
{
}

AnimSpriteActor::~AnimSpriteActor()
{
}

void AnimSpriteActor::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (_isEnd)
		return;

	if (_sprite == nullptr)
		return;

	_sumTime += deltaTime;

	int32 frameCountX = 0;
	int32 frameCountY = 0;
	_sprite->GetFrameCount(frameCountX, frameCountY);

	int32 totalCount = frameCountX * frameCountY;
	float delta = _durtaion / totalCount;

	// ���� �ð��� ������ ���� ������ �̵�
	if (_sumTime >= delta && _isEnd == false)
	{
		_animIndexX = ((_animIndexX + 1) % frameCountX);

		// x ������ �ѹ��� ���Ƽ� ������ ����
		if (_loop == false)
		{
			if (_animIndexX == frameCountX - 1)
			{
				// ������ �ƴ� �ִϸ��̼��� ������ �����ӿ� �����ְ� �Ѵ�.
				_isEnd = true;
			}
		}

		_sumTime -= delta;
	}

	SetSpriteIndex(_animIndexX, _animIndexY);
}
