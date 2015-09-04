#include "KShader.h"
#include <iostream>
#include "Console/Logging.h"
#include "SharedContext.h"
#include <string>

KShader::KShader(LPCWSTR file_path, const char* entry_point, const char* target, const D3D_SHADER_MACRO* macro_list)
{
	ID3DBlob* errorBlob = nullptr;

	uint32 shader_compile_flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;

#ifdef _DEBUG
	shader_compile_flags = D3DCOMPILE_DEBUG;
#endif

	HRESULT hr = D3DCompileFromFile(file_path, macro_list, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point, target, shader_compile_flags, 0, &m_Blob, &errorBlob);
	if (FAILED(hr))
	{
		if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
			shared_context.log->LogText(LogLevel::FATAL_ERROR, "File not found");
		else
			shared_context.log->LogText(LogLevel::FATAL_ERROR, (char*)errorBlob->GetBufferPointer());
	}
	else
		shared_context.log->LogText(LogLevel::SUCCESS, "Shader successfully compiled: %ls ", file_path);

	D3DReflect(GetBufferPointer(), GetBufferSize(), IID_PPV_ARGS(&m_Reflection));
	/*
	ID3DBlob* outBlob;
	D3DDisassemble(GetBufferPointer(), GetBufferSize(), 0, 0, &outBlob);

	std::wstring out_file_name = std::wstring(file_path) + std::wstring(L".blob");
	D3DWriteBlobToFile(outBlob, out_file_name.c_str(), true);

	outBlob->Release();
	*/
	D3D12_SHADER_DESC shaderDesc;
	m_Reflection->GetDesc(&shaderDesc);

	for (uint32 i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
		m_Reflection->GetInputParameterDesc(i, &paramDesc);
		
		// fill out input element desc
		D3D12_INPUT_ELEMENT_DESC elementDesc;
		ZeroMemory(&elementDesc, sizeof(elementDesc));
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) 
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) 
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) 
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) 
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) 
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) 
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) 
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) 
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) 
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) 
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) 
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) 
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		m_InputElementDesc.push_back(elementDesc);
	}

	//PrintShaderInfo(m_Reflection);

	if (errorBlob)
		errorBlob->Release();
}

KShader::~KShader()
{
	m_Blob->Release();
	m_Reflection->Release();
}

LPVOID KShader::GetBufferPointer()
{
	return m_Blob->GetBufferPointer();
}

SIZE_T KShader::GetBufferSize()
{
	return m_Blob->GetBufferSize();
}

