#pragma once
#include <D3D12.h>
#include "Types.h"
#include "d3dx12.h"

class KDescriptorHeap
{
public:
	KDescriptorHeap();
	~KDescriptorHeap();

	HRESULT CreateDescriptorHeap(uint32 num_descriptors, D3D12_DESCRIPTOR_HEAP_TYPE descriptor_type, D3D12_DESCRIPTOR_HEAP_FLAGS descriptor_flags);
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetNewCPUHandle();

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUHandleAtHead();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUHandleAtHead();

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUHandleAtStart();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUHandleAtStart();

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUHandleAt(int32 index);
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUHandleAt(int32 index);

	uint32 GetIncrSize();
	uint32 GetCapacity();
	uint32 GetSize();

	ID3D12DescriptorHeap* GetHeap();

private:

	ID3D12DescriptorHeap* m_DescriptorHeap;

	CD3DX12_CPU_DESCRIPTOR_HANDLE m_CPUHead;

	uint32 m_DescIncrSize;
	uint32 m_Capacity;
	uint32 m_Size;

};