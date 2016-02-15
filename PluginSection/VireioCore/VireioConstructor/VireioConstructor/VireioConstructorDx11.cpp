/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Constructor - Vireio Perception Direct 3D Device Handler (DX11)
Copyright (C) 2015 Denis Reischl

File <VireioConstructorDx11.cpp> and
Class <VireioConstructorDx11> :
Copyright (C) 2015 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include"VireioConstructorDx11.h"

#define INTERFACE_ID3D11DEVICE                                               6

#define METHOD_ID3D11DEVICE_CREATEBUFFER                                     3
#define METHOD_ID3D11DEVICE_CREATEVERTEXSHADER                               12
#define METHOD_ID3D11DEVICE_CREATEPIXELSHADER                                15

#define TO_DO_ADD_BOOL_HERE_TRUE                                           true
#define TO_DO_ADD_BOOL_HERE_FALSE                                         false

/**
* Constructor.
***/
VireioConstructorDx11::VireioConstructorDx11() :AQU_Nodus()
{
	// create a new HRESULT pointer
	m_pvReturn = (void*)new HRESULT();

	// nullptr for all dx10/11 input fields
	m_ppvShaderBytecode_VertexShader = nullptr;
	m_pnBytecodeLength_VertexShader = nullptr;
	m_ppcClassLinkage_VertexShader = nullptr;
	m_pppcVertexShader_DX11 = nullptr;
	m_ppvShaderBytecode_PixelShader = nullptr;
	m_pnBytecodeLength_PixelShader = nullptr;
	m_ppcClassLinkage_PixelShader = nullptr;
	m_pppcPixelShader_DX11 = nullptr;
	m_ppcVertexShader_11 = nullptr;
	m_ppcPixelShader_11 = nullptr;
	m_ppsDesc_DX11 = nullptr;
	m_ppsInitialData_DX11 = nullptr;
	m_pppcBuffer_DX11 = nullptr;

	// shader vector initialized in matrix modifier
	m_pasVShaders = nullptr;
	m_pasPShaders = nullptr;
}

/**
* Destructor.
***/
VireioConstructorDx11::~VireioConstructorDx11()
{
}

/**
* Return the name of the  Constructor node.
***/
const char* VireioConstructorDx11::GetNodeType()
{
	return "Vireio Constructor Dx11";
}

/**
* Returns a global unique identifier for the Constructor node.
***/
UINT VireioConstructorDx11::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 32
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Constructor node.
***/
LPWSTR VireioConstructorDx11::GetCategory()
{
	return L"Vireio Core";
}

/**
* Returns a logo to be used for the Constructor node.
***/
HBITMAP VireioConstructorDx11::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"VireioConstructorDx11.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the Constructor node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP VireioConstructorDx11::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR VireioConstructorDx11::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShaderBytecode_VertexShader:
			return L"pShaderBytecode_VS";
		case BytecodeLength_VertexShader:
			return L"BytecodeLength_VS";
		case pClassLinkage_VertexShader:
			return L"pClassLinkage_VertexShader";
		case ppVertexShader_DX11:
			return L"ppVertexShader_DX11";
		case pShaderBytecode_PixelShader:
			return L"pShaderBytecode_PS";
		case BytecodeLength_PixelShader:
			return L"BytecodeLength_PS";
		case pClassLinkage_PixelShader:
			return L"pClassLinkage_PixelShader";
		case ppPixelShader_DX11:
			return L"ppPixelShader_DX11";
		case pDesc_DX11:
			return L"pDesc_DX11";
		case pInitialData_DX11:
			return L"pInitialData_DX11";
		case ppBuffer_DX11:
			return L"ppBuffer_DX11";
		case asVShaderData:
			return L"Vertex Shader Data Array";
		case asPShaderData:
			return L"Pixel Shader Data Array";
	}

	return L"";
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD VireioConstructorDx11::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShaderBytecode_VertexShader:
			return NOD_Plugtype::AQU_PNT_VOID;
		case BytecodeLength_VertexShader:
			return NOD_Plugtype::AQU_SIZE_T;
		case pClassLinkage_VertexShader:
			return NOD_Plugtype::AQU_PNT_ID3D11CLASSLINKAGE;
		case ppVertexShader_DX11:
			return NOD_Plugtype::AQU_PPNT_ID3D11VERTEXSHADER;
		case pShaderBytecode_PixelShader:
			return NOD_Plugtype::AQU_PNT_VOID;
		case BytecodeLength_PixelShader:
			return NOD_Plugtype::AQU_SIZE_T;
		case pClassLinkage_PixelShader:
			return NOD_Plugtype::AQU_PNT_ID3D11CLASSLINKAGE;
		case ppPixelShader_DX11:
			return NOD_Plugtype::AQU_PPNT_ID3D11PIXELSHADER;
		case pDesc_DX11:
			return NOD_Plugtype::AQU_PNT_D3D11_BUFFER_DESC;
		case pInitialData_DX11:
			return NOD_Plugtype::AQU_PNT_D3D11_SUBRESOURCE_DATA;
		case ppBuffer_DX11:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case asVShaderData:
			return NOD_Plugtype::AQU_VOID;
		case asPShaderData:
			return NOD_Plugtype::AQU_VOID;
	}

	return 0;
}

