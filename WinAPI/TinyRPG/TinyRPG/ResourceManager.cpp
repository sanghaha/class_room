#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"	
#include "Sprite.h"

void ResourceManager::Init(HWND hwnd, fs::path directory)
{
	_hwnd = hwnd;
	_resourcePath = directory;

	// font
	{
		fs::path fullPath = _resourcePath / L"Font/MaplestoryLight.ttf";
	}
}

void ResourceManager::Update(float deltaTime)
{

}

void ResourceManager::Destroy()
{
	for (auto& [key, res] : _texture)
	{
		delete res;
	}
	_texture.clear();

	for (auto& [key, res] : _sprites)
	{
		delete res;
	}
	_sprites.clear();
}

Texture* ResourceManager::LoadBitmapTexture(wstring key, wstring path, int32 transparent)
{
	if (_texture.find(key) != _texture.end())
	{
		// 이미 존재하는 키라면 리턴
		return _texture[key];
	}

	fs::path fullPath = _resourcePath / path;

	BitmapTexture* texture = new BitmapTexture();
	texture->Load(_hwnd, fullPath.c_str(), transparent);
	_texture[key] = texture;
	return texture;
}

Texture* ResourceManager::LoadSlicedTexture(wstring key, wstring path, int32 left, int32 right)
{
	if (_texture.find(key) != _texture.end())
	{
		// 이미 존재하는 키라면 리턴
		return _texture[key];
	}

	fs::path fullPath = _resourcePath / path;

	Sliced3Texture* texture = new Sliced3Texture();
	texture->Load(_hwnd, fullPath.c_str(), left, right);
	_texture[key] = texture;
	return texture;
}

Texture* ResourceManager::GetTexture(wstring key)
{
	if (_texture.find(key) != _texture.end())
	{
		// 이미 존재하는 키라면 리턴
		return _texture[key];
	}
	return nullptr;
}

Sprite* ResourceManager::LoadSprite(wstring key, wstring path, int32 countX, int32 countY, bool alignCenter)
{
	if (_sprites.find(key) != _sprites.end())
	{
		// 이미 존재하는 키라면 리턴
		return _sprites[key];
	}

	fs::path fullPath = _resourcePath / path;

	Sprite* sprite = new Sprite();
	sprite->Load(fullPath.c_str(), countX, countY, alignCenter);
	_sprites[key] = sprite;
	return sprite;
}

Sprite* ResourceManager::GetSprite(wstring key)
{
	if (_sprites.find(key) != _sprites.end())
	{
		// 이미 존재하는 키라면 리턴
		return _sprites[key];
	}
	return nullptr;
}

//Gdiplus::Font* ResourceManager::GetFont(int32 fontSize)
//{
//	if (_fontCache.find(fontSize) != _fontCache.end())
//	{
//		return _fontCache[fontSize];
//	}
//	return nullptr;
//}
