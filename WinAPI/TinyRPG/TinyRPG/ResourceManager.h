#pragma once
#include "Singleton.h"

class PNGTexture;
class Sliced3Texture;
class Sprite;
class DXBitmap;

// 리소스들을 관리하는 객체
class ResourceManager : public Singleton<ResourceManager>
{
public:

	void Init(HWND hwnd, fs::path directory);
	void Update(float deltaTime);
	void Destroy() override;

	DXBitmap* LoadDXBitmap(wstring key, wstring path, int32 countX, int32 countY);
	DXBitmap* GetDXBitmap(wstring key);

	const SpriteIndex* GetItemSpriteIndex(string key);

	IDWriteTextFormat* GetFont(FontSize fontSize);
	ID2D1SolidColorBrush* GetBrush(BrushColor color);


public:
	fs::path GetResourcePath() const { return _resourcePath; }

private:
	bool loadFont();
	bool createBrushes();

public:
	HWND _hwnd;
	fs::path _resourcePath;

	// key : path, 
	unordered_map<wstring, DXBitmap*> _bitmap;

	IDWriteFactory5* _dwriteFactory = nullptr;
	IDWriteFontCollection1* _fontCollection = nullptr;
	IDWriteFontFile* _fontFile = nullptr;
	IDWriteFontSet* _fontSet = nullptr;
	IDWriteFontSetBuilder1* _fontSetBuilder = nullptr;
	unordered_map<FontSize, IDWriteTextFormat*> _fontCache;
	unordered_map<BrushColor, ID2D1SolidColorBrush*> _brushCache;

	unordered_map<string, SpriteIndex>	_itemSpriteNames;
};

