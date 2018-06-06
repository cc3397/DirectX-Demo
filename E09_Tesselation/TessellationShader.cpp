// tessellation shader.cpp
#include "tessellationshader.h"


TessellationShader::TessellationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tessellation_vs.cso", L"tessellation_hs.cso", L"tessellation_ds.cso", L"tessellation_ps.cso");
}


TessellationShader::~TessellationShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	//release tesBuffer
	if (tesBuff)
	{
		tesBuff->Release();
		tesBuff = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void TessellationShader::initShader(WCHAR* vsFilename,  WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC tesBuffer;

	// Load shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	//setup buffer to carry to enable user controlled tesselation
	tesBuffer.Usage = D3D11_USAGE_DYNAMIC;
	tesBuffer.ByteWidth = sizeof(TesBufferType);/////////////
	tesBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tesBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tesBuffer.MiscFlags = 0;
	tesBuffer.StructureByteStride = 0;

	//create the buffer
	renderer->CreateBuffer(&tesBuffer, NULL, &tesBuff);


}

void TessellationShader::initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
}


void TessellationShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* worldHeightTexture, ID3D11ShaderResourceView* waterHeightTexture, ID3D11ShaderResourceView* waterTexture, float terrainHeight, float tessellationFactor, float time)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	TesBufferType* tesPtr;
	//TessellationBufferType* tessellationPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;

	
	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	// Unlock the constant buffer.
	deviceContext->Unmap(matrixBuffer, 0);
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;
	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->DSSetConstantBuffers(bufferNumber, 2, &matrixBuffer);

	//Send the water and land height textures to the domain shader
	deviceContext->DSSetShaderResources(0, 1, &worldHeightTexture);
	deviceContext->DSSetShaderResources(4, 1, &waterHeightTexture);
	deviceContext->DSSetSamplers(0, 1, &sampleState);//point domain shader to sampler

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &worldHeightTexture);
	deviceContext->PSSetShaderResources(4, 1, &waterTexture);

	//send Tes factor the hull/domain shader. Also send to pixel shader for white ontop of waves
	deviceContext->Map(tesBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	tesPtr = (TesBufferType*)mappedResource.pData;
	tesPtr->height = terrainHeight;
	tesPtr->tesFactor = tessellationFactor;
	tesPtr->time = time;

	deviceContext->Unmap(tesBuff, 0);
	bufferNumber = 4;//4 due to cb0 taking up 64bytes of data
	deviceContext->DSSetConstantBuffers(bufferNumber, 2, &tesBuff);
	bufferNumber = 0;
	deviceContext->HSSetConstantBuffers(bufferNumber, 2, &tesBuff);

}

void TessellationShader::render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &sampleState);

	// Base render function.
	BaseShader::render(deviceContext, indexCount);
}



