#pragma once
#include "headers.h"
class VertexShader
{
public:
	VertexShader();
	VertexShader(ID3D11VertexShader *shader, ID3DBlob* shaderBlob);
	~VertexShader();
	void Set(ID3D11DeviceContext* context);
	static std::unique_ptr<VertexShader> CompileShader(ID3D11Device* device, LPCWSTR filename);
	ID3D11VertexShader *GetShader()
	{
		return shader.get();
	}
	ID3DBlob* GetShaderBlob()
	{
		return shaderBlob.get();
	}
private:
	std::unique_ptr<ID3D11VertexShader> shader;
	std::unique_ptr<ID3DBlob> shaderBlob;
};
