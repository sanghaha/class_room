#include "pch.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "GameScene.h"
#include "Player.h"
#include "Texture.h"
#include "Game.h"

void UIManager::Init()
{
	Player* player = Game::GetGameScene()->GetPlayer();
	if (player)
	{
		//int32 hp = player->GetHp();
		_hpTexture = new Texture(L"PlayerHP", 0, 0);
	}
}

void UIManager::Render(HDC hdc)
{
	// score
	{
		HFONT hFont = CreateFont(
			-16,			   // ���� ���� (�ȼ� ������ ��ȯ). ������ ���� ������. 
			0,                 // ���� �ʺ� (0�̸� �ڵ� ����)
			0,                 // ���� ���� ����
			0,                 // ���� ���̽����� ����
			FW_BOLD,           // ���� ���� (FW_NORMAL, FW_BOLD ��)
			FALSE,             // ���Ÿ� ����
			FALSE,             // ���� ����
			FALSE,             // ��Ҽ� ����
			DEFAULT_CHARSET,   // ���� ����
			OUT_DEFAULT_PRECIS,// ��� ���е�
			CLIP_DEFAULT_PRECIS,// Ŭ���� ���е�
			DEFAULT_QUALITY,   // ��� ǰ��
			DEFAULT_PITCH | FF_SWISS, // �۲� ��ġ�� ����
			L"Arial"           // �۲� �̸�
		);

		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

		// ���� ���� ����
		SetTextColor(hdc, RGB(255, 255, 255)); // ���
		SetBkMode(hdc, TRANSPARENT);           // ��� ����

		// �ؽ�Ʈ ���
		wstring str = std::format(L"Score : {0}", Game::GetGameScene()->GetScore());

		// �ؽ�Ʈ ũ�� ���
		SIZE textSize;
		GetTextExtentPoint32(hdc, str.c_str(), static_cast<int32>(str.size()), &textSize);

		// ȭ�� �߾� ��ǥ ���
		int x = (GWinSizeX - textSize.cx) / 2;
		int y = 10;

		TextOut(hdc, x, y, str.c_str(), static_cast<int32>(str.size()));

		// ���� ��Ʈ ���� �� �� ��Ʈ ����
		SelectObject(hdc, oldFont);
		DeleteObject(hFont);
	}

	Player* player = Game::GetGameScene()->GetPlayer();
	if (player)
	{
		int32 hp = player->GetHp();

		// HP �ؽ��� �׸���
		for (int32 i = 0; i < hp; ++i)
		{
			if (_hpTexture == nullptr)
				continue;
			_hpTexture->Render(hdc, Pos{ (float)(i * (_hpTexture->GetSize().w + 5)) + 10, (float)GWinSizeY - _hpTexture->GetSize().h - 30});
		}
	}
	else
	{
		// ���� ��Ʈ�� �����ϰ� �� ��Ʈ�� ����
		// ���ϴ� ��Ʈ ����
		{
			HFONT hFont = CreateFont(
				-60,			   // ���� ���� (�ȼ� ������ ��ȯ). ������ ���� ������. 
				0,                 // ���� �ʺ� (0�̸� �ڵ� ����)
				0,                 // ���� ���� ����
				0,                 // ���� ���̽����� ����
				FW_BOLD,           // ���� ���� (FW_NORMAL, FW_BOLD ��)
				FALSE,             // ���Ÿ� ����
				FALSE,             // ���� ����
				FALSE,             // ��Ҽ� ����
				DEFAULT_CHARSET,   // ���� ����
				OUT_DEFAULT_PRECIS,// ��� ���е�
				CLIP_DEFAULT_PRECIS,// Ŭ���� ���е�
				DEFAULT_QUALITY,   // ��� ǰ��
				DEFAULT_PITCH | FF_SWISS, // �۲� ��ġ�� ����
				L"Arial"           // �۲� �̸�
			);

			HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

			// ���� ���� ����
			SetTextColor(hdc, RGB(255, 255, 255)); // ���
			SetBkMode(hdc, TRANSPARENT);           // ��� ����

			// �ؽ�Ʈ ���
			wstring str = std::format(L"GAME OVER!");

			// �ؽ�Ʈ ũ�� ���
			SIZE textSize;
			GetTextExtentPoint32(hdc, str.c_str(), static_cast<int32>(str.size()), &textSize);

			// ȭ�� �߾� ��ǥ ���
			int x = (GWinSizeX - textSize.cx) / 2;
			int y = (GWinSizeY - textSize.cy) / 2;

			TextOut(hdc, x, y, str.c_str(), static_cast<int32>(str.size()));

			// ���� ��Ʈ ���� �� �� ��Ʈ ����
			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
		}
	}
}

void UIManager::Destroy()
{
	SAFE_DELETE(_hpTexture);
}
