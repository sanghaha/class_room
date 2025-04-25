#pragma once
#include "Singleton.h"

class ResourceBase;
class BitmapTexture;
class Sprite;

// ���ҽ����� �����ϴ� ��ü
class ResourceManager : public Singleton<ResourceManager>
{
public:

	void Init(HWND hwnd, fs::path directory);
	void Update(float deltaTime);
	void Destroy() override;

	BitmapTexture* LoadTexture(wstring key, wstring path, int32 transparent = -1);
	BitmapTexture* GetTexture(wstring key);

	Sprite* LoadSprite(wstring key, wstring path, int32 transparent, int32 countX, int32 countY);
	Sprite* LoadPNGSprite(wstring key, wstring path, int32 countX, int32 countY);
	Sprite* GetSprite(wstring key);


public:
	fs::path GetResourcePath() const { return _resourcePath; }

public:
	HWND _hwnd;
	fs::path _resourcePath;

	unordered_map<wstring, BitmapTexture*> _texture;
	unordered_map<wstring, Sprite*> _sprites;
};

