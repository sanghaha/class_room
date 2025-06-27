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
	int8						_checkPathCount = 3;	// 3칸정도 움직이면 플레이어를 향해 길을 찾자.
	int8						_currPathCount = 0;
	std::function<Cell()>		_getDestFunc;
};

