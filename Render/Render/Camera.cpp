#include "headers.h"
#include "Camera.h"

Camera::Camera(Vector3 eyePos, CD3D11_VIEWPORT viewPort, HWND hwnd) 
{
	// Lets rotate the camera 180 degrees to start off with.
	rotation.y = XM_PI;
	this->eyePos = eyePos;
	//this->lookAt = eyePos+Vector3::Forward;
	this->lookAt = eyePos + Vector3(0,0,1);
	this->upVector =Vector3::Up;
	this->screenCenter.x = viewPort.Width / 2.0f;
	this->screenCenter.y = viewPort.Height / 2.0f;
	this->view= XMMatrixLookAtLH(XMLoadFloat3(&eyePos), XMLoadFloat3(&lookAt), XMLoadFloat3(&upVector));
	this->projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, viewPort.Width / (FLOAT)viewPort.Height, 0.01f, 100.0f);
}
Camera::Camera(Vector3 eyePos,Vector3 lookAt,Vector3 up, CD3D11_VIEWPORT viewPort, HWND hwnd)
{
	// Lets rotate the camera 180 degrees to start off with.
	rotation.y = XM_PI;
	this->eyePos = eyePos;
	this->lookAt = eyePos + lookAt;
	//this->upVector = Vector3(1,0,0);
	this->upVector = up;
	this->screenCenter.x = viewPort.Width / 2.0f;
	this->screenCenter.y = viewPort.Height / 2.0f;
	this->view = XMMatrixLookAtLH(XMLoadFloat3(&eyePos), XMLoadFloat3(&lookAt), XMLoadFloat3(&upVector));
	this->projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, viewPort.Width / (FLOAT)viewPort.Height, 0.01f, 100.0f);
}

Camera::~Camera()
{
}