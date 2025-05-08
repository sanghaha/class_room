#include "pch.h"
#include "TransitionManager.h"
#include "Game.h"
#include "GameScene.h"

TransitionManager::TransitionManager()
{
}

TransitionManager::~TransitionManager()
{
    SAFE_DELETE(_effectFunc);
    SAFE_RELEASE(_capturedBitmap);
}

void TransitionManager::Start(TransitionType type, std::function<void()> onMidTransition)
{
    _renderTarget = Game::GetInstance()->GetRenderTarget();

    CaptureCurrentScreen();

    SAFE_DELETE(_effectFunc);
    if (type == TransitionType::FadeOut)
    {
        _effectFunc = new EffectFadeInOut();
    }
    else if (type == TransitionType::Fragment)
    {
        _effectFunc = new EffectFragment();
    }

    _effectFunc->Start();
    _active = true;
    _callback = onMidTransition;
}

void TransitionManager::Update(float deltaTime)
{
    if (!_active) return;

    if (_effectFunc->Update(deltaTime))
    {
        if (_callback) _callback();
        _active = false;
    }
}

void TransitionManager::Render()
{
    if (!_active || !_capturedBitmap) return;

    _effectFunc->Render(_renderTarget, _capturedBitmap);
}

void TransitionManager::CaptureCurrentScreen()
{
    ID2D1BitmapRenderTarget* pOffscreen;
    _renderTarget->CreateCompatibleRenderTarget(&pOffscreen);

    pOffscreen->BeginDraw();
    Game::GetInstance()->GetGameScene()->Render(pOffscreen);
    pOffscreen->EndDraw();

    pOffscreen->GetBitmap(&_capturedBitmap);
}

void TransitionManager::Release()
{
    SAFE_RELEASE(_capturedBitmap);
    _callback = nullptr;
}

void EffectFadeInOut::Start()
{
    _alpha = 1.0f;
    _fadingOut = true;
}

bool EffectFadeInOut::Update(float deltaTime)
{
    const float speed = 1.0f;
    if (_fadingOut) 
    {
        _alpha -= deltaTime * speed;
        if (_alpha <= 0.0f) 
        {
            _alpha = 0.0f;
            _fadingOut = false; 
            return true;
        }
    }
    else 
    {
        _alpha += deltaTime * speed;
        if (_alpha >= 1.0f) 
        {
            _alpha = 1.0f;
            return true;
        }
    }

    return false;
}

void EffectFadeInOut::Render(ID2D1HwndRenderTarget* renderTarget, ID2D1Bitmap* capturedBitmap)
{
    renderTarget->DrawBitmap(
        capturedBitmap,
        nullptr,
        _alpha,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
    );
}

void EffectFragment::Start()
{
    _fragments.clear();

    int cols = 4, rows = 4;
    float tileW = (float)GWinSizeX / cols;
    float tileH = (float)GWinSizeY / rows;

    for (int y = 0; y < rows; y++) 
    {
        for (int x = 0; x < cols; x++) 
        {
            Fragment frag;
            frag.srcRect = D2D1::RectF(x * tileW, y * tileH, (x + 1) * tileW, (y + 1) * tileH);
            frag.position = { x * tileW, y * tileH };

            // 랜덤 이동 방향
            frag.velocity = { (rand() % 5 - 2) * 2.0f, (rand() % 5 - 2) * 2.0f };
            frag.alpha = 1.0f;

            _fragments.push_back(frag);
        }
    }
}

bool EffectFragment::Update(float deltaTime)
{
    // 이동 및 사라짐 처리
    int32 endCount = 0;
    for (auto& frag : _fragments)
    {
        frag.position.x += frag.velocity.x;
        frag.position.y += frag.velocity.y;
        frag.alpha -= 0.02f; // 점점 사라짐
        if (frag.alpha < 0.0f)
        {
            frag.alpha = 0.0f;
            endCount++;
        }
    }

    return (endCount == _fragments.size());
}

void EffectFragment::Render(ID2D1HwndRenderTarget* renderTarget, ID2D1Bitmap* capturedBitmap)
{
    // 파편 하나씩 그리기
    for (auto& frag : _fragments)
    {
        D2D1_RECT_F dst = 
        {
            frag.position.x, frag.position.y,
            frag.position.x + (frag.srcRect.right - frag.srcRect.left),
            frag.position.y + (frag.srcRect.bottom - frag.srcRect.top)
        };

        renderTarget->DrawBitmap(capturedBitmap, dst, frag.alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, frag.srcRect);
    }
}
