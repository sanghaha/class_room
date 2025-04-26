#pragma once
#include "Scene.h"

class GameScene : public Scene
{
	using Super = Scene;
public:
	GameScene();
	virtual ~GameScene();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	void CreateGrid() override;

	// ���� �ִ� Ÿ������ �Ǵ�
	bool CanMove(Cell cell);

protected:
	void loadResources() override;
	void createObjects() override;
	void initTimer() override;
private:
	class Player* _player = nullptr;
	class Map* _map = nullptr;

};

