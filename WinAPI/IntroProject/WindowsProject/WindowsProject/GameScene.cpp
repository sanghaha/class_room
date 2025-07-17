#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "InputManager.h"

void GameScene::Init()
{
	_player = new Player();
	_player->Init();

	_enemy = new Enemy();
	_enemy->Init();
}

void GameScene::Update(float deltaTime)
{
	_player->Update(deltaTime);
	_enemy->Update(deltaTime);

	for (auto iter : _missile)
	{
		iter->Update(deltaTime);
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		mouseStart = InputManager::GetInstance()->GetMousePos();
	}
}

bool LineIntersectsAABB(HDC hdc, Vector p0, Vector p1, const RECT& rect, Vector& outNormal, Vector& outPos)
{
	Vector dir = p1 - p0;

	float tMin = 0.0f;
	float tMax = 1.0f;
	Vector normal;

	// x�� �˻�
	if (dir.x != 0)
	{
		float tx1 = (rect.left - p0.x) / dir.x;
		float tx2 = (rect.right - p0.x) / dir.x;

		{
			wstring str = std::format(L"tx1({0}), tx2({1})", tx1, tx2);
			::TextOut(hdc, 400, 200, str.c_str(), static_cast<int32>(str.size()));
		}

		// X �� �������� ���� ���� �������� �Ǵ�
		Vector nx = (tx1 < tx2) ? Vector(-1, 0) : Vector(1, 0);
		if (tx1 > tx2) std::swap(tx1, tx2);

		// ���� �ʰ� �����ߴٴ� ��
		if (tx1 > tMin)
		{
			normal = nx;
		}

		tMin = max(tMin, tx1);
		tMax = min(tMax, tx2);

		{
			wstring str = std::format(L"X tmin({0}), tmax({1})", tMin, tMax);
			::TextOut(hdc, 400, 220, str.c_str(), static_cast<int32>(str.size()));
		}

		if (tMin > tMax)
			return false; // �浹 �� ��
	}
	else if (p0.x < rect.left || p0.x > rect.right)
	{
		return false; // Y������ �����ϰ� �ۿ� ����
	}

	// y�� �˻�
	if (dir.y != 0)
	{
		float ty1 = (rect.top - p0.y) / dir.y;
		float ty2 = (rect.bottom - p0.y) / dir.y;

		wstring str = std::format(L"ty1({0}), ty2({1})", ty1, ty2);
		::TextOut(hdc, 400, 240, str.c_str(), static_cast<int32>(str.size()));

		// ���� �������κ��� �������ִ� �Ÿ� ����
		// ty1 �� ty2���� �۴ٸ�, ������ �Ʒ� �����̴�.
		Vector ny = (ty1 < ty2) ? Vector(0, -1) : Vector(0, 1);

		if (ty1 > ty2) std::swap(ty1, ty2);
		
		// ���� �ʰ� �����ߴٴ� ��
		if (ty1 > tMin)
		{
			normal = ny;
		}

		tMin = max(tMin, ty1);
		tMax = min(tMax, ty2);

		{
			wstring str = std::format(L"Y tmin({0}), tmax({1})", tMin, tMax);
			::TextOut(hdc, 400, 260, str.c_str(), static_cast<int32>(str.size()));
		}

		if (tMin > tMax)
			return false; // �浹 �� ��
	}
	else if (p0.y < rect.top || p0.y > rect.bottom)
	{
		return false;
	}

	// ������ �簢�� ���ο� �ִ� ��� ����ó��
	// ������ ������⿡ ���� �� �������� ���ݴ� ������ �������� normal �� �������ش�.
	if (normal == Vector(0, 0))
	{
		if (fabs(dir.x) > fabs(dir.y))
		{
			normal = (dir.x > 0) ? Vector(-1, 0) : Vector(1, 0);
		}
		else
		{
			normal = (dir.y > 0) ? Vector(0, -1) : Vector(0, 1);
		}
	}

	//if (tMin > tMax || tMin > 1.0f || tMin < 0.0f)
	//    return false;

	// �浹 ��� ����
	outPos = p0 + dir * tMin;
	outNormal = normal;

	return true;
}


void GameScene::Render(HDC hdc)
{
	_player->Render(hdc);
	_enemy->Render(hdc);

	for (auto iter : _missile)
	{
		iter->Render(hdc);
	}

	RECT rect;
	rect.left = 200;
	rect.top = 200;
	rect.right = 300;
	rect.bottom = 300;

	::Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	if (mouseStart.x != 0 && mouseStart.y != 0)
	{
		Vector mouseEnd = InputManager::GetInstance()->GetMousePos();
		::MoveToEx(hdc, mouseStart.x, mouseStart.y, nullptr);
		::LineTo(hdc, mouseEnd.x, mouseEnd.y);

		Vector outPos;
		Vector outNormal;
		if (LineIntersectsAABB(hdc, mouseStart, mouseEnd, rect, outPos, outNormal))
		{
			wstring str = std::format(L"Hit({0})", true);
			::TextOut(hdc, 400, 300, str.c_str(), static_cast<int32>(str.size()));
		}
	}
}

void GameScene::CreateMissile(Vector pos, float angle, class Enemy* target)
{
	Missile* missile = new Missile();
	missile->Init(pos, angle, target);
	_missile.push_back(missile);
}

void GameScene::RemoveMissile(Missile* missile)
{
	auto it = std::find(_missile.begin(), _missile.end(), missile);
	_missile.erase(it);

	delete missile;
}
