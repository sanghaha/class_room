#include "pch.h"
#include "LobbyScene.h"
#include "ResourceManager.h"
#include "UIPanel.h"
#include "Texture.h"
#include "UIButton.h"
#include "UIImage.h"
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

	ResourceManager::GetInstance()->LoadTexture(L"LobbyBG", L"background_title.bmp");
	ResourceManager::GetInstance()->LoadTexture(L"Btn_MapEditor", L"mapEditor.bmp");
	ResourceManager::GetInstance()->LoadTexture(L"Btn_Play", L"play.bmp", 1, 1, RGB(255,0,255));
	ResourceManager::GetInstance()->LoadTexture(L"Logo", L"title.bmp", 1, 1, RGB(255, 0, 255));

	_mainPanel = new UIPanel(Vector(0, 0));
	_mainPanel->Init();

	_mainPanel->CreateImage(Vector(0, 0), L"LobbyBG");
	_mainPanel->CreateImage(Vector(20, 20), L"Logo");
	
	UIButton* playBtn = _mainPanel->CreateButton(Vector(10, 300), L"Btn_Play");
	playBtn->SetClickEvent([this]() { onClickPlayButton();});

	UIButton* editBtn = _mainPanel->CreateButton(Vector(10, 500), L"Btn_MapEditor");
	editBtn->SetClickEvent([this]() { onClickMapEditorButton();});
}

void LobbyScene::Destory()
{
	Super::Destory();

	SAFE_DELETE(_mainPanel);
}

void LobbyScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (_mainPanel)
		_mainPanel->Update();
}

void LobbyScene::Render(HDC renderTarget)
{
	Super::Render(renderTarget);

	if (_mainPanel)
		_mainPanel->Render(renderTarget);
}

void LobbyScene::onClickPlayButton()
{
	Game::GetInstance()->ChangeGameScene();
}

void LobbyScene::onClickMapEditorButton()
{
	Game::GetInstance()->ChangeEditorScene();
}
