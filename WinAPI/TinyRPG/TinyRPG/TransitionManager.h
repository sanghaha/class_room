#pragma once
#include "Singleton.h"

enum TransitionType
{
    FadeOut,
    Fragment,
};

class TransitionEffect
{
public:
    TransitionEffect() {}
    virtual ~TransitionEffect() {}

    virtual void Start() abstract;
    virtual bool Update(float deltaTime) abstract;
    virtual void Render(ID2D1HwndRenderTarget* renderTarget, ID2D1Bitmap* capturedBitmap) abstract;
};

class EffectFadeInOut : public TransitionEffect
{
public:
    EffectFadeInOut() {}
    virtual ~EffectFadeInOut() {}

    void Start() override;
    bool Update(float deltaTime) override;
    void Render(ID2D1HwndRenderTarget* renderTarget, ID2D1Bitmap* capturedBitmap) override;

private:
    float _alpha = 1.0f;
    bool _fadingOut = true;
};

class EffectFragment : public TransitionEffect
{
public:
    struct Fragment {
        D2D1_RECT_F srcRect;   // 원본 영역
        D2D1_POINT_2F position; // 현재 위치
        D2D1_POINT_2F velocity; // 이동 속도
        float alpha;            // 투명도
    };

    EffectFragment() {}
    virtual ~EffectFragment() {}

    void Start() override;
    bool Update(float deltaTime) override;
    void Render(ID2D1HwndRenderTarget* renderTarget, ID2D1Bitmap* capturedBitmap) override;

private:
    vector<EffectFragment::Fragment> _fragments;
};

class TransitionManager : public Singleton<TransitionManager>
{
public:
    TransitionManager();
    virtual ~TransitionManager();
    void Start(TransitionType type, std::function<void()> onMidTransition);
    void Update(float deltaTime);
    void Render();
    bool IsActive() const { return _active; }

private:
    void CaptureCurrentScreen();
    void Release();

    ID2D1HwndRenderTarget* _renderTarget;
    ID2D1Bitmap* _capturedBitmap;

    TransitionType _type;
    TransitionEffect* _effectFunc = nullptr;

    bool _active = false;
    std::function<void()> _callback;
};

