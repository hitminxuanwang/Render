#pragma once
#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0600
#include <SDKDDKVer.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <exception>
#include <d3dcompiler.h>
#include "D3DX11async.h"
//#include "StepTimer.h"

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
//#include "StepTimer.h"

//#define Vector2 XMFLOAT2
//#define Vector3 XMFLOAT3
//#define Matrix	XMMATRIX
//const   Vector3 Up= Vector3(0.f, 1.f, 0.f);
//const 	Vector3 Forward = Vector3(0.f, 0.f, -1.f);
//const   Vector3	Right = Vector3(1.f, 0.f, 0.f);
