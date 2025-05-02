#pragma once
#include "Singleton.h"

class BaseBitmap;
class Sprite;

// 리소스들을 관리하는 객체
class ResourceManager : public Singleton<ResourceManager>
{
public:

	void Init(HWND hwnd, fs::path directory);
	void Update(float deltaTime);
	void Destroy() override;

	BaseBitmap* LoadSlicedTexture(wstring key, wstring path, int32 left, int32 right);
	BaseBitmap* GetTexture(wstring key);

	Sprite* LoadSprite(wstring key, wstring path, int32 countX, int32 countY, bool alignCenter = true);
	Sprite* GetSprite(wstring key);

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

	unordered_map<wstring, BaseBitmap*> _texture;
	unordered_map<wstring, Sprite*>		_sprites;

	IDWriteFactory5* _dwriteFactory = nullptr;
	IDWriteFontCollection1* _fontCollection = nullptr;
	IDWriteFontFile* _fontFile = nullptr;
	IDWriteFontSet* _fontSet = nullptr;
	IDWriteFontSetBuilder1* _fontSetBuilder = nullptr;
	unordered_map<FontSize, IDWriteTextFormat*> _fontCache;
	unordered_map<BrushColor, ID2D1SolidColorBrush*> _brushCache;
};

