#include "scene.h"

#include "gfw.h"

Scene::Scene(Gfw* gfw)
	: mGfw{ gfw },
	mState{ State::kActive }
{

}