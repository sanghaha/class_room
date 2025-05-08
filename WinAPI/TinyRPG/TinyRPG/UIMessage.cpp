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
	//	-40,			   // 글자 높이 (픽셀 단위로 변환). 음수는 실제 사이즈. 
	//	0,                 // 글자 너비 (0이면 자동 설정)
	//	0,                 // 글자 기울기 각도
	//	0,                 // 글자 베이스라인 각도
	//	FW_BOLD,           // 글자 굵기 (FW_NORMAL, FW_BOLD 등)
	//	FALSE,             // 이탤릭 여부
	//	FALSE,             // 밑줄 여부
	//	FALSE,             // 취소선 여부
	//	DEFAULT_CHARSET,   // 문자 집합
	//	OUT_DEFAULT_PRECIS,// 출력 정밀도
	//	CLIP_DEFAULT_PRECIS,// 클리핑 정밀도
	//	DEFAULT_QUALITY,   // 출력 품질
	//	DEFAULT_PITCH | FF_SWISS, // 글꼴 피치와 가족
	//	L"Arial"           // 글꼴 이름
	//);

	//HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	//// 글자 색상 설정
	//SetTextColor(hdc, RGB(255, 255, 255)); // 흰색
	//SetBkMode(hdc, TRANSPARENT);           // 배경 투명

	//// 텍스트 출력
	//wstring str = _message;

	//// 텍스트 크기 계산
	//SIZE textSize;
	//GetTextExtentPoint32(hdc, str.c_str(), static_cast<int32>(str.size()), &textSize);

	//Vector pos(GWinSizeX / 2, GWinSizeY / 2);

	//// 화면 중앙 좌표 계산
	//int32 x = (GWinSizeX - textSize.cx) / 2;
	//int32 y = (GWinSizeY - textSize.cy) / 4;

	//_texture->Render(hdc, graphics, Vector((float)x - 25, (float)y), textSize.cx + 50, 100);
	//TextOut(hdc, x, y, str.c_str(), static_cast<int32>(str.size()));

	//// 이전 폰트 복원 및 새 폰트 삭제
	//SelectObject(hdc, oldFont);
	//DeleteObject(hFont);
}
