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

	DXBitmap* LoadDXBitmap(wstring path);

	Sliced3Texture* LoadSlicedTexture(wstring key, wstring path, int32 left, int32 right);
	Sliced3Texture* GetSlicedexture(wstring key);

	PNGTexture* LoadPNGTexture(wstring key, wstring path, int32 width = 0, int32 height = 0);
	PNGTexture* GetPNGTexture(wstring key);

	Sprite* LoadSprite(wstring key, wstring path, int32 countX, int32 countY);
	Sprite* GetSprite(wstring key);
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

	unordered_map<wstring, Sliced3Texture*> _slicedtexture;
	unordered_map<wstring, PNGTexture*> _texture;
	unordered_map<wstring, Sprite*>		_sprites;

	IDWriteFactory5* _dwriteFactory = nullptr;
	IDWriteFontCollection1* _fontCollection = nullptr;
	IDWriteFontFile* _fontFile = nullptr;
	IDWriteFontSet* _fontSet = nullptr;
	IDWriteFontSetBuilder1* _fontSetBuilder = nullptr;
	unordered_map<FontSize, IDWriteTextFormat*> _fontCache;
	unordered_map<BrushColor, ID2D1SolidColorBrush*> _brushCache;

	unordered_map<string, SpriteIndex>	_itemSpriteNames;
};

