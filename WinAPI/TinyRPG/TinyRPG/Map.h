#pragma once
#include "Actor.h"

class Sprite;
class Texture;

class Map : public Actor
{
	using Super = Actor;
public:
	Map(Vector pos);
	virtual ~Map();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Background; }
	bool IsBlockingCell() override { return false; }

	void LoadTileMap(int8 stage, wstring path);

	Size GetMapSize();
	int32 GetGridWidth() const { return _gridW; }
	int32 GetGridHeight() const { return _gridH; }

	void ConvertGroundTileIndex(int32 x, int32 y, int32& outTileX, int32& outTileY);

	void ResetSelectedIndex() { _selectedX = -1; _selectedY = -1; }
	void SetSelectedIndex(int32 x, int32 y) { _selectedX = x; _selectedY = y; }
	void GetSelectedIndex(int32& outX, int32& outY) { outX = _selectedX; outY = _selectedY; }
private:
	void drawTileOnGrid(ID2D1RenderTarget* renderTarget, int layer, int x, int y);

private:
	Sprite* _tile = nullptr;
	Texture* _selector = nullptr;

	int32 _gridW = 0;
	int32 _gridH = 0;
	int32 _tileCountX = 0;
	int32 _tileCountY = 0;

	const static int _layerCount = 4;	// 제일 마지막이 갈수 있는 영역을 그리는곳.
	struct TileLayer
	{
		std::vector<int32> mainGrid; // 메인 창 그리드 데이터
	};
	TileLayer _layer[_layerCount]; // 레이어 배열 정의

	int32 _selectedX = -1;
	int32 _selectedY = -1;
};

