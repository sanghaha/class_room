#pragma once
#include "Scene.h"

class UIImage;
class UIButton;

class LobbyScene : public Scene
{
	using Super = Scene;
public:
	LobbyScene();
	virtual ~LobbyScene();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC renderTarget) override;

private:
	void onClickPlayButton();
	void onClickMapEditorButton();

private:
	UIImage* _bg = nullptr;
	UIImage* _logo = nullptr;
	UIButton* _playButton = nullptr;
	UIButton* _mapEditorButton = nullptr;

};

