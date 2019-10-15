#include "game.h"
#include "Input.h"
#include "renderer.h"
#include "SceneBase.h"
#include "Actor.h"
#include "Animation.h"
#include "Skeleton.h"
#include <SDL/SDL.h>
#include <typeinfo>
#include <iostream>
#include "CameraActor.h"

Game::Game()
	:mRenderer(nullptr)
	, mNowScene(nullptr)
	, mIsRunning(true)
	, mIsPauseMode(false)
{
}

Game::~Game()
{
}

void Game::Input()
{
	// �I���C�x���g�̃L���b�`
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// �L�[�{�[�h���͍X�V
	INPUT_INSTANCE.Update();

	// ESC�������ꂽ��I��
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_ESCAPE))
	{
		mIsRunning = false;
	}

	// �A�N�^�[�f�o�b�O
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F12))
	{
		ShowActor();
	}

	// �����_�����O���\�[�X�\��
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F11))
	{
		GetRenderer()->ShowResource();
	}

	// �|�[�Y���[�h�ڍs�^����
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_BACKSPACE))
	{
		//�|�[�Y���[�h�ؑ�
		mIsPauseMode = !mIsPauseMode;
		Actor::State changeState;
		if (mIsPauseMode)
		{
			changeState = Actor::EPaused; //�|�[�Y
		}
		else
		{
			changeState = Actor::EActive; //�A�N�e�B�u
		}

		//�S�ẴX�e�[�g��ύX����
		for (auto itr : mActors)
		{
			if (itr->GetState() != Actor::EDead)
			{
				itr->SetState(changeState);
			}
		}
	}
}

void Game::Draw()
{
	if (!mNowScene)
	{
		return;
	}
	mNowScene->draw();
}

int Game::Update()
{
	// 16�~���b(= 60�t���[��/�b)�ɂȂ�悤�ɁA�O�̃t���[�������16�~���b�ȏ�o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// �O�̃t���[�����猻�ݎ����܂ł̌o�ߎ��ԎZ�o(�b�P�ʁj
	mDeltaTime = (SDL_GetTicks() - mTicksCount + 16) / 1000.0f;

	//���̃t���[���̊J�n���ԕۑ��i���̃t���[���܂ł̌o�ߎ��Ԃ��͂��邽�߁j
	mTicksCount = SDL_GetTicks();

	// �t���[�����Ԃ����܂�ɂ��o�߂��Ă���ꍇ��0.05 �� 20fps�Œ�
	if (mDeltaTime > 0.05f)
	{
		mDeltaTime = 0.05f;
	}

	if (!mNowScene)
	{
		return 0;
	}
	SceneBase* tmpScene;

	// �A�N�^�[�̏���
	ActorUpdate();
	// �J�����A�N�^�[�̏���
	if (mActiveCamera != nullptr)
	{
		// �J�����s��v�Z
		mActiveCamera->Update(mDeltaTime);
		// �A�N�e�B�u�J�����̃r���[�s��������Ă���
		mViewMatrix = mActiveCamera->GetViewMatrix();
		GetRenderer()->SetViewMatrix(mViewMatrix);
	}
	
	//�V�[�������s
	tmpScene = mNowScene->update();

	if (tmpScene != mNowScene)
	{
		delete mNowScene;

		//�V�K�V�[�������̃V�[���ɍ����ւ�
		mNowScene = tmpScene;
		return 1;
	}
	return 0;
}

void Game::ActorUpdate()
{
	//�S�ẴA�N�^�[�̍X�V
	for (auto actor : mActors)
	{
		actor->Update(mDeltaTime);
	}

	//�y���f�B���O���̃A�N�^�[���A�N�e�B�u�A�N�^�[�Ɉړ�
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// �S�Ă̎���ł���A�N�^�[���ꎞ�ۊ�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}
	// ����ł���A�N�^�[��delete(mActor���������)
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();
}

void Game::ShowActor()
{
	printf("\n\n<--------------ActorList----------------->\n");
	printf("---------> Active Actor ( %4d ) <-----------\n", mActors.size());

	for (auto i : mActors)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}
	printf("---------> Pending Actor ( %4d ) <-----------\n", mPendingActors.size());
	for (auto i : mPendingActors)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}
}


void Game::Run()
{
	// �����_���[������������Ă��Ȃ������珉����
	if (!mRenderer)
	{
		Initialize(1024,768);
	}
	// �J�n�V�[������`����Ă��Ȃ�������I��
	if (!mNowScene)
	{
		printf("�J�n�V�[�����ݒ肳��Ă��܂���");
		return;
	}

	// �Q�[�����[�v
	while (mIsRunning)
	{
		Input();
		if (Update())
		{
			continue; // �V�[���؂�ւ����̂ݕ`�揈���������Ȃ�����
		}
		Draw();
	}
}

void Game::Shutdown()
{
	// �A�N�^�[�̍폜�@�i�A�N�^�[��ʂ��ăR���|�[�l���g���폜�����j
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	while (!mPendingActors.empty())
	{
		delete mPendingActors.back();
	}
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
}

bool Game::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	// SDL������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("SDL���������s: %s\n", SDL_GetError());
		return false;
	}

	// �����_���[�̍쐬
	mRenderer = new Renderer;
	if (!mRenderer->Initialize(screenWidth, screenHeight, fullScreen))
	{
		printf("Renderer�̏������Ɏ��s");
		delete mRenderer;
		return false;
	}

	mTicksCount = SDL_GetTicks();
	return true;
}

SDL_Renderer* Game::GetSDLRenderer()
{
	return mRenderer->GetSDLRenderer();
}

void Game::AddActor(Actor * actor)
{
	// ��������y���f�B���O�A�N�^�[�ɒǉ�
	mPendingActors.emplace_back(actor);
}

void Game::RemoveActor(Actor * actor)
{
	//�y���f�B���O�A�N�^�[���ɂ���H
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		//�y���f�B���O�A�N�^�[�̍Ō���Ƀf�[�^���ړ����āA�f�[�^������
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
		return;
	}

	// �A�N�e�B�u�A�N�^�[���ɂ���H
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		//�A�N�e�B�u�A�N�^�[�̍Ō���Ƀf�[�^���ړ����āA�f�[�^����
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();

	}
}

void Game::SetCameraActor(CameraActor * camera)
{
	mActiveCamera = camera;
}

void Game::InActiveCamera(CameraActor * inActiveCam)
{
	mActiveCamera = nullptr;
	mViewMatrix = Matrix4::CreateLookAt(Vector3(0, 0, 100),
		                                Vector3(0, 0, 0),
		                                Vector3(0, 0, 1));
}

const Matrix4 & Game::GetViewMatrix()
{
	return mViewMatrix;
}


const Vector3 & Game::GetViewVector()
{
	return mActiveCamera->GetViewVector();
}

