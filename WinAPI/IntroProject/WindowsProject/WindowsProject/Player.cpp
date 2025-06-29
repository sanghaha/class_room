#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "LineMesh.h"
#include "Game.h"
#include "GameScene.h"

void Player::Init()
{
	_lineMesh = ResourceManager::GetInstance()->GetLineMesh(L"Player");
	_angle = DegreeToRadian(90);
}

void Player::Update(float deltaTime)
{
	if(InputManager::GetInstance()->GetButtonPressed(KeyType::A))
	{
		_playerPos.x -= _speed * deltaTime;
	}

	if(InputManager::GetInstance()->GetButtonPressed(KeyType::D))
	{
		_playerPos.x += _speed * deltaTime;
	}

	if(InputManager::GetInstance()->GetButtonPressed(KeyType::S))
	{
		_playerPos.y += _speed * deltaTime;
	}

	if(InputManager::GetInstance()->GetButtonPressed(KeyType::W))
	{
		_playerPos.y -= _speed * deltaTime;
	}

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::Q))
	{
		_angle += 10 * deltaTime;
	}

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::E))
	{
		_angle -= 10 * deltaTime;
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::SpaceBar))
	{
		GameScene* gameScene = dynamic_cast<GameScene*>(Game::GetInstance()->GetScene());
		if (gameScene)
		{
			gameScene->CreateMissile(_playerPos, _angle);
		}
	}
}

void Player::Render(HDC hdc)
{
	if (_lineMesh)
		_lineMesh->Render(hdc, _playerPos, 0.5f, 0.5f);

	{
		Vector firePos = GetFirePos();

		::MoveToEx(hdc, _playerPos.x, _playerPos.y, nullptr);
		::LineTo(hdc, firePos.x, firePos.y);
	}

	{
		wstring str = std::format(L"angle({0})", RadianToDegree(_angle));
		::TextOut(hdc, 300, 400, str.c_str(), static_cast<int32>(str.size()));
	}

	//Ellipse(hdc, _playerPos.x, _playerPos.y, _playerPos.x + 100, _playerPos.y + 100);
}

Vector Player::GetFirePos()
{
	Vector pos = _playerPos;
	pos.x += _length * ::cos(_angle);
	pos.y -= _length * ::sin(_angle);

	return pos;
}
