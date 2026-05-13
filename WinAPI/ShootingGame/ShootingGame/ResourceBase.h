#pragma once

#include "Component.h"

//class ResourceBase : public Component
//{
//	using Super = Component;
//public:
//	ResourceBase(wstring key, int32 width, int32 height);
//	virtual ~ResourceBase();
//
//	Size GetSize() { return _size; }
//	const HBitmapInfo* GetBitmapInfo() const { return _bitmapInfo; }
//	void SetCameraApply(bool apply) { _applyCamera = apply; }
//	void SetCenterApply(bool apply) { _applyCenter = apply; }
//
//protected:
//	Size				_size = {};
//	const HBitmapInfo*	_bitmapInfo = nullptr;
//	wstring				_bitmapKey;
//	bool				_applyCamera = true;
//	bool				_applyCenter = true;
//};
//
