#pragma once
#include "Actor.h"

class Portal : public Actor
{
	using Super = Actor;
public:
	Portal(Vector pos);
	virtual ~Portal();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	bool IsBlockingCell() override { return false; }

	virtual RenderLayer GetRenderLayer() override;

	void EnterNextStage();
};

