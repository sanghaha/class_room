#include "pch.h"
#include "Sprite.h"
#include "Game.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	SAFE_RELEASE(_bitmap);
}

void Sprite::Load(wstring path, int32 maxCountX, int32 maxCountY, bool alignCenter)
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
	frame->GetSize(&_sizeX, &_sizeY);

	_maxCountX = maxCountX;
	_maxCountY = maxCountY;
	_alignCenter = alignCenter;

	renderSizeX = _maxCountX != 0 ? (_sizeX / _maxCountX) : _sizeX;
	renderSizeY = _maxCountY != 0 ? (_sizeY / _maxCountY) : _sizeY;
	_renderSize.Width = renderSizeX;
	_renderSize.Height = renderSizeY;

cleanup:
	if (decoder) decoder->Release();
	if (frame) frame->Release();
	if (converter) converter->Release();
}

void Sprite::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 indexX, int32 indexY, int32 dirX)
{
	// 소스 비트맵에서 복사할 시작 좌표 계산
	float srcX = indexX * (float)_renderSize.Width;
	float srcY = indexY * (float)_renderSize.Height;

	// 원본 비트맵에서 그릴 영역 (소스 영역)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcX + 1.0f,
		srcY + 1.0f,
		srcX + _renderSize.Width - 1.0f,
		srcY + _renderSize.Height - 1.0f);

	Vector renderPos = Game::ConvertRenderPos(pos);

	// 화면에 렌더링할 위치와 크기 (대상 영역)
	D2D1_RECT_F destRect;
	if (_alignCenter)
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x - _renderSize.Width * 0.5f),
			roundf(renderPos.y - _renderSize.Height * 0.5f),
			roundf(renderPos.x + _renderSize.Width * 0.5f),
			roundf(renderPos.y + _renderSize.Height * 0.5f));
	}
	else
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x),
			roundf(renderPos.y),
			roundf(renderPos.x + _renderSize.Width),
			roundf(renderPos.y + _renderSize.Height));
	}

	// 기존 변환 상태 저장
	D2D1_MATRIX_3X2_F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	// 좌우 반전 변환 행렬 설정
	if (dirX < 0) // dirX가 음수일 경우 좌우 반전
	{
		D2D1_MATRIX_3X2_F flipTransform = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f), // X축 반전, Y축 그대로
			D2D1::Point2F(renderPos.x, renderPos.y) // 반전 기준점
		);
		renderTarget->SetTransform(flipTransform * originalTransform);
	}

	// 비트맵 렌더링
	renderTarget->DrawBitmap(_bitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);

	// 원래 변환 상태 복원
	renderTarget->SetTransform(originalTransform);

}
