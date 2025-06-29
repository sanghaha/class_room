#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "LineMesh.h"
#include "Game.h"
#include "GameScene.h"
#include "Enemy.h"

void Player::Init()
{
	//_lineMesh = ResourceManager::GetInstance()->GetLineMesh(L"Player");
	{
		fs::path fullPath = fs::current_path() / "../Resources/Player.bmp";

		HDC hdc = ::GetDC(Game::GetInstance()->GetHwnd());

		_textureHdc = ::CreateCompatibleDC(hdc);
		_bitmap = (HBITMAP)::LoadImageW(
			nullptr,
			fullPath.c_str(),
			IMAGE_BITMAP,
			0,
			0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION
		);
		if (_bitmap == 0)
		{
			::MessageBox(Game::GetInstance()->GetHwnd(), fullPath.c_str(), L"Invalid Texture Load", MB_OK);
			return;
		}

		_transparent = RGB(252, 0, 255);
	
		HBITMAP prev = (HBITMAP)::SelectObject(_textureHdc, _bitmap);
		::DeleteObject(prev);

		BITMAP bit = {};
		::GetObject(_bitmap, sizeof(BITMAP), &bit);

		_sizeX = bit.bmWidth;
		_sizeY = bit.bmHeight;
	}



	_angle = DegreeToRadian(90);
	_FOV = DegreeToRadian(60);
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
			Enemy* enemy = gameScene->GetEnemy();

			Vector dirToTarget = (enemy->GetPos() - _playerPos);
			dirToTarget.Normalize();

			Vector playerDir = GetForward();
			float dot = dirToTarget.Dot(playerDir);

			// 내적 활용
			if (dot > cos(_FOV / 2.0f)) 
			{
				gameScene->CreateMissile(_playerPos, _angle, enemy);
			}
			else
			{
				gameScene->CreateMissile(_playerPos, _angle, nullptr);
			}
		}
	}
}

void Player::Render(HDC hdc)
{

	if (_transparent == -1)
	{
		::BitBlt(hdc,	// 백버퍼에
			(int32)_playerPos.x - (_sizeX / 2),
			(int32)_playerPos.y - (_sizeY / 2),
			_sizeX,
			_sizeY,
			_textureHdc,	// 텍스쳐 그리기
			0,
			0,
			SRCCOPY);
	}
	else
	{
		::TransparentBlt(hdc,
			(int32)_playerPos.x - (_sizeX / 2),
			(int32)_playerPos.y - (_sizeY / 2),
			_sizeX,
			_sizeY,
			_textureHdc,
			0,
			0,
			_sizeX,
			_sizeY,
			_transparent);
	}


	//if (_lineMesh)
	//	_lineMesh->Render(hdc, _playerPos, 0.5f, 0.5f);

	{
		Vector firePos = GetFirePos();
		::MoveToEx(hdc, _playerPos.x, _playerPos.y, nullptr);
		::LineTo(hdc, firePos.x, firePos.y);

		Vector dir = GetForward();

		Vector mousePos(InputManager::GetInstance()->GetMousePos().x, InputManager::GetInstance()->GetMousePos().y);
		DWORD color;
		Vector dirToTarget = (mousePos - _playerPos);
		dirToTarget.Normalize();

		float cross = dir.Cross(dirToTarget);
		if (cross > 0)
		{
			color = RGB(0, 255, 0);
		}
		else
		{
			color = RGB(0, 0, 255);
		}

		wstring str = std::format(L"cross({0})", RadianToDegree(cross));
		::TextOut(hdc, 400, 400, str.c_str(), static_cast<int32>(str.size()));


		HPEN pen = ::CreatePen(PS_SOLID, 1, color);
		HPEN oldPen = (HPEN)::SelectObject(hdc, pen);

		Vector leftPos = _playerPos + (dir.Rotate(-_FOV / 2) * 100);
		::MoveToEx(hdc, _playerPos.x, _playerPos.y, nullptr);
		::LineTo(hdc, leftPos.x, leftPos.y);
		
		Vector rightPos = _playerPos + (dir.Rotate(_FOV / 2) * 100);
		::MoveToEx(hdc, _playerPos.x, _playerPos.y, nullptr);
		::LineTo(hdc, rightPos.x, rightPos.y);

		::SelectObject(hdc, oldPen);
		::DeleteObject(pen);
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

Vector Player::GetForward()
{
	Vector dir = GetFirePos() - _playerPos;
	dir.Normalize();
	return dir;
}
