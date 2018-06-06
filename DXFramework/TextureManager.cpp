// texture
// Loads and stores a single texture.
// Handles .dds, .png and .jpg (probably).
#include "TextureManager.h"


 //Attempt to load texture. If load fails use default texture.
 //Based on extension, uses slightly different loading function for different image types .dds vs .png/.jpg.
TextureManager::TextureManager(ID3D11Device* ldevice, ID3D11DeviceContext* ldeviceContext)
{
	device = ldevice;
	deviceContext = ldeviceContext;
}

void TextureManager::loadTexture(std::string uid, WCHAR* filename)
{
	HRESULT result;

	// check if file exists
	if (!filename)
	{
		filename = L"../res/DefaultDiffuse.png";
	}
	// if not set default texture
	if (!does_file_exist(filename))
	{
		// change default texture
		filename = L"../res/DefaultDiffuse.png";
	}

	// check file extension for correct loading function.
	std::wstring fn(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
	}

	// Load the texture in.
	if (extension == L"dds")
	{
		result = CreateDDSTextureFromFile(device, deviceContext, filename, NULL, &texture, 0, NULL);
	}
	else
	{
		result = CreateWICTextureFromFile(device, deviceContext, filename, NULL, &texture, 0);
	}
	
	if (FAILED(result))
	{
		MessageBox(NULL, L"Texture loading error", L"ERROR", MB_OK);
	}
	else
	{
		textureMap.insert(std::make_pair(uid, texture));
	}
}

// Release resource.
TextureManager::~TextureManager()
{
	if (texture)
	{
		texture->Release();
		texture = 0;
	}
}

// Return texture as a shader resource.
ID3D11ShaderResourceView* TextureManager::getTexture(std::string uid)
{
	if (textureMap.find(uid) != textureMap.end())
	{
		// texture exists
		return textureMap.at(uid);
	}
	else
	{
		return textureMap.at("default");
	}
}

bool TextureManager::does_file_exist(const WCHAR *fname)
{
	std::ifstream infile(fname);
	return infile.good();
}