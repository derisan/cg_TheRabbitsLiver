#include "loading_scene.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <rapidjson/document.h>

#include "gfw.h"
#include "renderer.h"
#include "actor.h"
#include "sprite_component.h"
#include "sound_engine.h"
#include "shader.h"

LoadingScene::LoadingScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr },
	mMeshIdx{ 0 },
	mSoundIdx{ 0 },
	mImgIdx{ 0 },
	mCurIdx{ 0 },
	mTotalFiles{ 0 },
	mElapsed{ 0.0f }
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");
}

void LoadingScene::Enter()
{
	if (!ReadFromJson("assets.json"))
	{
		std::cout << "Failed to read assets from json" << std::endl;
		mGfw->PopScene();
	}

	mTotalFiles = mMeshFiles.size() + mSoundFiles.size() + mImgFiles.size();
}

void LoadingScene::Exit()
{
	mGfw->RemoveAll();
}

void LoadingScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->PopScene();
}

void LoadingScene::Update()
{
	mElapsed += mGfw->dt;

	for (; mCurIdx < mMeshIdx + mSoundIdx + mImgIdx; ++mCurIdx)
	{
		auto bar = new Actor{ mGfw, mGfw->kSprite };
		auto sc = new SpriteComponent{ bar, "Assets/white.png" };
		bar->SetScale(1.8f / mTotalFiles);
		bar->SetPosition(glm::vec3{ -0.9f + (1.8f / mTotalFiles) * mCurIdx, -0.2f, 0.0f });
	}

	if (mMeshIdx < mMeshFiles.size())
	{
		mRenderer->GetMesh(mMeshFiles[mMeshIdx]);
		std::cout << "Read mesh: " << mMeshFiles[mMeshIdx++] << std::endl;
	}
	else if (mSoundIdx < mSoundFiles.size())
	{
		SoundEngine::Get()->Create("Assets/" + mSoundFiles[mSoundIdx], mSoundFiles[mSoundIdx]);
		std::cout << "Create music: " << mSoundFiles[mSoundIdx++] << std::endl;
	}
	else if (mImgIdx < mImgFiles.size())
	{
		mRenderer->GetTexture(mImgFiles[mImgIdx]);
		std::cout << "Read texture: " << mImgFiles[mImgIdx++] << std::endl;
	}
	else if (mElapsed > 3.0f)
		mGfw->ChangeScene("title");
}

void LoadingScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mSpriteShader->SetActive();
	auto sprites = mGfw->GetSprites();
	for (auto sprite : sprites)
		sprite->Draw(mSpriteShader);
		
	glutSwapBuffers();
}

bool LoadingScene::ReadFromJson(const std::string& file)
{
	std::ifstream f{ file };
	if (!f.is_open())
	{
		std::cout << "Failed to open file " << file << std::endl;
		return false;
	}

	std::stringstream fileStream;
	fileStream << f.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		std::cout << file << " is not valid json" << std::endl;
		return false;
	}

	// gpmesh file
	const rapidjson::Value& meshes = doc["gpmesh"];
	if (!meshes.IsArray() || meshes.Size() < 1)
	{
		std::cout << file << " has no gpmesh." << std::endl;
		return false;
	}

	for (rapidjson::SizeType i = 0; i < meshes.Size(); i++)
	{
		std::string meshName = meshes[i].GetString();
		mMeshFiles.push_back("Assets/" + meshName);
	}

	// wav file
	const rapidjson::Value& wavs = doc["wav"];
	if (!wavs.IsArray() || wavs.Size() < 1)
	{
		std::cout << file << " has no wav." << std::endl;
		return false;
	}

	for (rapidjson::SizeType i = 0; i < wavs.Size(); i++)
	{
		std::string soundName = wavs[i].GetString();
		mSoundFiles.push_back(soundName);
	}

	// mp3 file
	const rapidjson::Value& mp3s = doc["mp3"];
	if (!mp3s.IsArray() || mp3s.Size() < 1)
	{
		std::cout << file << " has no mp3." << std::endl;
		return false;
	}

	for (rapidjson::SizeType i = 0; i < mp3s.Size(); i++)
	{
		std::string soundName = mp3s[i].GetString();
		mSoundFiles.push_back(soundName);
	}

	// png
	const rapidjson::Value& pngs = doc["png"];
	if (!pngs.IsArray() || pngs.Size() < 1)
	{
		std::cout << file << " is unknown png file." << std::endl;
		return false;
	}

	for (rapidjson::SizeType i = 0; i < pngs.Size(); i++)
	{
		std::string png = pngs[i].GetString();
		mImgFiles.push_back("Assets/" + png);
	}

	// jpg
	const rapidjson::Value& jpgs = doc["jpg"];
	if (!jpgs.IsArray() || jpgs.Size() < 1)
	{
		std::cout << file << " is unknown jpg file." << std::endl;
		return false;
	}

	for (rapidjson::SizeType i = 0; i < jpgs.Size(); i++)
	{
		std::string jpg = jpgs[i].GetString();
		mImgFiles.push_back("Assets/" + jpg);
	}

	return true;
}