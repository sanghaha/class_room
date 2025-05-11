#pragma once
#include "Scene.h"

class SpriteActor;
class Ball;
class Block;
class Star;

struct EditActorInfo
{
	SpriteActor* tempActor;
	int32 spriteCount = 0;
	int32 spriteMax = 0;
	string spriteName;
};

class EditorScene : public Scene
{
	using Super = Scene;
public:
	EditorScene();
	virtual ~EditorScene();

	void Init() override;
	void Destory() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;

	void Save();
	void Load();

protected:
	void OnMouseWheel(int32 delta);

	void SetSpriteInfo(SpriteActor * actor, string spriteName, int32 count);
	EditActorInfo* GetCurrModeActor();

private:
	enum SELECT_MODE
	{
		SM_BLOCK,
		SM_STAR,
		SM_BALL,
		SM_COUNT
	};

	EditActorInfo _editActorInfo[SELECT_MODE::SM_COUNT];
	SELECT_MODE _currMode = SM_BLOCK;
};