/**
* Sets the input pointer for the requested decommander.
***/
void VireioConstructorDx11::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShaderBytecode_VertexShader:
			m_ppvShaderBytecode_VertexShader = (void**)pData;
			break;
		case BytecodeLength_VertexShader:
			m_pnBytecodeLength_VertexShader = (SIZE_T*)pData;
			break;
		case pClassLinkage_VertexShader:
			m_ppcClassLinkage_VertexShader = (ID3D11ClassLinkage**)pData;
			break;
		case ppVertexShader_DX11:
			m_pppcVertexShader_DX11 = (ID3D11VertexShader***)pData;
			break;
		case pShaderBytecode_PixelShader:
			m_ppvShaderBytecode_PixelShader = (void**)pData;
			break;
		case BytecodeLength_PixelShader:
			m_pnBytecodeLength_PixelShader = (SIZE_T*)pData;
			break;
		case pClassLinkage_PixelShader:
			m_ppcClassLinkage_PixelShader = (ID3D11ClassLinkage**)pData;
			break;
		case ppPixelShader_DX11:
			m_pppcPixelShader_DX11 = (ID3D11PixelShader***)pData;
			break;
		case pDesc_DX11:
			m_ppsDesc_DX11 = (D3D11_BUFFER_DESC**)pData;
			break;
		case pInitialData_DX11:
			m_ppsInitialData_DX11 = (D3D11_SUBRESOURCE_DATA**)pData;
			break;
		case ppBuffer_DX11:
			m_pppcBuffer_DX11 = (ID3D11Buffer***)pData;
			break;
		case asVShaderData:
			m_pasVShaders = (std::vector<Vireio_D3D11_Shader>*)pData;
			break;
		case asPShaderData:
			m_pasPShaders = (std::vector<Vireio_D3D11_Shader>*)pData;
			break;
	}
}

/**
* Constructor supports D3D 11 device calls.
***/
bool VireioConstructorDx11::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_11) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_11_2))
	{
		if (nD3DInterface == INTERFACE_ID3D11DEVICE)
		{
			if ((nD3DMethod == METHOD_ID3D11DEVICE_CREATEVERTEXSHADER) ||
				(nD3DMethod == METHOD_ID3D11DEVICE_CREATEBUFFER) ||
				(nD3DMethod == METHOD_ID3D11DEVICE_CREATEPIXELSHADER))
				return true;
		}
	}
	return false;
}

