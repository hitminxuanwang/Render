#pragma once
class PixelShader
{
public:
	PixelShader();
	PixelShader(ID3D11PixelShader* shader, ID3DBlob *shaderBlob);
	~PixelShader();
	void Set(ID3D11DeviceContext *context);

	static std::unique_ptr<PixelShader> CompileShader(ID3D11Device* device, LPCWSTR filename);
	ID3D11PixelShader* GetShader()
	{
		return shader.get();
	}
	ID3DBlob* GetShaderBlob()
	{
		return shaderBlob.get();
	}
private:
	std::unique_ptr<ID3D11PixelShader> shader;
	std::unique_ptr<ID3DBlob> shaderBlob;
};