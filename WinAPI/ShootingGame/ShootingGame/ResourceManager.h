#pragma once
#include "Singleton.h"


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
	HFONT GetFont() { return hFont; }

public:
	HWND _hwnd;
	fs::path _resourcePath;

	unordered_map<wstring, HBitmapInfo*> _bitmaps;
	HFONT hFont;
};

