#pragma once
#include "headers.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Camera {
public:
	Camera(Vector3 eyePos, CD3D11_VIEWPORT viewPort, HWND hwnd);
	Camera(Vector3 eyePos, Vector3 lookAt,Vector3 up,CD3D11_VIEWPORT viewPort, HWND hwnd);
	~Camera();
	//void Update(HWND hwnd,float elapsedTime);
	//void HandleInput(HWND hwnd,float elapsedTime);
	Matrix GetView()
	{
		return this->view;
	}
	Matrix GetProjection()
	{
		return this->projection;
	}
	Vector3 GetPostion()
	{
		return this->eyePos;
	}


private:

	Matrix			view;
	Matrix			projection;

	Vector3			eyePos;
	Vector3			lookAt;
	Vector3			upVector = Vector3(0.f, 1.f, 0.f);


	POINT			screenCenter;
	float			moveSpeed = 100.0f;
	Vector2			rotation = Vector2(0.f, 0.f);
	float			rotationSpeed = 0.0025f;
	Matrix			rotationMatrix = XMMatrixIdentity();
};
