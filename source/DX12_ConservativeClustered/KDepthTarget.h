#pragma once
#include "KGraphicsDevice.h"

class KDepthTarget
{
public:
	KDepthTarget();
	~KDepthTarget();

	void CreateDepthTarget(int32 width, int32 height);

	ID3D12Resource* GetResource();

	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUHandle();

	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle();

private:

	ID3D12Resource* m_DTResource;

	D3D12_CPU_DESCRIPTOR_HANDLE m_DSVCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_DSVGPUHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE m_SRVCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_SRVGPUHandle;
};