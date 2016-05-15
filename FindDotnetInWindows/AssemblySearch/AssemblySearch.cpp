#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <cor.h>
#include <stdio.h>
#include <objbase.h>
#include <Shlobj.h>
#include <Shobjidl.h>
#include <Structuredquery.h>
#include <propkey.h> 
#include <algorithm>
class CCoInitialize {
public:
	CCoInitialize() : m_hr(CoInitialize(nullptr)) { }
	~CCoInitialize() { if (SUCCEEDED(m_hr)) CoUninitialize(); }
	operator HRESULT() const { return m_hr; }
	HRESULT m_hr;
};

int main()
{
	CCoInitialize init;
	HRESULT hr = S_OK;

	std::vector<int> searchLocations;
	searchLocations.push_back(CSIDL_WINDOWS);
	searchLocations.push_back(CSIDL_PROGRAM_FILES);
	searchLocations.push_back(CSIDL_PROGRAM_FILESX86);

	std::vector<LPCITEMIDLIST> scopeArray;
	scopeArray.resize(searchLocations.size());
	for (size_t i = 0; i < searchLocations.size(); 
		i++)
	{
		hr = SHGetFolderLocation(nullptr, 
			searchLocations[i], 
			nullptr, 
			0, 
			const_cast<LPITEMIDLIST*>(&scopeArray[i]));
	}
	IShellItemArray* scope = nullptr;
	hr = SHCreateShellItemArrayFromIDLists(
		searchLocations.size(),
		scopeArray.data(), &scope);
	ISearchFolderItemFactory* searchFolderFactory = nullptr;
	hr = CoCreateInstance(CLSID_SearchFolderItemFactory, nullptr, 
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&searchFolderFactory));
	hr = searchFolderFactory->SetScope(scope);
	scope->Release();

	IConditionFactory2* conditionFactory = nullptr;
	hr = CoCreateInstance(CLSID_ConditionFactory, nullptr, 
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&conditionFactory));
	std::vector<std::wstring> extensions;
	extensions.push_back(L".exe");
	extensions.push_back(L".dll");
	size_t arrayLen = extensions.size();
	std::vector<ICondition*> filterConditions(arrayLen);

	for (size_t i = 0; i < arrayLen; i++)
	{
		hr = conditionFactory->CreateStringLeaf(
			PKEY_FileExtension, 
			COP_EQUAL,
			extensions[i].c_str(),
			nullptr,
			CONDITION_CREATION_DEFAULT,
			IID_PPV_ARGS(&filterConditions[i]));
	}
	ICondition* compondFilterCondition = nullptr;
	hr = conditionFactory->CreateCompoundFromArray(
			CT_OR_CONDITION,
			filterConditions.data(),
			static_cast<unsigned long>(filterConditions.size()),
			CONDITION_CREATION_DEFAULT,
			IID_PPV_ARGS(&compondFilterCondition));
	for (auto p : filterConditions)
	{
		p->Release();
	}
	conditionFactory->Release();

	hr = searchFolderFactory->SetCondition(compondFilterCondition);
	compondFilterCondition->Release();
	
	IShellItem* searchFolder = nullptr;
	hr = searchFolderFactory->GetShellItem(IID_PPV_ARGS(&searchFolder));
	searchFolderFactory->Release();

	IEnumShellItems* shellEnumerator = nullptr;
	hr = searchFolder->BindToHandler(nullptr, 
		BHID_EnumItems, IID_PPV_ARGS(&shellEnumerator));
	searchFolder->Release();

	std::vector<IShellItem*> shellItemBuffer;
	shellItemBuffer.resize(100);
	unsigned long fetched = 0;
	IMetaDataDispenser* dispenser = nullptr;
	hr = CoCreateInstance(CLSID_CorMetaDataDispenser, 
		0, CLSCTX_INPROC_SERVER,
		IID_IMetaDataDispenser, (LPVOID*) &dispenser);
	hr = shellEnumerator->Next(shellItemBuffer.size(), 
		shellItemBuffer.data(), &fetched);
	while (fetched > 0)
	{
		for (unsigned long i = 0; i < fetched; i++)
		{
			IShellItem2* shellItem2 = nullptr;
			hr = shellItemBuffer[i]->QueryInterface(
				IID_PPV_ARGS(&shellItem2));
			LPWSTR path = nullptr;
			hr = shellItem2->GetString(PKEY_ParsingPath, &path);
			std::wstring fileName(path);
			::CoTaskMemFree(path);
			shellItem2->Release();
			shellItemBuffer[i]->Release();
			bool skip = false;
			std::transform(fileName.begin(), fileName.end(), 
				fileName.begin(), ::tolower);
			for (auto s : {
				L"managed",L"assembly",L"assemblies",L"microsoft.net",
				L"winsxs",L"appv",L"powershell",L"asp.net",
				L"microsoft help",L"sql server",L"msbuild",L"xbox",
				L"windows kits",L"microsoft sdks",L"silverlight",
				L"iis",L"microsoft web",L"microsoft xde",
				L"visual studio",L"microsoft wcf",L"windows phone",
				L"phone tools",L"microsoft shared",
				L"microsoft analysis",L"microsoft emulator",
				L"windows\\diagnostics",L"xamarin",L"red gate",
				L"mono",L"bluestacks",L"ankhsvn",L"teamviewer",
				L"tencent",L"groupdoc",L"npgsql"
				
			})
			{
				if (fileName.find(s) != std::string::npos)
				{
					skip = true; break;
				}
			}
			if (skip) continue;
			IMetaDataImport* metaDataImport = nullptr;
			if (SUCCEEDED(dispenser->OpenScope(fileName.c_str(), 
				0, IID_IMetaDataImport, 
				reinterpret_cast<LPUNKNOWN*>( &metaDataImport))))
			{
				HCORENUM    enumHandle = 0;
				mdModuleRef   moduleRef[100];
				ULONG       count = sizeof(moduleRef);
				hr = metaDataImport->EnumModuleRefs(&enumHandle,
					moduleRef, count, &count);
				while (count > 0)
				{
					for (ULONG j = 0; j < count; j++)
					{
						wchar_t moduleNameBuf[MAX_PATH];
						ULONG moduleNameLength =
							sizeof(moduleNameBuf);
						hr = metaDataImport->GetModuleRefProps(
							moduleRef[j],
							moduleNameBuf, 
							moduleNameLength, 
							&moduleNameLength);
						std::wstring moduleName(moduleNameBuf);
						std::transform(
							moduleName.begin(), 
							moduleName.end(),
							moduleName.begin(),
							::tolower);
						if (moduleName.find(
							L"presentationframework") 
							!= std::string::npos
							||moduleName.find(
								L"presentationcore")
							!= std::string::npos)
						{
							std::wcout << L"[WPF]";
						}
					}
					hr = metaDataImport->EnumModuleRefs(
						&enumHandle, 
						moduleRef, 
						sizeof(moduleRef), 
						&count);
				}
				std::wcout << fileName << std::endl;
				metaDataImport->CloseEnum(enumHandle);
				metaDataImport->Release();
			}
		}
		hr = shellEnumerator->Next(shellItemBuffer.size(), 
			shellItemBuffer.data(), &fetched);
	}
	dispenser->Release();
	shellEnumerator->Release();
	return 0;
}

