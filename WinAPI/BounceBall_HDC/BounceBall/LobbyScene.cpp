#include "pch.h"
#include "LobbyScene.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "Texture.h"
#include "UIButton.h"
#include "Game.h"

LobbyScene::LobbyScene()
{
}

LobbyScene::~LobbyScene()
{
}

void LobbyScene::Init()
{
	Super::Init();

	ResourceManager::GetInstance()->LoadDXBitmap(L"LobbyBG", L"background_title.bmp");
	ResourceManager::GetInstance()->LoadDXBitmap(L"Btn_MapEditor", L"mapEditor.bmp");
	ResourceManager::GetInstance()->LoadDXBitmap(L"Btn_Play", L"play.png");
	ResourceManager::GetInstance()->LoadDXBitmap(L"Logo", L"title.png");

	_bg = _ui.CreateImage(Vector(0, 0), L"LobbyBG");
	_logo = _ui.CreateImage(Vector(20, 20), L"Logo");
	
	_playButton = _ui.CreateButton(Vector(10, 300), L"Btn_Play");
	_playButton->SetClickEvent([this]() { onClickPlayButton();});

	_mapEditorButton = _ui.CreateButton(Vector(10, 500), L"Btn_MapEditor");
	_mapEditorButton->SetClickEvent([this]() { onClickMapEditorButton();});
}

void LobbyScene::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void LobbyScene::Render(HDC renderTarget)
{
	Super::Render(renderTarget);
}

void LobbyScene::onClickPlayButton()
{
	Game::GetInstance()->ChangeGameScene();
}

void LobbyScene::onClickMapEditorButton()
{
	Game::GetInstance()->ChangeEditorScene();
}
