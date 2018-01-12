#include "headers.h"
#include "Camera.h"

XMFLOAT3 operator+(XMFLOAT3 left, XMFLOAT3 right)
{
	XMFLOAT3 xmf;
	XMVECTOR xmv = XMVECTOR(XMLoadFloat3(&left)) + XMVECTOR(XMLoadFloat3(&right));
	XMStoreFloat3(&xmf, xmv);
	return xmf;
}

Camera::Camera(Vector3 eyePos, CD3D11_VIEWPORT viewPort, HWND hwnd) 
{
	// Lets rotate the camera 180 degrees to start off with.
	rotation.y = XM_PI;
	this->eyePos = eyePos;
	this->lookAt = eyePos+Vector3::Forward;
	this->upVector =Vector3::Up;
	this->screenCenter.x = viewPort.Width / 2.0f;
	this->screenCenter.y = viewPort.Height / 2.0f;
	this->view= XMMatrixLookAtLH(XMLoadFloat3(&eyePos), XMLoadFloat3(&lookAt), XMLoadFloat3(&upVector));
	this->projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, viewPort.Width / (FLOAT)viewPort.Height, 0.01f, 100.0f);
}
Camera::Camera(Vector3 eyePos,Vector3 lookAt, CD3D11_VIEWPORT viewPort, HWND hwnd)
{
	// Lets rotate the camera 180 degrees to start off with.
	rotation.y = XM_PI;
	this->eyePos = eyePos;
	this->lookAt = eyePos-lookAt;
	this->upVector = Vector3::Up;
	this->screenCenter.x = viewPort.Width / 2.0f;
	this->screenCenter.y = viewPort.Height / 2.0f;
	this->view = XMMatrixLookAtLH(XMLoadFloat3(&eyePos), XMLoadFloat3(&lookAt), XMLoadFloat3(&upVector));
	this->projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, viewPort.Width / (FLOAT)viewPort.Height, 0.01f, 100.0f);
}

Camera::~Camera()
{
}