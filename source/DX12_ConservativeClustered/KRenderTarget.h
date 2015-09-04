#pragma once
#include "KGraphicsDevice.h"

class KRenderTarget
{
public:
	KRenderTarget();
	~KRenderTarget();

	void CreateRenderTarget(DXGI_FORMAT tex_format, int32 width, int32 height, const float clear_color[4] = 0);
	void CreateRenderTargetArray(uint32 num_slices, DXGI_FORMAT tex_format, int32 width, int32 height, const  float clear_color[4] = 0);

	ID3D12Resource* GetResource();

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUHandle();

	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle();

private:

	ID3D12Resource* m_RTResource;

	D3D12_CPU_DESCRIPTOR_HANDLE m_RTVCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_RTVGPUHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE m_SRVCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_SRVGPUHandle;
};