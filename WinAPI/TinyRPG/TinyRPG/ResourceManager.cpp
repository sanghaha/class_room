#include "pch.h"
#include "ResourceManager.h"
#include "BitmapTexture.h"	
#include "Sprite.h"

void ResourceManager::Init(HWND hwnd, fs::path directory)
{
	_hwnd = hwnd;
	_resourcePath = directory;
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

BitmapTexture* ResourceManager::LoadTexture(wstring key, wstring path, int32 transparent)
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

BitmapTexture* ResourceManager::GetTexture(wstring key)
{
	if (_texture.find(key) != _texture.end())
	{
		// 이미 존재하는 키라면 리턴
		return _texture[key];
	}
	return nullptr;
}

Sprite* ResourceManager::LoadSprite(wstring key, wstring path, int32 transparent, int32 countX, int32 countY)
{
	if (_sprites.find(key) != _sprites.end())
	{
		// 이미 존재하는 키라면 리턴
		return _sprites[key];
	}

	fs::path fullPath = _resourcePath / path;

	BitmapSprite* sprite = new BitmapSprite();
	sprite->Load(_hwnd, fullPath.c_str(), transparent, countX, countY);
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

Sprite* ResourceManager::LoadPNGSprite(wstring key, wstring path, int32 countX, int32 countY)
{
	if (_sprites.find(key) != _sprites.end())
	{
		// 이미 존재하는 키라면 리턴
		return _sprites[key];
	}

	fs::path fullPath = _resourcePath / path;

	PNGSprite* sprite = new PNGSprite();
	sprite->Load(_hwnd, fullPath.c_str(), countX, countY);
	_sprites[key] = sprite;
	return sprite;
}