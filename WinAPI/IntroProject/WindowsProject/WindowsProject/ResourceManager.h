#pragma once
#include "Singleton.h"

class LineMesh;

class ResourceManager : public Singleton<ResourceManager>
{
public:

	void Init();
	void Clear();

	LineMesh* GetLineMesh(wstring key);

private:
	unordered_map<wstring, LineMesh*> _lineMeshes;
};


