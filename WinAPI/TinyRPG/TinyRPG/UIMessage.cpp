#include "pch.h"
#include "UIMessage.h"
#include "Texture.h"
#include "ResourceManager.h"

UIMessage::UIMessage()
{
}

UIMessage::~UIMessage()
{
	SAFE_DELETE(_texture);
}

void UIMessage::Init()
{
	_texture = new Sliced3Texture(L"Ribbon_Red_3Slides", 68, 68);
}

void UIMessage::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (_isOpen == false)
		return;

	if (_texture == nullptr)
		return;

	//HFONT hFont = CreateFont(
	//	-40,			   // ���� ���� (�ȼ� ������ ��ȯ). ������ ���� ������. 
	//	0,                 // ���� �ʺ� (0�̸� �ڵ� ����)
	//	0,                 // ���� ���� ����
	//	0,                 // ���� ���̽����� ����
	//	FW_BOLD,           // ���� ���� (FW_NORMAL, FW_BOLD ��)
	//	FALSE,             // ���Ÿ� ����
	//	FALSE,             // ���� ����
	//	FALSE,             // ��Ҽ� ����
	//	DEFAULT_CHARSET,   // ���� ����
	//	OUT_DEFAULT_PRECIS,// ��� ���е�
	//	CLIP_DEFAULT_PRECIS,// Ŭ���� ���е�
	//	DEFAULT_QUALITY,   // ��� ǰ��
	//	DEFAULT_PITCH | FF_SWISS, // �۲� ��ġ�� ����
	//	L"Arial"           // �۲� �̸�
	//);

	//HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	//// ���� ���� ����
	//SetTextColor(hdc, RGB(255, 255, 255)); // ���
	//SetBkMode(hdc, TRANSPARENT);           // ��� ����

	//// �ؽ�Ʈ ���
	//wstring str = _message;

	//// �ؽ�Ʈ ũ�� ���
	//SIZE textSize;
	//GetTextExtentPoint32(hdc, str.c_str(), static_cast<int32>(str.size()), &textSize);

	//Vector pos(GWinSizeX / 2, GWinSizeY / 2);

	//// ȭ�� �߾� ��ǥ ���
	//int32 x = (GWinSizeX - textSize.cx) / 2;
	//int32 y = (GWinSizeY - textSize.cy) / 4;

	//_texture->Render(hdc, graphics, Vector((float)x - 25, (float)y), textSize.cx + 50, 100);
	//TextOut(hdc, x, y, str.c_str(), static_cast<int32>(str.size()));

	//// ���� ��Ʈ ���� �� �� ��Ʈ ����
	//SelectObject(hdc, oldFont);
	//DeleteObject(hFont);
}
