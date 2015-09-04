#include "KDescriptorHeap.h"
#include "KGraphicsDevice.h"
#include "SharedContext.h"
#include "Console/Logging.h"
#include "d3dx12.h"

KDescriptorHeap::KDescriptorHeap()
	: m_Size(0), m_DescIncrSize(0), m_Capacity(0)
{

}

KDescriptorHeap::~KDescriptorHeap()
{
	m_DescriptorHeap->Release();
}

HRESULT KDescriptorHeap::CreateDescriptorHeap(uint32 num_descriptors, D3D12_DESCRIPTOR_HEAP_TYPE descriptor_type, D3D12_DESCRIPTOR_HEAP_FLAGS descriptor_flags)
{
	HRESULT hr;

	D3D12_DESCRIPTOR_HEAP_DESC descHeap;
	descHeap.NumDescriptors = num_descriptors;
	descHeap.Type = descriptor_type;
	descHeap.Flags = descriptor_flags;
	descHeap.NodeMask = 0;
	
	hr = shared_context.gfx_device->GetDevice()->CreateDescriptorHeap(&descHeap, IID_PPV_ARGS(&m_DescriptorHeap));
	if (FAILED(hr))
		shared_context.log->LogText(LogLevel::FATAL_ERROR, "Failed to create descriptor heap");

	//Store the increment size for this heap
	m_DescIncrSize = shared_context.gfx_device->GetDevice()->GetDescriptorHandleIncrementSize(descriptor_type);

	//Store the max number of descriptors in this heap
	m_Capacity = num_descriptors;
	
	//Get start handles
	m_CPUHead = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	return hr;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE KDescriptorHeap::GetNewCPUHandle()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE retHandle = m_CPUHead;
	m_CPUHead.Offset(1, m_DescIncrSize);
	++m_Size;
	if(m_Size > m_Capacity)
		shared_context.log->LogText(LogLevel::WARNING, "NO MORE FREE HANDLES");
	return retHandle;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE KDescriptorHeap::GetGPUHandleAtHead()
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(), m_Size - 1, m_DescIncrSize);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE KDescriptorHeap::GetCPUHandleAtHead()
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_Size - 1, m_DescIncrSize);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE KDescriptorHeap::GetCPUHandleAtStart()
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

CD3DX12_GPU_DESCRIPTOR_HANDLE KDescriptorHeap::GetGPUHandleAtStart()
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());
}

CD3DX12_CPU_DESCRIPTOR_HANDLE KDescriptorHeap::GetCPUHandleAt(int32 index)
{
	if ((uint32)index >= m_Size)
		shared_context.log->LogText(LogLevel::WARNING, "GETTING CPU DESCRIPTOR HANDLE OUT OF RANGE");

	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(), index, m_DescIncrSize);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE KDescriptorHeap::GetGPUHandleAt(int32 index)
{
	if((uint32)index >= m_Size)
		shared_context.log->LogText(LogLevel::WARNING, "GETTING GPU DESCRIPTOR HANDLE OUT OF RANGE");

	return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(), index, m_DescIncrSize);
}

uint32 KDescriptorHeap::GetIncrSize()
{
	return m_DescIncrSize;
}

uint32 KDescriptorHeap::GetCapacity()
{
	return m_Capacity;
}

uint32 KDescriptorHeap::GetSize()
{
	return m_Size;
}

ID3D12DescriptorHeap* KDescriptorHeap::GetHeap()
{
	return m_DescriptorHeap;
}

