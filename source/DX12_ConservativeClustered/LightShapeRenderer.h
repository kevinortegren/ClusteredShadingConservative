#pragma once
#include "KGraphicsDevice.h"

class LightShapeRenderer
{
public:
	LightShapeRenderer();
	~LightShapeRenderer();

	ID3D12PipelineState* GetPSO();
	ID3D12PipelineState* GetSpotPSO();
	ID3D12RootSignature* GetRootSig();

private:
	ID3D12PipelineState* m_PSO;
	ID3D12PipelineState* m_SpotPSO;
	ID3D12RootSignature* m_RootSignature;
};