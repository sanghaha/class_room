#pragma once


class Sprite;

class AnimSprite
{
public:
	AnimSprite();
	virtual ~AnimSprite();

	void Update(float deltaTime);
	void Render(HDC hdc, Pos pos);

	void SetSprite(Sprite* sprite, float frameTime);
	void SetFrameTime(float time) { _time = time; }
	void SetEnd() { _isEnd = true; }
	bool IsEnd() { return _isEnd; }
	void Reset();

private:
	Sprite*			_sprite = nullptr;
	int32 _indexX = 0;
	int32 _indexY = 0;

	float _time = 0.2f;
	float _sumTime = 0;
	bool _isEnd = false;
};

