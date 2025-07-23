#pragma once
#include "Singleton.h"

class Texture;
class Sound;

// 리소스들을 관리하는 객체
class ResourceManager : public Singleton<ResourceManager>
{
public:

	void Init(HWND hwnd, fs::path directory);
	void Update(float deltaTime);
	void Destroy() override;

	Texture* LoadTexture(wstring key, wstring path, int32 countX = 1, int32 countY = 1, int32 transparent = RGB(255, 255, 255));
	Texture* GetTexture(wstring key);

	const SpriteInfo* GetSpriteInfo(string key);

	//void PlayWAVSound(string fileName);

	Sound* GetSound(const wstring& key) { return _sounds[key]; }
	Sound* LoadSound(const wstring& key, const wstring& path);

public:
	fs::path GetResourcePath() const { return _resourcePath; }
	HPEN GetRedPen() { return redPen; }

private:
	bool loadFont();
	bool createBrushes();
	void createSpriteNameInfo(string spriteName, int32 xCount, int32 totalCount, wstring bitmapKey);
public:
	HWND _hwnd;
	fs::path _resourcePath;

	HPEN redPen;

	// key : path, 
	unordered_map<wstring, Texture*> _bitmap;

	// sprite info
	unordered_map<string, SpriteInfo>	_spriteNames;

	// sound
	unordered_map<wstring, Sound*> _sounds;

};