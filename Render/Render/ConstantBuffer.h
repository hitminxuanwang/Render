#pragma once
#include "headers.h"

template<typename T> class ConstantBuffer
{
public:

	T Data;

protected:

	ID3D11Buffer* buffer;
	bool initialized;

public:
	ConstantBuffer() : initialized(false)
	{
		ZeroMemory(&Data, sizeof(T));
	}

	ID3D11Buffer* Buffer() const
	{
		return buffer;
	}
	void Initialize(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT32>(sizeof(T) + (16 - (sizeof(T) % 16)));

		//DX::ThrowIfFailed(device->CreateBuffer(&desc, NULL, &buffer));
		device->CreateBuffer(&desc, NULL, &buffer);

		initialized = true;
	}

	void ApplyChanges(ID3D11DeviceContext* deviceContext)
	{
		_ASSERT(initialized);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		//DX::ThrowIfFailed(deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		CopyMemory(mappedResource.pData, &Data, sizeof(T));
		deviceContext->Unmap(buffer, 0);
	}
	void SetVS(ID3D11DeviceContext* deviceContext) const
	{
		deviceContext->VSSetConstantBuffers(0, 1, &buffer);
	}
	void SetPS(ID3D11DeviceContext* deviceContext) const
	{
		deviceContext->PSSetConstantBuffers(0, 1, &buffer);
	}
};