#pragma once
#include "KGraphicsDevice.h"

class Texture
{
public:
	Texture();
	~Texture();

	void CreateTextureFromFile(ID3D12GraphicsCommandList* gfx_command_list, const char* file);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle();
	void DeleteUploadTexture();
private:
	ID3D12Resource* m_texture;
	ID3D12Resource* m_uploadTexture;

	D3D12_GPU_DESCRIPTOR_HANDLE m_GPUhandle;
};