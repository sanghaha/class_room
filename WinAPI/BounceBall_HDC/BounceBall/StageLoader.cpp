#include "pch.h"
#include "StageLoader.h"
#include "Scene.h"
#include "Background.h"
#include "Ball.h"
#include "Block.h"
#include "Star.h"
#include "Player.h"
#include "PlayerScene.h"

int32 StageLoader::Load(Scene* scene, std::wifstream& file)
{
	int32 maxStartCount = 0;

	// 기존 액터 삭제
	scene->removeAllActor();

	// bg
	{
		Background* bg = new Background(Vector(0, 0));
		scene->ReserveAdd(bg);
	}

	std::wstring line;
	while (std::getline(file, line))
	{
		std::wistringstream iss(line);

		int32 actorType;
		iss >> actorType;

		Actor* actor = nullptr;
		switch (actorType)
		{
		case ActorType::AT_BLOCK: 
			actor = new Block(Vector(0, 0)); 
			break;
		case ActorType::AT_BALL:
		{
			if (dynamic_cast<PlayerScene*>(scene))
			{
				actor = new Player(Vector(0, 0));
			}
			else
			{
				actor = new Ball(Vector(0, 0));
			}
		}
			break;
		case ActorType::AT_STAR:
		{
			actor = new Star(Vector(0, 0));
			maxStartCount++;
		}
			break;
		}

		if (actor)
		{
			actor->Init();
			actor->LoadActor(iss);
			scene->ReserveAdd(actor);
		}
	}

	return maxStartCount;
}
