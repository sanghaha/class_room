#include "pch.h"
#include "UIWidget.h"

UIWidget::UIWidget(Vector pos) : Super(pos, false)
{
}

UIWidget::~UIWidget()
{
}

void UIWidget::Update(float deltaTime)
{
	if (false == _visible)
		return;

	Super::Update(deltaTime);
}

void UIWidget::Render(ID2D1RenderTarget* renderTarget)
{
	if (false == _visible)
		return;

	Super::Render(renderTarget);
}
