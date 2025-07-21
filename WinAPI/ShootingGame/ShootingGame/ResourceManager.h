#pragma once
#include "Singleton.h"

enum class FontSize
{
	Font_12 = 12,
	Font_18 = 18,
	Font_24 = 24,
};

// 리소스들을 관리하는 객체
class ResourceManager : public Singleton<ResourceManager>
{
public:

	void Init(HWND hwnd, fs::path directory);
	void Update(float deltaTime);
	void Destroy() override;

	const HBitmapInfo* LoadHBitmap(wstring key, wstring path, int32 transparent, int32 countX = 1, int32 countY = 1, bool loop = false);
	const HBitmapInfo* GetHBitmap(wstring key);

	wstring GetName() override { return L"ResourceManager"; };
	fs::path GetResourcePath() const { return _resourcePath; }
	HFONT GetFont(FontSize size) 
	{
		if (_fonts.find(size) != _fonts.end())
		{
			return _fonts[size];
		}
		return nullptr; 
	}

public:
	HWND _hwnd;
	fs::path _resourcePath;

	unordered_map<wstring, HBitmapInfo*> _bitmaps;
	unordered_map<FontSize, HFONT> _fonts;
	//HFONT hFont;
};

