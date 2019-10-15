#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "PlayerActor.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"

const float cAnimationSpeed = 0.5f;

PlayerActor::PlayerActor()
	:mbWait(true)
	,mAnimState(AnimationType::PLAYERANIM_IDLE)
{
	//���b�V���̃��[�h
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(RENDERER->GetMesh("Assets/SK_Mannequin.gpmesh"));
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/SK_Mannequin.gpskel"));
	
	//�A�j���[�V�����̎擾&�A�j���[�V�����z��ɃZ�b�g
	mAnimTypes.resize(AnimationType::PLAYERANIM_ITEMNUM);
	mAnimTypes[PLAYERANIM_IDLE] = RENDERER->GetAnimation("Assets/ThirdPersonIdle.gpanim");
	mAnimTypes[PLAYERANIM_RUN]  = RENDERER->GetAnimation("Assets/ThirdPersonRun.gpanim");

	// �A�C�h����ԃA�j���[�V�������Z�b�g
	mMeshComp->PlayAnimation( mAnimTypes[PLAYERANIM_IDLE], cAnimationSpeed);

	printf("PlayerActor�쐬 id:[%5d] this : (0x%p)\n", mID, this);
}

PlayerActor::~PlayerActor()
{
	mAnimTypes.clear(); //�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerActor::UpdateActor(float deltaTime)
{

	//�L������ԑJ��
	StateChange();

	//�L��������
	const float speed = 200.0f;
	Vector3 forwardVec = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 rightVec   = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = GetForward();

	// �L�����N�^�[�ړ�
	Vector3 DirVec(0.0f, 0.0f, 0.0f);
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_UP))
	{
		DirVec += forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_DOWN))
	{
		DirVec -= forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_RIGHT))
	{
		DirVec += rightVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LEFT))
	{
		DirVec -= rightVec;
	}

	// �i�s�����i�ރx�N�g�����������Ȃ�L�[�͉�����Ă��Ȃ��B�ȉ��̈ړ��������L�����Z��
	if (DirVec.LengthSq() < 0.1f)
	{
		return;
	}
	
	// �����L�[����
	charaForwardVec = DirVec ;
	
	// �i�s�����Ɍ����ĉ�]
	charaForwardVec.Normalize();
	RotateToNewForward(charaForwardVec);

	// �i�s�����Ɉړ�
	mPosition += speed * deltaTime * DirVec;
}

// �L������ԑJ��
void PlayerActor::StateChange()
{
	// �L�[���͂���A�j���[�V������Ԃֈڍs
	bool IsRunStart  = INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_UP) | 
		               INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_RIGHT) |
		               INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_DOWN) |
		               INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_LEFT);
	bool IsIdleStart = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_UP) &
		               INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_RIGHT) &
		               INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_DOWN) & 
		               INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_LEFT);

	// �ҋ@�A�j���J�n
	if (IsIdleStart && (mAnimState != PLAYERANIM_IDLE))
	{
		mMeshComp->PlayAnimation(mAnimTypes[PLAYERANIM_IDLE], cAnimationSpeed);
		mAnimState = PLAYERANIM_IDLE;
		return;
	}

	// RUN�A�j���J�n
	if (IsRunStart && (mAnimState != PLAYERANIM_RUN))
	{
		mMeshComp->PlayAnimation(mAnimTypes[PLAYERANIM_RUN], cAnimationSpeed);
		mAnimState = PLAYERANIM_RUN;
		return;
	}

}
