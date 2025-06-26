#pragma once

class DataObject
{
public:
	DataObject() {}
	virtual ~DataObject() {}

	virtual wstring GetFileName() abstract;
	virtual void Load(const json& data) abstract;
	virtual void Clear() abstract;

private:

};

std::string Utf8ToAnsi(const std::string& utf8Str);
std::wstring Utf8ToWide(const std::string& utf8Str);
