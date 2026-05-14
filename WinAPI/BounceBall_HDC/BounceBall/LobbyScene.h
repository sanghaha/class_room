#pragma once
#include "Scene.h"

class UIPanel;

class LobbyScene : public Scene
{
	using Super = Scene;
public:
	LobbyScene();
	virtual ~LobbyScene();

	virtual void Init() override;
	virtual void Destory() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC renderTarget) override;

private:
	void onClickPlayButton();
	void onClickMapEditorButton();

private:
	UIPanel* _mainPanel = nullptr;
};
