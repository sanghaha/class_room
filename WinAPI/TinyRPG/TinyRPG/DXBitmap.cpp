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

	// 4. Direct2D 비트맵 생성
	hr = renderTarget->CreateBitmapFromWicBitmap(converter, nullptr, &_bitmap);
	if (FAILED(hr)) goto cleanup;

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
