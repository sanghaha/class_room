#pragma once

class Creature;
class PathFind
{
public:

	void Start(Creature* owner, Cell start, Cell dest, int8 refreshCount, std::function<Cell()> getDestFunc);
	void Update(float deltaTime);
	bool IsComplete();

private:
	void calcPath();

private:
	Creature*					_owner = nullptr;
	std::vector<Cell>			_path;
	Cell						_destCell;
	int8						_checkPathCount = 3;	// 3ĭ���� �����̸� �÷��̾ ���� ���� ã��.
	int8						_currPathCount = 0;
	std::function<Cell()>		_getDestFunc;
};

