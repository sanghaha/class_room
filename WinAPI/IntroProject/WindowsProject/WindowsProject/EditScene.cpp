#include "pch.h"
#include "EditScene.h"
#include "InputManager.h"
#include "Game.h"
#include <fstream>

void EditScene::Update(float deltaTime)
{
	if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();

		if (_drawing == false)
		{
			_startPos = mousePos;
			_drawing = true;
		}
	}

	if (InputManager::GetInstance()->GetButtonUp(KeyType::LeftMouse))
	{
		if (_drawing)
		{
			POINT mousePos = InputManager::GetInstance()->GetMousePos();
			_lines.push_back(make_pair(_startPos, mousePos));
			_drawing = false;
		}
	}

	// Save
	if (InputManager::GetInstance()->GetButtonDown(KeyType::S))
	{
		save_dialog();
	}

	// Load
	if (InputManager::GetInstance()->GetButtonDown(KeyType::L))
	{
		load_dialog();
	}
}

void EditScene::Render(HDC hdc)
{
	// 현재 그리고 있는 선
	if (_drawing)
	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();

		::MoveToEx(hdc, _startPos.x, _startPos.y, nullptr);
		::LineTo(hdc, mousePos.x, mousePos.y);
	}

	for (auto& line : _lines)
	{
		POINT pt1 = line.first;
		POINT pt2 = line.second;

		Vector pos1;
		pos1.x = (float)pt1.x;
		pos1.y = (float)pt1.y;

		Vector pos2;
		pos2.x = (float)pt2.x;
		pos2.y = (float)pt2.y;

		::MoveToEx(hdc, static_cast<int32>(pos1.x), static_cast<int32>(pos1.y), nullptr);
		::LineTo(hdc, static_cast<int32>(pos2.x), static_cast<int32>(pos2.y));
	}
}

void EditScene::save_basic(std::wstring fileName)
{
	wofstream file;
	file.open(fileName);

	int32 minX = INT32_MAX;
	int32 maxX = INT32_MIN;
	int32 minY = INT32_MAX;
	int32 maxY = INT32_MIN;

	for (auto& line : _lines)
	{
		POINT from = line.first;
		POINT to = line.second;

		minX = min(min(minX, from.x), to.x);
		maxX = max(max(maxX, from.x), to.x);
		minY = min(min(minY, from.y), to.y);
		maxY = max(max(maxY, from.y), to.y);
	}

	int32 midX = (maxX + minX) / 2;
	int32 midY = (maxY + minY) / 2;

	// 라인 개수
	file << static_cast<int32>(_lines.size()) << endl;

	for (auto& line : _lines)
	{
		POINT from = line.first;
		from.x -= midX;
		from.y -= midY;

		POINT to = line.second;
		to.x -= midX;
		to.y -= midY;

		wstring str = std::format(L"({0},{1})->({2},{3})", from.x, from.y, to.x, to.y);
		file << str << endl;
	}

	file.close();
}

void EditScene::load_basic(std::wstring fileName)
{
	wifstream file;
	file.open(fileName);

	// 라인 개수
	int32 count;
	file >> count;

	_lines.clear();

	int32 midX = 400;
	int32 midY = 300;

	for (int32 i = 0; i < count; i++)
	{
		POINT pt1;
		POINT pt2;

		wstring str;
		file >> str;
		::swscanf_s(str.c_str(), L"(%d,%d)->(%d,%d)", &pt1.x, &pt1.y, &pt2.x, &pt2.y);

		pt1.x += midX;
		pt1.y += midY;
		pt2.x += midX;
		pt2.y += midY;

		_lines.push_back(make_pair(pt1, pt2));
	}

	file.close();
}

void EditScene::save_dialog()
{
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Game::GetInstance()->GetHwnd();
	ofn.lpstrFilter = L"메시 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"stage";

	std::filesystem::path path = fs::current_path();
	ofn.lpstrInitialDir = path.c_str();

	if (GetSaveFileName(&ofn))
	{
		// 파일 이름이 선택되었으면 저장
		std::wstring fileName = szFileName;
		save_basic(fileName);
	}
}

void EditScene::load_dialog()
{
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Game::GetInstance()->GetHwnd();
	ofn.lpstrFilter = L"메시 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"stage";

	std::filesystem::path path = fs::current_path();
	ofn.lpstrInitialDir = path.c_str();

	if (GetOpenFileName(&ofn))
	{
		// 파일 이름이 선택되었으면 로드
		std::wstring fileName = szFileName;
		load_basic(fileName);
	}
}
