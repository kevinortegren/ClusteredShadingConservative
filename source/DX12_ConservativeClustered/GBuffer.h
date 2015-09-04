#pragma once
#include <D3D12.h>
#include "KRenderTarget.h"
#include "KDepthTarget.h"

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

	ID3D12PipelineState* GetPSO();
	ID3D12RootSignature* GetRootSig();

	KDepthTarget* GetDepthTarget(int32 swap_index);

	KRenderTarget* GetColorRT(int32 swap_index);
	KRenderTarget* GetNormalRT(int32 swap_index);
	KRenderTarget* GetPositionRT(int32 swap_index);

private:

	ID3D12PipelineState* m_Pso;
	ID3D12RootSignature* m_RootSignature;

	KDepthTarget m_DepthTarget[2];

	KRenderTarget m_ColorRT[2];
	KRenderTarget m_NormalRT[2];
	KRenderTarget m_PositionRT[2];
};