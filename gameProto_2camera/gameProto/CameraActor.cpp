#include "Game.h"
#include "CameraActor.h"

CameraActor::CameraActor(Actor *targetActor)
	:mTargetActor(targetActor)
{
	mPosition = Vector3(0, 100, 100);
	//�f�t�H���g�J�����l�Z�b�g
	mViewMatrix = Matrix4::CreateLookAt(Vector3(-100, 0, 100),
		                                Vector3(0, 0, 0),
		                                Vector3(0, 0, 1));
	//�Q�[���V�X�e�����ɂ��ꂪ�A�N�e�B�u�J�����ł��邱�Ƃ�ʒm
	SetActive();
}

CameraActor::~CameraActor()
{
	GAMEINSTANCE.InActiveCamera(this);
}

void CameraActor::UpdateActor(float deltaTime)
{
}

void CameraActor::Update(float deltaTime)
{
	UpdateActor(deltaTime);

	mViewVector = mViewTarget - mPosition;
	mViewMatrix = Matrix4::CreateLookAt(mPosition, mViewTarget, Vector3(0, 0, 1));
}

void CameraActor::SetActive()
{
	GAMEINSTANCE.SetCameraActor(this);
}

//�ʒu����������
void CameraActor::Init(const Vector3& cameraPos, const Vector3& targetPos, const Vector3 &upVec)
{
	mPosition = cameraPos;
	mViewTarget = targetPos;
	mViewVector = targetPos - cameraPos;

	mViewMatrix = Matrix4::CreateLookAt(cameraPos, targetPos, upVec);
}