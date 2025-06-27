#include "pch.h"
#include "ResourceManager.h"
#include "DXBitmap.h"
#include "Texture.h"	
#include "Sprite.h"
#include "Game.h"

void ResourceManager::Init(HWND hwnd, fs::path directory)
{
	_hwnd = hwnd;
	_resourcePath = directory;

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5), (IUnknown**)&_dwriteFactory);

	{
		// UI-Item Icon
		_spriteNames.emplace("UI_Meat1", SpriteInfo{2, 15, "Items", false});
		_spriteNames.emplace("UI_Potion1", SpriteInfo{4, 19, "Items", false });
		_spriteNames.emplace("UI_Sword1", SpriteInfo{2, 5, "Items", false });
		_spriteNames.emplace("UI_Bow1", SpriteInfo{3, 6, "Items", false });
		_spriteNames.emplace("UI_Armor1", SpriteInfo{5, 7, "Items", false });

		// Actor-Item Icon
		_spriteNames.emplace("Meat1", SpriteInfo{ 2, 15, "Items", true });
		_spriteNames.emplace("Potion1", SpriteInfo{ 4, 19, "Items", true });
		_spriteNames.emplace("Sword1", SpriteInfo{ 2, 5, "Items", true });
		_spriteNames.emplace("Bow1", SpriteInfo{ 3, 6, "Items", true });
		_spriteNames.emplace("Armor1", SpriteInfo{ 5, 7, "Items", true });

		// Effect
		_spriteNames.emplace("Explosion", SpriteInfo{ 0, 0, "Explosion", true});

		// Map
		_spriteNames.emplace("TileMap", SpriteInfo{ 0, 0, "TileMap", false });

		// Player
		_spriteNames.emplace("Warrior_Blue", SpriteInfo{ 0, 0, "Warrior_Blue", true });
		_spriteNames.emplace("Bow_Blue", SpriteInfo{ 0, 0, "Bow_Blue", true });
		_spriteNames.emplace("Arrow", SpriteInfo{ 0, 0, "Arrow", true });

		// Enemy
		_spriteNames.emplace("Torch_Red", SpriteInfo{ 0, 0, "Torch_Red", true });

		// UI
		_spriteNames.emplace("HUD_AttackIcon", SpriteInfo{ 1, 0, "HudIcons", false });
		_spriteNames.emplace("HUD_DefenseIcon", SpriteInfo{ 0, 0, "HudIcons", false });
		_spriteNames.emplace("HUD_Number", SpriteInfo{ 0, 0, "Numbers", false });
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

DXBitmap* ResourceManager::LoadDXBitmap(string key, wstring path, int32 countX, int32 countY)
{
	if (_bitmap.find(key) != _bitmap.end())
	{
		// 이미 존재하는 키라면 리턴
		return _bitmap[key];
	}

	fs::path fullPath = _resourcePath / path;

	DXBitmap* bitmap = new DXBitmap();
	bitmap->Load(fullPath.c_str(), countX, countY);
	_bitmap[key] = bitmap;
	return bitmap;
}

DXBitmap* ResourceManager::GetDXBitmap(string key)
{
	if (_bitmap.find(key) != _bitmap.end())
	{
		// 이미 존재하는 키라면 리턴
		return _bitmap[key];
	}

	return nullptr;
}

//Sliced3Texture* ResourceManager::LoadSlicedTexture(wstring key, wstring path, int32 left, int32 right)
//{
//	if (_slicedtexture.find(key) != _slicedtexture.end())
//	{
//		// 이미 존재하는 키라면 리턴
//		return _slicedtexture[key];
//	}
//
//	fs::path fullPath = _resourcePath / path;
//
//	Sliced3Texture* texture = new Sliced3Texture();
//	texture->Load(fullPath.c_str(), left, right);
//	_slicedtexture[key] = texture;
//	return texture;
//}
//
//Sliced3Texture* ResourceManager::GetSlicedexture(wstring key)
//{
//	if (_slicedtexture.find(key) != _slicedtexture.end())
//	{
//		// 이미 존재하는 키라면 리턴
//		return _slicedtexture[key];
//	}
//	return nullptr;
//}
//
//PNGTexture* ResourceManager::LoadPNGTexture(wstring key, wstring path, int32 width, int32 height)
//{
//	if (_texture.find(key) != _texture.end())
//	{
//		// 이미 존재하는 키라면 리턴
//		return _texture[key];
//	}
//
//	fs::path fullPath = _resourcePath / path;
//
//	PNGTexture* texture = new PNGTexture();
//	texture->Load(fullPath.c_str(), width, height);
//	_texture[key] = texture;
//	return texture;
//}
//
//PNGTexture* ResourceManager::GetPNGTexture(wstring key)
//{
//	if (_texture.find(key) != _texture.end())
//	{
//		// 이미 존재하는 키라면 리턴
//		return _texture[key];
//	}
//	return nullptr;
//}
//
//Sprite* ResourceManager::LoadSprite(wstring key, wstring path, int32 countX, int32 countY)
//{
//	if (_sprites.find(key) != _sprites.end())
//	{
//		// 이미 존재하는 키라면 리턴
//		return _sprites[key];
//	}
//
//	fs::path fullPath = _resourcePath / path;
//
//	Sprite* sprite = new Sprite();
//	sprite->Load(fullPath.c_str(), countX, countY);
//	_sprites[key] = sprite;
//	return sprite;
//}
//
//Sprite* ResourceManager::GetSprite(wstring key)
//{
//	if (_sprites.find(key) != _sprites.end())
//	{
//		// 이미 존재하는 키라면 리턴
//		return _sprites[key];
//	}
//	return nullptr;
//}

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
	createFont(FontSize::FONT_16);
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
	return false;
}

void ResourceManager::createSpriteNameInfo(string spriteName, int32 xCount, int32 totalCount, wstring bitmapKey)
{
}
