#include "headers.h"
#include "PixelShader.h"

PixelShader::PixelShader()
{
}

PixelShader::PixelShader(ID3D11PixelShader* shader, ID3DBlob *shaderBlob)
{
	this->shader.reset(shader);
	this->shaderBlob.reset(shaderBlob);
}
std::unique_ptr<PixelShader> PixelShader::CompileShader(ID3D11Device *device, LPCWSTR filename)
{
	ID3DBlob* shaderBlob = NULL;
	ID3D11PixelShader* shader = NULL;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* pErrorBlob;
	HRESULT hr = D3DX11CompileFromFile(filename, NULL, NULL, "main", "ps_4_0",
		dwShaderFlags, 0, NULL, &shaderBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
		{
			std::ofstream out("ps_debug.txt");
			out << (char*)pErrorBlob->GetBufferPointer();
			out.close();
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		}
		if (pErrorBlob) pErrorBlob->Release();

	}
	if (pErrorBlob) pErrorBlob->Release();
	device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
	return std::unique_ptr<PixelShader>(new PixelShader(shader, shaderBlob));
}
void PixelShader::Set(ID3D11DeviceContext *context)
{
	context->PSSetShader(this->shader.get(), NULL, 0);
}
PixelShader::~PixelShader()
{
}