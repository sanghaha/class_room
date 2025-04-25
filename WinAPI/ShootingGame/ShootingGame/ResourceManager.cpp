#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"	
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
	for (auto& [key, texture] : _texture)
	{
		delete texture;
	}
	_texture.clear();

	for (auto& [key, sprite] : _sprites)
	{
		delete sprite;
	}
	_sprites.clear();
}

Texture* ResourceManager::LoadTexture(wstring key, wstring path, int32 transparent)
{
	if (_texture.find(key) != _texture.end())
	{
		// 이미 존재하는 키라면 리턴
		return _texture[key];
	}

	fs::path fullPath = _resourcePath / path;

	Texture* texture = new Texture();
	texture->Load(_hwnd, fullPath.c_str(), transparent);
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

Sprite* ResourceManager::LoadSprite(wstring key, wstring path, int32 transparent, int32 countX, int32 countY, bool loop)
{
	if (_sprites.find(key) != _sprites.end())
	{
		// 이미 존재하는 키라면 리턴
		return _sprites[key];
	}

	fs::path fullPath = _resourcePath / path;

	Sprite* sprite = new Sprite();
	sprite->Load(_hwnd, fullPath.c_str(), transparent, countX, countY, loop);
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
