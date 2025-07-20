#include "pch.h"
#include "ResourceManager.h"
#include "DXBitmap.h"
#include "Texture.h"	
#include "Sprite.h"
#include "AnimSprite.h"
#include "Game.h"
#include "Sound.h"

void ResourceManager::Init(HWND hwnd, fs::path directory)
{
	_hwnd = hwnd;
	_resourcePath = directory;

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5), (IUnknown**)&_dwriteFactory);

	{
		createSpriteNameInfo("Ball", 1, 3, L"Ball");
		createSpriteNameInfo("Block", 3, 17, L"Block");
		createSpriteNameInfo("Star", 1, 3, L"Star");
		_spriteNames.emplace("EatStarEffect", SpriteInfo{0, 0, L"EatStarEffect", true});
		_spriteNames.emplace("DeadBall", SpriteInfo{ 0, 0, L"DeadBall", true });
	}

	// font
	{
		loadFont();
	}

	// brush
	{
		createBrushes();
	}
}

void ResourceManager::Update(float deltaTime)
{

}

void ResourceManager::Destroy()
{
	for (auto& [key, res] : _bitmap)
	{
		delete res;
	}
	_bitmap.clear();

	for (auto& [key, res] : _fontCache)
	{
		SAFE_RELEASE(res);
	}
	_fontCache.clear();

	for (auto& [key, res] : _brushCache)
	{
		SAFE_RELEASE(res);
	}
	_brushCache.clear();

	SAFE_RELEASE(_fontCollection);
	SAFE_RELEASE(_fontSet);
	SAFE_RELEASE(_fontSetBuilder);
	SAFE_RELEASE(_dwriteFactory);
	SAFE_RELEASE(_fontFile);
}

DXBitmap* ResourceManager::LoadDXBitmap(wstring key, wstring path, int32 countX, int32 countY, bool transparent)
{
	if (_bitmap.find(key) != _bitmap.end())
	{
		// 이미 존재하는 키라면 리턴
		return _bitmap[key];
	}

	fs::path fullPath = _resourcePath / "Image/" / path;

	DXBitmap* bitmap = new DXBitmap();
	bitmap->Load(fullPath.c_str(), countX, countY, transparent);
	_bitmap[key] = bitmap;
	return bitmap;
}

DXBitmap* ResourceManager::GetDXBitmap(wstring key)
{
	if (_bitmap.find(key) != _bitmap.end())
	{
		// 이미 존재하는 키라면 리턴
		return _bitmap[key];
	}

	return nullptr;
}

const SpriteInfo* ResourceManager::GetSpriteInfo(string key)
{
	if (_spriteNames.find(key) != _spriteNames.end())
	{
		// 이미 존재하는 키라면 리턴
		return &_spriteNames[key];
	}
	return nullptr;
}

IDWriteTextFormat* ResourceManager::GetFont(FontSize fontSize)
{
	if (_fontCache.find(fontSize) != _fontCache.end())
	{
		// 이미 존재하는 키라면 리턴
		return _fontCache[fontSize];
	}
	return nullptr;
}

ID2D1SolidColorBrush* ResourceManager::GetBrush(BrushColor color)
{
	if (_brushCache.find(color) != _brushCache.end())
	{
		// 이미 존재하는 키라면 리턴
		return _brushCache[color];
	}
	return nullptr;
}

Sound* ResourceManager::LoadSound(const wstring& key, const wstring& path)
{
	if (_sounds.find(key) != _sounds.end())
		return _sounds[key];

	fs::path fullPath = _resourcePath / path;

	Sound* sound = new Sound();
	sound->LoadWave(fullPath);
	_sounds[key] = sound;

	return sound;
}

//void ResourceManager::PlayWAVSound(string fileName)
//{
//	fs::path fullPath = _resourcePath / L"Sound/" / fileName;
//	PlaySound(fullPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
//}

bool ResourceManager::loadFont()
{
	fs::path fullPath = _resourcePath / L"Font/MaplestoryLight.ttf";
	HRESULT hr;

	hr = _dwriteFactory->CreateFontFileReference(
		fullPath.c_str(),  // 경로
		nullptr,                   // 마지막 수정시간 (nullptr이면 현재 파일 기준)
		&_fontFile
	);
	if (FAILED(hr)) return false;

	hr = _dwriteFactory->CreateFontSetBuilder(&(_fontSetBuilder));
	if (FAILED(hr)) return false;

	_fontSetBuilder->AddFontFile(_fontFile);

	hr = _fontSetBuilder->CreateFontSet(&_fontSet);
	if (FAILED(hr)) return false;

	hr = _dwriteFactory->CreateFontCollectionFromFontSet(_fontSet, &_fontCollection);
	if (FAILED(hr)) return false;

	IDWriteFontFamily* fontFamily = nullptr;
	_fontCollection->GetFontFamily(0, &fontFamily);

	IDWriteLocalizedStrings* pFamilyNames = nullptr;
	fontFamily->GetFamilyNames(&pFamilyNames);

	UINT32 index = 0;
	BOOL exists = FALSE;
	pFamilyNames->FindLocaleName(L"ko-kr", &index, &exists);
	if (!exists) index = 0;

	WCHAR familyName[100] = {};
	pFamilyNames->GetString(index, familyName, 100);

	// 사이즈에 맞게 폰트 생성
	auto createFont = [&](FontSize size)
		{
			int32 fontSize = (int32)size;
			IDWriteTextFormat* textFormat = nullptr;
			HRESULT hr = _dwriteFactory->CreateTextFormat(
				familyName, _fontCollection,
				DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, (float)fontSize, L"ko-kr",
				&textFormat
			);
			if (FAILED(hr)) return false;

			_fontCache[size] = textFormat;
			return true;
		};

	createFont(FontSize::FONT_12);
	createFont(FontSize::FONT_20);
	createFont(FontSize::FONT_30);

	SAFE_RELEASE(pFamilyNames);
	SAFE_RELEASE(fontFamily);
	return SUCCEEDED(hr);
}

bool ResourceManager::createBrushes()
{
	ID2D1HwndRenderTarget* renderTarget = Game::GetInstance()->GetRenderTarget();
	if (nullptr == renderTarget)
		return false;

	{
		ID2D1SolidColorBrush* brush = nullptr;
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush);
		_brushCache[BrushColor::Red] = brush;
	}
	{
		ID2D1SolidColorBrush* brush = nullptr;
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
		_brushCache[BrushColor::White] = brush;
	}
	{
		ID2D1SolidColorBrush* brush = nullptr;
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &brush);
		_brushCache[BrushColor::Blue] = brush;
	}
	{
		ID2D1SolidColorBrush* brush = nullptr;
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &brush);
		_brushCache[BrushColor::Green] = brush;
	}
	return false;
}

void ResourceManager::createSpriteNameInfo(string spriteName, int32 xCount, int32 totalCount, wstring bitmapKey)
{
	for (int32 i = 0; i < totalCount; ++i)
	{
		int32 x = i % xCount;
		int32 y = i / xCount;
		_spriteNames.emplace(std::format("{0}_{1}", spriteName, i), SpriteInfo{ x, y, bitmapKey, true });
	}
}


