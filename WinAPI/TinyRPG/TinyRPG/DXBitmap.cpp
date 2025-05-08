#include "pch.h"
#include "DXBitmap.h"
#include "Game.h"

DXBitmap::DXBitmap()
{
}

DXBitmap::~DXBitmap()
{
	SAFE_RELEASE(_bitmap);
}

void DXBitmap::Load(wstring path, int32 maxCountX, int32 maxCountY)
{
	ID2D1HwndRenderTarget* renderTarget = Game::GetInstance()->GetRenderTarget();
	IWICImagingFactory* wicFactory = Game::GetInstance()->GetWICFactory();
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* frame = nullptr;
	IWICFormatConverter* converter = nullptr;
	uint32 renderSizeX = 0;
	uint32 renderSizeY = 0;

	// 1. WIC ���ڴ��� ����Ͽ� PNG ���� �ε�
	HRESULT hr = wicFactory->CreateDecoderFromFilename(
		path.c_str(),                  // ���� ���
		nullptr,                   // ���ڴ� ������ (�⺻�� ���)
		GENERIC_READ,              // �б� ����
		WICDecodeMetadataCacheOnLoad, // ��Ÿ������ ĳ�� ���
		&decoder                   // ���ڴ� ���
	);

	if (FAILED(hr)) goto cleanup;

	// 2. ù ��° ������ ���ڵ�
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) goto cleanup;

	// 3. ���� ��ȯ (32��Ʈ BGRA�� ��ȯ)
	hr = wicFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) goto cleanup;

	hr = converter->Initialize(
		frame,                          // �Է� ������
		GUID_WICPixelFormat32bppPBGRA,  // ��� ���� (Direct2D ȣȯ)
		WICBitmapDitherTypeNone,        // ����� ����
		nullptr,                        // �ȷ�Ʈ (�ʿ� ����)
		0.0,                            // ���� �Ӱ谪
		WICBitmapPaletteTypeCustom      // ����� ���� �ȷ�Ʈ
	);
	if (FAILED(hr)) goto cleanup;

	// 4. Direct2D ��Ʈ�� ����
	hr = renderTarget->CreateBitmapFromWicBitmap(converter, nullptr, &_bitmap);
	if (FAILED(hr)) goto cleanup;

	// �̹��� ũ�� ��������
	frame->GetSize(&_bitmapSizeX, &_bitmapSizeY);
	
	_maxCountX = maxCountX;
	_maxCountY = maxCountY;
	_frameSizeX = _bitmapSizeX / _maxCountX;
	_frameSizeY = _bitmapSizeY / _maxCountY;

cleanup:
	if (decoder) decoder->Release();
	if (frame) frame->Release();
	if (converter) converter->Release();
}
