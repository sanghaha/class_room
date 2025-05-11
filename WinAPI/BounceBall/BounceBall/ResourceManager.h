#pragma once
#include "Singleton.h"

class DXBitmap;
class BaseResource;

// ���ҽ����� �����ϴ� ��ü
class ResourceManager : public Singleton<ResourceManager>
{
public:

	void Init(HWND hwnd, fs::path directory);
	void Update(float deltaTime);
	void Destroy() override;

	DXBitmap* LoadDXBitmap(wstring key, wstring path, int32 countX = 1, int32 countY = 1);
	DXBitmap* GetDXBitmap(wstring key);

	const SpriteInfo* GetSpriteInfo(string key);

	IDWriteTextFormat* GetFont(FontSize fontSize);
	ID2D1SolidColorBrush* GetBrush(BrushColor color);

public:
	fs::path GetResourcePath() const { return _resourcePath; }

private:
	bool loadFont();
	bool createBrushes();
	void createSpriteNameInfo(string spriteName, int32 xCount, int32 totalCount, wstring bitmapKey);
public:
	HWND _hwnd;
	fs::path _resourcePath;

	// key : path, 
	unordered_map<wstring, DXBitmap*> _bitmap;

	// sprite info
	unordered_map<string, SpriteInfo>	_spriteNames;

	IDWriteFactory5* _dwriteFactory = nullptr;
	IDWriteFontCollection1* _fontCollection = nullptr;
	IDWriteFontFile* _fontFile = nullptr;
	IDWriteFontSet* _fontSet = nullptr;
	IDWriteFontSetBuilder1* _fontSetBuilder = nullptr;
	unordered_map<FontSize, IDWriteTextFormat*> _fontCache;
	unordered_map<BrushColor, ID2D1SolidColorBrush*> _brushCache;

};