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

	// 1. WIC 디코더를 사용하여 PNG 파일 로드
	HRESULT hr = wicFactory->CreateDecoderFromFilename(
		path.c_str(),                  // 파일 경로
		nullptr,                   // 디코더 공급자 (기본값 사용)
		GENERIC_READ,              // 읽기 권한
		WICDecodeMetadataCacheOnLoad, // 메타데이터 캐시 모드
		&decoder                   // 디코더 출력
	);

	if (FAILED(hr)) goto cleanup;

	// 2. 첫 번째 프레임 디코딩
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) goto cleanup;

	// 3. 포맷 변환 (32비트 BGRA로 변환)
	hr = wicFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) goto cleanup;

	hr = converter->Initialize(
		frame,                          // 입력 프레임
		GUID_WICPixelFormat32bppPBGRA,  // 출력 포맷 (Direct2D 호환)
		WICBitmapDitherTypeNone,        // 디더링 없음
		nullptr,                        // 팔레트 (필요 없음)
		0.0,                            // 알파 임계값
		WICBitmapPaletteTypeCustom      // 사용자 정의 팔레트
	);
	if (FAILED(hr)) goto cleanup;

	_transparent = transparent;

	if (transparent)
	{
		// 3. 임시 WICBitmap 생성 (수정 가능한 메모리)
		IWICBitmap* wicBitmap = nullptr;
		wicFactory->CreateBitmapFromSource(converter, WICBitmapCacheOnLoad, &wicBitmap);

		// 4. 픽셀 직접 수정하여 흰색 투명화
		UINT width, height;
		wicBitmap->GetSize(&width, &height);

		WICRect rect = { 0, 0, (INT)width, (INT)height };
		UINT stride = width * 4;
		UINT size = stride * height;

		std::vector<BYTE> pixels(size);
		wicBitmap->CopyPixels(&rect, stride, size, pixels.data());

		// 5. 흰색(RGB=255,255,255) 픽셀을 알파 0으로
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

		// 6. 수정된 픽셀로 D2D 비트맵 생성
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
		// 4. Direct2D 비트맵 생성
		hr = renderTarget->CreateBitmapFromWicBitmap(converter, nullptr, &_bitmap);
		if (FAILED(hr)) goto cleanup;
	}

	// 이미지 크기 가져오기
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
