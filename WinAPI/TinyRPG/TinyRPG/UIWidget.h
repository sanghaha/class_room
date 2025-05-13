#pragma once

#include "Actor.h"

class UIWidget : public Actor
{
	using Super = Actor;
public:
	UIWidget(Vector pos);
	virtual ~UIWidget();

	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_UI; }

	bool GetVisible() const { return _visible; }
	void SetVisible(bool visible) { _visible = visible; }

protected:
	bool _visible = true;
};

