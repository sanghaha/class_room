#pragma once
#include "Singleton.h"

class ResourceBase;
class Texture;
class Sprite;

// ���ҽ����� �����ϴ� ��ü
class ResourceManager : public Singleton<ResourceManager>
{
public:

	void Init(HWND hwnd, fs::path directory);
	void Update(float deltaTime);
	void Destroy() override;

	Texture* LoadTexture(wstring key, wstring path, int32 transparent = -1);
	Texture* GetTexture(wstring key);

	Sprite* LoadSprite(wstring key, wstring path, int32 transparent, int32 countX, int32 countY, bool loop);
	Sprite* GetSprite(wstring key);

public:
	HWND _hwnd;
	fs::path _resourcePath;

	unordered_map<wstring, Texture*> _texture;
	unordered_map<wstring, Sprite*> _sprites;
};

