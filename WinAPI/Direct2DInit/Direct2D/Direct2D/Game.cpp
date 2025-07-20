#include "Game.h"

using namespace std;


void Game::Init(HWND hwnd)
{
	RECT _rect;
	::GetClientRect(hwnd, &_rect);

	// dx init
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_dxFactory);

	D2D1_SIZE_U size = D2D1::SizeU(_rect.right - _rect.left, _rect.bottom - _rect.top);

	// Create a Direct2D render target.
	_dxFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&_dxRenderTarget);

	// 이미지 로드 초기화
	CoInitialize(NULL);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_wicFactory));

	wchar_t buffer[MAX_PATH];
	DWORD length = ::GetCurrentDirectory(MAX_PATH, buffer);
	fs::path currentPath = fs::path(buffer) / L"../Resources/Warrior_Blue.png";

	LoadTexture(currentPath.c_str(), 6, 8);
}

void Game::Update()
{
}

void Game::Render()
{
	_dxRenderTarget->BeginDraw();
	_dxRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));


	int posX = 100;
	int posY = 100;

	// 소스 비트맵에서 복사할 시작 좌표 계산
	float srcX = _indexX * (float)_frameSizeX;
	float srcY = _indexY * (float)_frameSizeY;

	D2D1_RECT_F srcLeft = D2D1::RectF(
		0,
		0,
		static_cast<float>(_frameSizeX),
		static_cast<float>(_frameSizeY));

	D2D1_RECT_F destLeft = D2D1::RectF(
		posX,
		posY,
		posX + static_cast<float>(_frameSizeX),
		posY + static_cast<float>(_frameSizeY)
	);
	_dxRenderTarget->DrawBitmap(_bitmap, destLeft, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcLeft);


	_dxRenderTarget->EndDraw();
}

void Game::LoadTexture(wstring path, int maxCountX, int maxCountY)
{
	ID2D1HwndRenderTarget* renderTarget = _dxRenderTarget;
	IWICImagingFactory* wicFactory = _wicFactory;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* frame = nullptr;
	IWICFormatConverter* converter = nullptr;
	int renderSizeX = 0;
	int renderSizeY = 0;

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
