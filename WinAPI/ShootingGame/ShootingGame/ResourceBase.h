#pragma once


class ResourceBase
{
public:
	ResourceBase();
	virtual ~ResourceBase();

	virtual Size GetSize() abstract;
	virtual void Update(float deltaTime) {}
};

