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
		_itemSpriteNames.emplace("Meat1", SpriteIndex{2, 15});
		_itemSpriteNames.emplace("Potion1", SpriteIndex{4, 19});
		_itemSpriteNames.emplace("Sword1", SpriteIndex{2, 5});
		_itemSpriteNames.emplace("Bow1", SpriteIndex{3, 6});
		_itemSpriteNames.emplace("Armor", SpriteIndex{5, 7});
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

DXBitmap* ResourceManager::LoadDXBitmap(wstring key, wstring path, int32 countX, int32 countY)
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

DXBitmap* ResourceManager::GetDXBitmap(wstring key)
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

const SpriteIndex* ResourceManager::GetItemSpriteIndex(string key)
{
	if (_itemSpriteNames.find(key) != _itemSpriteNames.end())
	{
		// 이미 존재하는 키라면 리턴
		return &_itemSpriteNames[key];
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

//Gdiplus::Font* ResourceManager::GetFont(int32 fontSize)
//{
//	if (_fontCache.find(fontSize) != _fontCache.end())
//	{
//		return _fontCache[fontSize];
//	}
//	return nullptr;
//}
