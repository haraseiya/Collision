#include "LookdownCamera.h"

LookDownCamera::LookDownCamera(Actor * targetActor)
	:CameraActor(targetActor)
{
	mCameraHeight = 700.0f;
	mCameraBack = 650.0f;
}

LookDownCamera::~LookDownCamera()
{

}

void LookDownCamera::UpdateActor(float deltaTime)
{
	// �ڋߌW��
	const float dumper = 1.5f;
	//�����_
	Vector3 diff;
	Vector3 playerPos = mTargetActor->GetPosition();
	//�����x�N�g������v���[���[�x�N�g���܂ŋ߂Â���
	diff = playerPos - mViewTarget;
	diff = dumper * deltaTime * diff;

	//�����_�E�J�����ʒu�Z�b�g
	mViewTarget = mViewTarget + diff;
	mPosition = mViewTarget;
	mPosition.x -= mCameraBack;
	mPosition.z = mCameraHeight;
}
