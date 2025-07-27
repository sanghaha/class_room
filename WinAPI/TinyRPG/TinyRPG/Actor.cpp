#include "pch.h"
#include "Actor.h"
#include "Scene.h"
#include "Game.h"
#include "GameScene.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Texture.h"

Actor::Actor(Vector pos, bool convertCellPos)
{
	_posCell = Cell::ConvertToCell(pos);
	if (convertCellPos)
	{
		_pos = _posCell.ConvertToPos();
	}
	else
	{
		_pos = pos;
	}
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
	Game::GetScene()->UpdateGrid(this, Cell{ 0,0 }, _posCell);
}

void Actor::Update(float deltaTime)
{
	for (auto iter : _components)
	{
		iter->Update(deltaTime);
	}
}

void Actor::Render(ID2D1RenderTarget* renderTarget)
{
	if (IsCulling())
		return;

	for (auto iter : _components)
	{
		iter->Render(renderTarget, GetPos());
	}
}

void Actor::Destory()
{
	// ���������
	Game::GetGameScene()->ReserveRemove(this);
}

bool Actor::AddPosDelta(float deltaTime)
{
	Vector destPos = _posCell.ConvertToPos();
	Vector dir = destPos - _pos;
	dir.Normalize();

	Vector delta = dir * _moveSpeed * deltaTime;

	Vector prevPos = _pos;
	Vector nextPos = _pos + delta;

	Vector toPrev = destPos - prevPos;
	Vector toCurrent = destPos - nextPos;

	float dot = toPrev.Dot(toCurrent);
	//PrintLog(std::format(L"dest:{0},{1}, prev:{2},{3}, next:{4},{5}, dot:{6}, index:{7},{8}",
	//	(int32)destPos.x, (int32)destPos.y,
	//	(int32)prevPos.x, (int32)prevPos.y,
	//	(int32)nextPos.x, (int32)nextPos.y,
	//	dot,
	//	_posCell.index_X, _posCell.index_Y));

	_moveDeltaDistance -= delta.Length();

	if (dot <= 0)	// ���⿡ ���� �����ص��ǰ�
	//if(_moveDeltaDistance <= 0)	//1) ���� ������� üũ�ص� �ǰ�.
	{
		_moveDeltaDistance = 0;
		_pos = destPos;
		return false;
	}
	else
	{
		_pos = nextPos;
		return true;
	}
}

void Actor::SetPos(Vector pos, bool notifyScene)
{
	Cell prevCell = _posCell;

	_pos = pos;
	_posCell = Cell::ConvertToCell(pos);
	_moveDeltaDistance = 0;

	if (notifyScene)
	{
		// Scene�� �˷��ش�.
		Game::GetScene()->UpdateGrid(this, prevCell, _posCell);
	}
}

void Actor::SetPosCell(Cell cell, bool notifyScene)
{
	Cell prevCell = _posCell;
	_posCell = cell;
	_moveDeltaDistance = prevCell.DeltaLength(cell) * GTileSize;

	if (notifyScene)
	{
		// Scene�� �˷��ش�.
		Game::GetScene()->UpdateGrid(this, prevCell, _posCell);
	}
}

bool Actor::IsCulling()
{
	if (GetRenderLayer() == RenderLayer::RL_UI)
		return false;

	return Game::GetScene()->IsCulling(_pos);
}


Sprite* Actor::CreateSpriteComponent(string spriteName, int32 width, int32 height)
{
	const SpriteInfo* info = ResourceManager::GetInstance()->GetSpriteInfo(spriteName);
	if (nullptr == info)
		return nullptr;

	Sprite* sprite = new Sprite(info->bitmapKey, width, height, info->alignCenter);
	sprite->SetIndex(info->indexX, info->indexY);
	sprite->SetApplyCamera(GetRenderLayer() != RenderLayer::RL_UI);

	_components.emplace_back(sprite);
	return sprite;
}

Texture* Actor::CreateTextureComponent(string bitmapKey, int32 width, int32 height)
{
	Texture* sprite = new Texture(bitmapKey, width, height);
	_components.emplace_back(sprite);
	return sprite;
}


//-------------------------------------------------------------------
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

void SpriteActor::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
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

	int32 countX = (_anim.countX);
	int32 countY = (_anim.countY);

	int32 totalCount = countX * countY;
	float delta = _anim.durtaion / totalCount;

	// ���� �ð��� ������ ���� ������ �̵�
	if (_sumTime >= delta && _isEnd == false)
	{
		_animIndexX = ((_animIndexX + 1 - _anim.startX) % countX) + _anim.startX;

		if (_anim.startX == _animIndexX)
		{
			_animIndexY = ((_animIndexY + 1 - _anim.startY) % countY) + _anim.startY;
		}
 
		// x ������ �ѹ��� ���Ƽ� ������ ����
		if (_anim.loop == false)
		{
			if (_animIndexX == _anim.startX && _animIndexY == _anim.startY)
			{
				// ������ �ƴ� �ִϸ��̼��� ������ �����ӿ� �����ְ� �Ѵ�.
				_animIndexX = _anim.startX + countX - 1;
				_animIndexY = _anim.startY + countY - 1;
				_isEnd = true;
			}
		}

		_sumTime -= delta;
	}

	if (_sprite)
	{
		_sprite->SetIndex(_animIndexX, _animIndexY);
		if (_anim.flipX != _sprite->GetFlip())
		{
			_sprite->SetFlip(_anim.flipX);
		}
	}
}

void AnimSpriteActor::SetAnimInfo(const AnimInfo& info)
{
	_anim = info;
}