#pragma once
#pragma once

#include "headers.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

class SnowManEffectFactory : public EffectFactory
{
public:

	SnowManEffectFactory(_In_ ID3D11Device* device) : EffectFactory(device)
	{
		this->directory = L"..\\Media\\";
	}

	virtual void __cdecl CreateTexture(_In_z_ const WCHAR* name, _In_opt_ ID3D11DeviceContext* deviceContext, _Outptr_ ID3D11ShaderResourceView** textureView) override
	{
		WCHAR path[MAX_PATH] = { 0 };

		wcscpy_s(path, directory);
		wcscat_s(path, name);

		EffectFactory::CreateTexture(path, deviceContext, textureView);
	}

private:
	LPCWSTR directory;
};
