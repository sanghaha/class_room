#pragma once

class DataObject
{
public:
	DataObject() {}
	virtual ~DataObject() {}

	virtual wstring GetFileName() abstract;
	virtual void Load(const json& data) abstract;
};

