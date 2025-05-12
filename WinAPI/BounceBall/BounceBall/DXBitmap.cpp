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

void DXBitmap::Load(wstring path, int32 maxCountX, int32 maxCountY, bool transparent)
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

	_transparent = transparent;

	if (transparent)
	{
		// 3. �ӽ� WICBitmap ���� (���� ������ �޸�)
		IWICBitmap* wicBitmap = nullptr;
		wicFactory->CreateBitmapFromSource(converter, WICBitmapCacheOnLoad, &wicBitmap);

		// 4. �ȼ� ���� �����Ͽ� ��� ����ȭ
		UINT width, height;
		wicBitmap->GetSize(&width, &height);

		WICRect rect = { 0, 0, (INT)width, (INT)height };
		UINT stride = width * 4;
		UINT size = stride * height;

		std::vector<BYTE> pixels(size);
		wicBitmap->CopyPixels(&rect, stride, size, pixels.data());

		// 5. ���(RGB=255,255,255) �ȼ��� ���� 0����
		for (UINT i = 0; i < size; i += 4)
		{
			BYTE b = pixels[i];
			BYTE g = pixels[i + 1];
			BYTE r = pixels[i + 2];

			if (r == 255 && g == 255 && b == 255)
			{
				pixels[i + 0] = 0;
				pixels[i + 1] = 0;
				pixels[i + 2] = 0;
				pixels[i + 3] = 0; // A
			}
			else
			{
				pixels[i + 3] = 255;
			}
		}

		// 6. ������ �ȼ��� D2D ��Ʈ�� ����
		D2D1_BITMAP_PROPERTIES props =
			D2D1::BitmapProperties(D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED
			));

		hr = renderTarget->CreateBitmap(
			D2D1::SizeU(width, height), pixels.data(), stride, &props, &_bitmap);
	}
	else
	{
		// 4. Direct2D ��Ʈ�� ����
		hr = renderTarget->CreateBitmapFromWicBitmap(converter, nullptr, &_bitmap);
		if (FAILED(hr)) goto cleanup;
	}

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
