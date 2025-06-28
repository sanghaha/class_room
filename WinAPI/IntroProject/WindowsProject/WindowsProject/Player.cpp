#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "LineMesh.h"

void Player::Init()
{
	_lineMesh = ResourceManager::GetInstance()->GetLineMesh(L"Player");
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
}

void Player::Render(HDC hdc)
{
	if (_lineMesh)
		_lineMesh->Render(hdc, _playerPos, 0.5f, 0.5f);

	//Ellipse(hdc, _playerPos.x, _playerPos.y, _playerPos.x + 100, _playerPos.y + 100);
}
