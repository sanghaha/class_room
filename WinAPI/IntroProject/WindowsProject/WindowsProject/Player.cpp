#include "pch.h"
#include "Player.h"
#include "InputManager.h"

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
}

void Player::Render(HDC hdc)
{
	Ellipse(hdc, _playerPos.x, _playerPos.y, _playerPos.x + 100, _playerPos.y + 100);
}
