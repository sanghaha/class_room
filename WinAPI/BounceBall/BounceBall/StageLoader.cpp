#include "pch.h"
#include "StageLoader.h"
#include "Scene.h"
#include "Background.h"
#include "Ball.h"
#include "Block.h"
#include "Star.h"


void StageLoader::Load(Scene* scene, std::wifstream& file)
{
	// 모든 액터 지우고
	scene->removeAllActor();

	// bg
	{
		Background* bg = new Background(Vector(0, 0));
		scene->addActor(bg);
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
		case ActorType::AT_BLOCK: actor = new Block(Vector(0, 0)); break;
		case ActorType::AT_BALL: actor = new Ball(Vector(0, 0)); break;
		case ActorType::AT_STAR: actor = new Star(Vector(0, 0)); break;
		}

		if (actor)
		{
			scene->addActor(actor);
			actor->LoadActor(iss);
		}
	}
}
