#include "headers.h"
#include "VertexShader.h"


VertexShader::VertexShader() 
{
}

VertexShader::VertexShader(ID3D11VertexShader* shader, ID3DBlob *shaderBlob)
{
	this->shader.reset(shader);
	this->shaderBlob.reset(shaderBlob);
}
std::unique_ptr<VertexShader> VertexShader::CompileShader(ID3D11Device *device, LPCWSTR filename)
{
	ID3DBlob* shaderBlob = NULL;
	ID3D11VertexShader* shader = NULL;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* pErrorBlob;
	HRESULT hr=D3DX11CompileFromFile(filename, NULL, NULL, "main", "vs_5_0",
		dwShaderFlags, 0, NULL, &shaderBlob , &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
		{
			std::ofstream out("vs_debug.txt");
			out << (char*)pErrorBlob->GetBufferPointer();
			out.close();
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		}
		if (pErrorBlob) pErrorBlob->Release();
		
	}
	if (pErrorBlob) pErrorBlob->Release();
	device->CreateVertexShader(shaderBlob->GetBufferPointer(),shaderBlob->GetBufferSize(),NULL,&shader);
	return std::unique_ptr<VertexShader>(new VertexShader(shader, shaderBlob));
}
void VertexShader::Set(ID3D11DeviceContext *context)
{
	context->VSSetShader(this->shader.get(),NULL,0);
}
VertexShader::~VertexShader()
{
}