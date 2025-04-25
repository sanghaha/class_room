#include "pch.h"
#include "Sprite.h"
#include "Game.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

//------------------------------------------------------------------
// Bitmap Sprite
//------------------------------------------------------------------

BitmapSprite::BitmapSprite()
{
}

BitmapSprite::~BitmapSprite()
{
}

void BitmapSprite::Load(HWND hwnd, wstring path, int32 transparent, int32 maxCountX, int32 maxCountY)
{
	HDC hdc = ::GetDC(hwnd);

	_hdc = ::CreateCompatibleDC(hdc);
	_bitmap = (HBITMAP)::LoadImageW(
		nullptr,
		path.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);

	if (_bitmap == 0)
	{
		::MessageBox(hwnd, path.c_str(), L"Invalid Bitmap Load", MB_OK);
		return;
	}

	HBITMAP prev = (HBITMAP)::SelectObject(_hdc, _bitmap);
	::DeleteObject(prev);

	BITMAP bit = {};
	::GetObject(_bitmap, sizeof(BITMAP), &bit);

	_sizeX = bit.bmWidth;
	_sizeY = bit.bmHeight;
	_transparent = transparent;
	_maxCountX = maxCountX;
	_maxCountY = maxCountY;

	int32 renderSizeX = _maxCountX != 0 ? (_sizeX / _maxCountX) : _sizeX;
	int32 renderSizeY = _maxCountY != 0 ? (_sizeY / _maxCountY) : _sizeY;
	_renderSize.Width = renderSizeX;
	_renderSize.Height = renderSizeY;
}

void BitmapSprite::Render(HDC hdc, Pos pos, int32 indexX, int32 indexY, int32 dirX)
{
	// �ҽ� ��Ʈ�ʿ��� ������ ���� ��ǥ ���
	int32 srcX = indexX * _renderSize.Width;
	int32 srcY = indexY * _renderSize.Height;

	Pos renderPos = Game::ConvertRenderPos(pos);

	if (_transparent == -1)
	{
		::BitBlt(hdc,	// ����ۿ�
			(int32)renderPos.x,
			(int32)renderPos.y,
			_renderSize.Width,
			_renderSize.Height,
			_hdc,	// �ؽ��� �׸���
			srcX,
			srcY,
			SRCCOPY);
	}
	else
	{
		::TransparentBlt(hdc,
			(int32)renderPos.x,
			(int32)renderPos.y,
			_renderSize.Width,
			_renderSize.Height,
			_hdc,
			srcX,
			srcY,
			_renderSize.Width,
			_renderSize.Height,
			_transparent);
	}
}

//------------------------------------------------------------------
// PNGSprite Sprite
//------------------------------------------------------------------

PNGSprite::PNGSprite()
{
}

PNGSprite::~PNGSprite()
{
}

void PNGSprite::Load(HWND hwnd, wstring path, int32 maxCountX, int32 maxCountY)
{
	// PNG ���� �ε�
	_image = new Image(path.c_str());
	if (_image->GetLastStatus() != Ok)
	{
		MessageBox(nullptr, L"Failed to load image", L"Error", MB_OK);

		delete _image;
		_image = nullptr;
		return;
	}

	// �̹��� ũ�� ��������
	_sizeX = _image->GetWidth();
	_sizeY = _image->GetHeight();

	_maxCountX = maxCountX;
	_maxCountY = maxCountY;

	int32 renderSizeX = _maxCountX != 0 ? (_sizeX / _maxCountX) : _sizeX;
	int32 renderSizeY = _maxCountY != 0 ? (_sizeY / _maxCountY) : _sizeY;
	_renderSize.Width = renderSizeX;
	_renderSize.Height = renderSizeY;
}

void PNGSprite::Render(HDC hdc, Pos pos, int32 indexX, int32 indexY, int32 dirX)
{
	// �ҽ� ��Ʈ�ʿ��� ������ ���� ��ǥ ���
	int32 srcX = indexX * _renderSize.Width;
	int32 srcY = indexY * _renderSize.Height;

	// �¿� ���� ó��
	int32 srcWidth = _renderSize.Width;
	if (dirX < 0) // dirX�� ������ ��� �¿� ����
	{
		srcX += _renderSize.Width; // ������ ������ ����
		srcWidth = -_renderSize.Width; // �ʺ� ������ ����
	}

	// ���� �̹������� �׸� ���� (�ҽ� ����)
	Rect srcRect(srcX, srcY, srcWidth, _renderSize.Height);

	// ��� ��ġ (ȭ�鿡 �׸� ��ġ)
	Pos renderPos = Game::ConvertRenderPos(pos);
	Rect destRect((int32)renderPos.x - _renderSize.Width/2, (int32)renderPos.y - _renderSize.Height/2, _renderSize.Width, _renderSize.Height);

	Graphics graphics(hdc);
	graphics.DrawImage(_image, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);

}
