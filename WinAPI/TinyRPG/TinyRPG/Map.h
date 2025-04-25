#pragma once
#include "Actor.h"

class Sprite;

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

	void LoadTileMap(wstring path);
	void SetSprite(Sprite* sprite);
	Size GetMapSize();

private:
	void drawTileOnGrid(HDC hdc, int layer, int x, int y);

private:
	Sprite* _sprite = nullptr;

	int32 _gridW = 0;
	int32 _gridH = 0;
	int32 _tileSize = 64;
	int32 _tileCountX = 0;
	int32 _tileCountY = 0;

	const static int _layerCount = 3;
	struct TileLayer
	{
		std::vector<int32> mainGrid; // 메인 창 그리드 데이터
	};
	TileLayer _layer[_layerCount]; // 레이어 배열 정의
};