/**
* Handle Stereo Drawing.
***/
void* VireioConstructorDx11::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	switch (eD3DMethod)
	{
#pragma region ID3D11Device::CreateVertexShader
		case METHOD_ID3D11DEVICE_CREATEVERTEXSHADER:
			// CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader);
			if (!m_pasVShaders) return nullptr;
			if (!pThis) return nullptr;
			if (!m_ppvShaderBytecode_VertexShader) return nullptr;
			if (!m_pnBytecodeLength_VertexShader) return nullptr;
			if (!m_ppcClassLinkage_VertexShader) return nullptr;
			if (!m_pppcVertexShader_DX11) return nullptr;

			{
				// create the shader
				*(HRESULT*)m_pvReturn = ((ID3D11Device*)pThis)->CreateVertexShader(*m_ppvShaderBytecode_VertexShader,
					*m_pnBytecodeLength_VertexShader,
					*m_ppcClassLinkage_VertexShader,
					*m_pppcVertexShader_DX11);

				// call the handling method
				CreateShader(m_pasVShaders,
					*m_ppvShaderBytecode_VertexShader,
					*m_pnBytecodeLength_VertexShader,
					*m_ppcClassLinkage_VertexShader,
					(ID3D11DeviceChild**)*m_pppcVertexShader_DX11, false, 'V');

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
				return m_pvReturn;
			}
#pragma endregion
#pragma region ID3D11Device::CreatePixelShader
		case METHOD_ID3D11DEVICE_CREATEPIXELSHADER:
			// CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader);
			if (!m_pasPShaders) return nullptr;
			if (!pThis) return nullptr;
			if (!m_ppvShaderBytecode_PixelShader) return nullptr;
			if (!m_pnBytecodeLength_PixelShader) return nullptr;
			if (!m_ppcClassLinkage_PixelShader) return nullptr;
			if (!m_pppcPixelShader_DX11) return nullptr;
			
			{
				// create the shader
				*(HRESULT*)m_pvReturn = ((ID3D11Device*)pThis)->CreatePixelShader(*m_ppvShaderBytecode_PixelShader,
					*m_pnBytecodeLength_PixelShader,
					*m_ppcClassLinkage_PixelShader,
					*m_pppcPixelShader_DX11);

				// call the handling method
				CreateShader(m_pasPShaders,
					*m_ppvShaderBytecode_PixelShader,
					*m_pnBytecodeLength_PixelShader,
					*m_ppcClassLinkage_PixelShader,
					(ID3D11DeviceChild**)*m_pppcPixelShader_DX11, false, 'P');

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
				return m_pvReturn;
			}
#pragma endregion
#pragma region ID3D11Device::CreateBuffer
		case METHOD_ID3D11DEVICE_CREATEBUFFER:
			if (!TO_DO_ADD_BOOL_HERE_FALSE) return nullptr; // add option to enable stereo constant buffer creation
			if (!m_ppsDesc_DX11) return nullptr;
			if (!m_ppsInitialData_DX11) return nullptr;
			if (!m_pppcBuffer_DX11) return nullptr;

			// is this a constant buffer ?
			if (!*m_ppsDesc_DX11) return nullptr;
			if (((*m_ppsDesc_DX11)->BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
			{
				// create the buffer
				*(HRESULT*)m_pvReturn = ((ID3D11Device*)pThis)->CreateBuffer(*m_ppsDesc_DX11,
					*m_ppsInitialData_DX11,
					*m_pppcBuffer_DX11);

				// succeeded ?
				if (SUCCEEDED(*(HRESULT*)m_pvReturn))
				{
					// create stereo buffers
					CreateStereoConstantBuffer((ID3D11Device*)pThis, nullptr, (**m_pppcBuffer_DX11), *m_ppsDesc_DX11, *m_ppsInitialData_DX11, false);
				}

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
				return m_pvReturn;
			}
			else return nullptr;
#pragma endregion		
	}
	return nullptr;
}

/**
* Handles CreateVertexShader() and CreatePixelShader() calls.
***/
void VireioConstructorDx11::CreateShader(std::vector<Vireio_D3D11_Shader>* pasShaders, const void *pcShaderBytecode, SIZE_T unBytecodeLength, ID3D11ClassLinkage *pcClassLinkage, ID3D11DeviceChild** ppcShader, bool bOutputCode, char cPrefix)
{
	// get the shader pointer
	ID3D11DeviceChild* pcShader = nullptr;
	if (ppcShader)
	if (*ppcShader)
		pcShader = *ppcShader;
	if (pcShader)
	{
		// get the hash code
		DWORD dwHashCode = GetHashCode((BYTE*)pcShaderBytecode, (DWORD)unBytecodeLength);

		// is this shader already enumerated ?
		for (size_t nShaderDescIndex = 0; nShaderDescIndex < (*pasShaders).size(); nShaderDescIndex++)
		{
			if (dwHashCode == (*pasShaders)[nShaderDescIndex].dwHashCode)
			{
				// create and set private shader data
				Vireio_Shader_Private_Data sPrivateData;
				sPrivateData.dwHash = dwHashCode;
				sPrivateData.dwIndex = (UINT)nShaderDescIndex;

				pcShader->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);
				return;
			}
		}

		// shader data for each shader
		Vireio_D3D11_Shader sShaderData = { 0 };

		// first, disassemble shader
		ID3DBlob* pcIDisassembly = nullptr;
		HRESULT hr = D3DDisassemble(pcShaderBytecode, (DWORD)unBytecodeLength, D3D_DISASM_ENABLE_DEFAULT_VALUE_PRINTS, 0, &pcIDisassembly);

		// get the shader code to a stringstream
		std::stringstream szStream = std::stringstream();
		szStream << static_cast<char*>(pcIDisassembly->GetBufferPointer());
		std::string szLine;

		// get all constant buffers from the stream
		while (std::getline(szStream, szLine))
		{
			if (szLine.find("dcl_constantbuffer") != std::string::npos)
			{
				Vireio_D3D11_Constant_Buffer_Unaccounted sBufferUnaccounted;

				// dcl_constantbuffer cb12[37], immediateIndexed
				std::stringstream szLineStream(szLine);
				std::string szTemp;
				std::getline(szLineStream, szTemp, ' ');
				std::getline(szLineStream, szTemp, '[');
				std::stringstream szIndex(szTemp);
				szIndex.ignore(2);
				szIndex >> sBufferUnaccounted.dwRegister;

				// get size between '[' and ']'
				std::getline(szLineStream, szTemp, ']');
				std::stringstream szSize(szTemp);
				szSize >> sBufferUnaccounted.dwSize;

				// no constant rules addressed at shader creation
				sBufferUnaccounted.nConstantRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;

				// store the unaccounted buffer
				sShaderData.asBuffersUnaccounted.push_back(sBufferUnaccounted);
			}
		}

		// create reflection class
		ID3D11ShaderReflection* pcReflector = NULL;
		if (SUCCEEDED(D3DReflect(pcShaderBytecode,
			unBytecodeLength,
			IID_ID3D11ShaderReflection,
			(void**)&pcReflector)))
		{
			// get desc
			D3D11_SHADER_DESC sDesc;
			pcReflector->GetDesc(&sDesc);

			// fill shader data
			sShaderData.dwConstantBuffers = sDesc.ConstantBuffers;
			sShaderData.dwVersion = sDesc.Version;
			sShaderData.dwBoundResources = sDesc.BoundResources;
			sShaderData.dwInputParameters = sDesc.InputParameters;
			sShaderData.dwOutputParameters = sDesc.OutputParameters;
			sShaderData.dwHashCode = dwHashCode;

			// get name size, max to VIREIO_MAX_VARIABLE_NAME_LENGTH
			UINT dwLen = (UINT)strnlen_s(sDesc.Creator, VIREIO_MAX_VARIABLE_NAME_LENGTH - 1);
			CopyMemory(sShaderData.szCreator, sDesc.Creator, sizeof(CHAR)*dwLen);
			sShaderData.szCreator[dwLen] = 0;

			for (UINT dwIndex = 0; dwIndex < sDesc.ConstantBuffers; dwIndex++)
			{
				// get next constant buffer
				ID3D11ShaderReflectionConstantBuffer* pcConstantBuffer = pcReflector->GetConstantBufferByIndex(dwIndex);
				if (pcConstantBuffer)
				{
					// get desc
					D3D11_SHADER_BUFFER_DESC sDescBuffer;
					pcConstantBuffer->GetDesc(&sDescBuffer);

					// fill buffer data
					Vireio_D3D11_Constant_Buffer sBufferData;
					sBufferData.eType = sDescBuffer.Type;
					sBufferData.dwVariables = sDescBuffer.Variables;
					sBufferData.dwSize = sDescBuffer.Size;
					sBufferData.dwFlags = sDescBuffer.uFlags;

					// get name size, max to VIREIO_MAX_VARIABLE_NAME_LENGTH
					dwLen = (UINT)strnlen_s(sDescBuffer.Name, VIREIO_MAX_VARIABLE_NAME_LENGTH - 1);
					CopyMemory(sBufferData.szName, sDescBuffer.Name, sizeof(CHAR)*dwLen);
					sBufferData.szName[dwLen] = 0;

					// enumerate variables
					for (UINT dwIndexVariable = 0; dwIndexVariable < sDescBuffer.Variables; dwIndexVariable++)
					{
						ID3D11ShaderReflectionVariable* pcVariable = pcConstantBuffer->GetVariableByIndex(dwIndexVariable);
						if (pcVariable)
						{
							// get desc
							D3D11_SHADER_VARIABLE_DESC sDescVariable;
							pcVariable->GetDesc(&sDescVariable);

							// fill variable data
							Vireio_D3D11_Shader_Variable sVariableData;
							sVariableData.dwSize = sDescVariable.Size;
							sVariableData.dwStartOffset = sDescVariable.StartOffset;

							// get name size, max to VIREIO_MAX_VARIABLE_NAME_LENGTH
							dwLen = (UINT)strnlen_s(sDescVariable.Name, VIREIO_MAX_VARIABLE_NAME_LENGTH - 1);
							CopyMemory(sVariableData.szName, sDescVariable.Name, sizeof(CHAR)*dwLen);
							sVariableData.szName[dwLen] = 0;

							// TODO !! FILL DEFAULT VALUE sVariableData.pcDefaultValue (maybe we need this later)

#ifdef _GET_PROJECTION_MATRICES
							// quickly search for projection matrices here
							if (std::strstr(sDescVariable.Name, "roj"))
								OutputDebugStringA(sDescVariable.Name);
#endif
							// and add to buffer desc
							sBufferData.asVariables.push_back(sVariableData);
						}
					}

					// no constant rules addressed at shader creation
					sBufferData.nConstantRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;

					// and add to shader desc
					sShaderData.asBuffers.push_back(sBufferData);
				}
			}

			pcReflector->Release();
		}

		// and add to shader vector
		(*pasShaders).push_back(sShaderData);

		// create and set private shader data
		Vireio_Shader_Private_Data sPrivateData;
		sPrivateData.dwHash = dwHashCode;
		sPrivateData.dwIndex = (UINT)(*pasShaders).size() - 1;

		pcShader->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);

		// output shader code ?
		if (bOutputCode)
		{
			// optionally, output shader code to "VS(hash).txt"
			char buf[32]; ZeroMemory(&buf[0], 32);
			sprintf_s(buf, "VS%u.txt", dwHashCode);
			buf[0] = cPrefix;
			std::ofstream oLogFile(buf, std::ios::ate);

			if (oLogFile.is_open())
			{
				if (SUCCEEDED(hr))
				{
					oLogFile << static_cast<char*>(pcIDisassembly->GetBufferPointer()) << std::endl;
					oLogFile << std::endl << std::endl;
					oLogFile << "// Shader Hash   : " << dwHashCode << std::endl;
				}
				oLogFile << std::endl;
				for (int i = 0; i < (int)sShaderData.asBuffers.size(); i++)
				{
					for (int j = 0; j < (int)sShaderData.asBuffers[i].asVariables.size(); j++)
						oLogFile << "// Shader Constant : Buffer - " << i << " - Variable - " << j << " - " << sShaderData.asBuffers[i].asVariables[j].szName << " " << sShaderData.asBuffers[i].asVariables[j].dwStartOffset << " " << sShaderData.asBuffers[i].asVariables[j].dwSize << std::endl;
				}
				oLogFile.close();
			}
		}
	}
	else OutputDebugString(L"MatrixModifier: Failed to reflect vertex shader !");
}