void KShader::PrintShaderInfo(ID3D12ShaderReflection* reflection)
{
	D3D12_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);

	shared_context.log->LogText(LogLevel::PINK_PRINT, "---SHADER INFO---");

	std::string version;
	switch (D3D12_SHVER_GET_TYPE(shaderDesc.Version))
	{
	case D3D11_SHVER_PIXEL_SHADER:
		version = "PIXEL SHADER";
		break;

	case D3D11_SHVER_VERTEX_SHADER:
		version = "VERTEX SHADER";
		break;

	case D3D11_SHVER_GEOMETRY_SHADER:
		version = "GEOMETRY SHADER";
		break;

	case D3D11_SHVER_HULL_SHADER:
		version = "HULL SHADER";
		break;

	case D3D11_SHVER_DOMAIN_SHADER:
		version = "DOMAIN SHADER";
		break;

	case D3D11_SHVER_COMPUTE_SHADER:
		version = "COMPUTE SHADER";
		break;
	}
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Version: %s", version.c_str());
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Creator: %s", shaderDesc.Creator);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Flags: %d", shaderDesc.Flags);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "ConstantBuffers: %d", shaderDesc.ConstantBuffers);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "BoundResources: %d", shaderDesc.BoundResources);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "InputParameters: %d", shaderDesc.InputParameters);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "OutputParameters: %d", shaderDesc.OutputParameters);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "InstructionCount: %d", shaderDesc.InstructionCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  TextureNormalInstructions: %d", shaderDesc.TextureNormalInstructions);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  TextureLoadInstructions: %d", shaderDesc.TextureLoadInstructions);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  TextureCompInstructions: %d", shaderDesc.TextureCompInstructions);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  TextureBiasInstructions: %d", shaderDesc.TextureBiasInstructions);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  TextureGradientInstructions: %d", shaderDesc.TextureGradientInstructions);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  FloatInstructionCount: %d", shaderDesc.FloatInstructionCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  IntInstructionCount: %d", shaderDesc.IntInstructionCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  UintInstructionCount: %d", shaderDesc.UintInstructionCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  MacroInstructionCount: %d", shaderDesc.MacroInstructionCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  ArrayInstructionCount: %d", shaderDesc.ArrayInstructionCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  CutInstructionCount: %d", shaderDesc.CutInstructionCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  EmitInstructionCount: %d", shaderDesc.EmitInstructionCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  cBarrierInstructions: %d", shaderDesc.cBarrierInstructions);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  cInterlockedInstructions: %d", shaderDesc.cInterlockedInstructions);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  cTextureStoreInstructions: %d", shaderDesc.cTextureStoreInstructions);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  Conversion Instruction Count: %d", reflection->GetConversionInstructionCount());
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  Movc Instruction Count: %d", reflection->GetMovcInstructionCount());
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  Mov Instruction Count: %d", reflection->GetMovInstructionCount());
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "  Get Bitwise Instruction Count: %d", reflection->GetBitwiseInstructionCount());

	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "TempRegisterCount: %d", shaderDesc.TempRegisterCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "TempArrayCount: %d", shaderDesc.TempArrayCount);

	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "DefCount: %d", shaderDesc.DefCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "DclCount: %d", shaderDesc.DclCount);

	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "StaticFlowControlCount: %d", shaderDesc.StaticFlowControlCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "DynamicFlowControlCount: %d", shaderDesc.DynamicFlowControlCount);

	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Interface Slots: %d", reflection->GetNumInterfaceSlots());
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "GSMaxOutputVertexCount: %d", shaderDesc.GSMaxOutputVertexCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "PatchConstantParameters: %d", shaderDesc.PatchConstantParameters);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "cGSInstanceCount: %d", shaderDesc.cGSInstanceCount);
	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "cControlPoints: %d", shaderDesc.cControlPoints);

	//shared_context.log->LogText(LogLevel::DEBUG_PRINT, "HSOutputPrimitive: %d", shaderDesc.HSOutputPrimitive);
	//shared_context.log->LogText(LogLevel::DEBUG_PRINT, "HSPartitioning: %d", shaderDesc.HSPartitioning);
	//shared_context.log->LogText(LogLevel::DEBUG_PRINT, "TessellatorDomain: %d", shaderDesc.TessellatorDomain);
	//shared_context.log->LogText(LogLevel::DEBUG_PRINT, "InputPrimitive: %d", shaderDesc.InputPrimitive);
	//shared_context.log->LogText(LogLevel::DEBUG_PRINT, "GSOutputTopology: %d", shaderDesc.GSOutputTopology);

	shared_context.log->LogText(LogLevel::DEBUG_PRINT, "");

	for (uint32 i = 0; i < m_InputElementDesc.size(); i++)
	{
		shared_context.log->LogText(LogLevel::PINK_PRINT, "INPUT PARAMETER: %d", i);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Semantic name: %s", m_InputElementDesc[i].SemanticName);
		switch (m_InputElementDesc[i].Format)
		{
		case DXGI_FORMAT_R32_UINT: 
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32_UINT");
			break;
		case DXGI_FORMAT_R32_SINT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32_SINT");
			break;
		case DXGI_FORMAT_R32_FLOAT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32_FLOAT");
			break;
		case DXGI_FORMAT_R32G32_UINT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32G32_UINT");
			break;
		case DXGI_FORMAT_R32G32_SINT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32G32_SINT");
			break;
		case DXGI_FORMAT_R32G32_FLOAT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32G32_FLOAT");
			break;
		case DXGI_FORMAT_R32G32B32_UINT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32G32B32_UINT");
			break;
		case DXGI_FORMAT_R32G32B32_SINT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32G32B32_SINT");
			break;
		case DXGI_FORMAT_R32G32B32_FLOAT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32G32B32_FLOAT");
			break;
		case DXGI_FORMAT_R32G32B32A32_UINT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32G32B32A32_UINT");
			break;
		case DXGI_FORMAT_R32G32B32A32_SINT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32G32B32A32_SINT");
			break;
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Format: DXGI_FORMAT_R32G32B32A32_FLOAT");
			break;
		default:
			break;
		}
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "");

	}

	for (uint32 i = 0; i < shaderDesc.BoundResources; ++i)
	{
		D3D12_SHADER_INPUT_BIND_DESC bindDesc;
		reflection->GetResourceBindingDesc(i, &bindDesc);
		shared_context.log->LogText(LogLevel::PINK_PRINT, "RESOURCE BINDING: %d", i);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Bind Point: %d", bindDesc.BindPoint);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Bind Count: %d", bindDesc.BindCount);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Name: %s", bindDesc.Name);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "NumSamples: %d", bindDesc.NumSamples);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Space: %d", bindDesc.Space);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Type: %d", bindDesc.Type);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Dimension: %d", bindDesc.Dimension);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "Return type: %d", bindDesc.ReturnType);
		shared_context.log->LogText(LogLevel::DEBUG_PRINT, "");
	}
}

D3D12_INPUT_LAYOUT_DESC KShader::GetInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	inputLayoutDesc.NumElements = (UINT)m_InputElementDesc.size();
	inputLayoutDesc.pInputElementDescs = &m_InputElementDesc[0];
	return inputLayoutDesc;
}

