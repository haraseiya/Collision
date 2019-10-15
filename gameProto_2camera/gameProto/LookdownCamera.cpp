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
	// 接近係数
	const float dumper = 1.5f;
	//注視点
	Vector3 diff;
	Vector3 playerPos = mTargetActor->GetPosition();
	//差分ベクトルからプレーヤーベクトルまで近づける
	diff = playerPos - mViewTarget;
	diff = dumper * deltaTime * diff;

	//注視点・カメラ位置セット
	mViewTarget = mViewTarget + diff;
	mPosition = mViewTarget;
	mPosition.x -= mCameraBack;
	mPosition.z = mCameraHeight;
}
