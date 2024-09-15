#include <Windows.h>
#include <Shlobj.h>
#include <Shobjidl.h>
#include <Propkey.h>
#include <iostream>
#include <string>

#ifndef BHID_ColumnManager
extern "C" const GUID BHID_ColumnManager = { 0xD8EC27BB, 0x3F3B, 0x4042, { 0xB1, 0x4D, 0x34, 0xE2, 0xD0, 0x3B, 0x0C, 0x30 } };
#endif

int wmain(int argc, wchar_t* argv[])
{
    if (argc != 2)
    {
        std::wcout << L"Usage: GetColumns.exe <directory_path>" << std::endl;
        return 1;
    }

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        std::wcout << L"CoInitialize failed" << std::endl;
        return 1;
    }

    IShellItem2* pShellItem = nullptr;
    hr = SHCreateItemFromParsingName(argv[1], NULL, IID_PPV_ARGS(&pShellItem));
    if (FAILED(hr))
    {
        std::wcout << L"Failed to create IShellItem2 for " << argv[1] << std::endl;
        CoUninitialize();
        return 1;
    }

    IColumnManager* pColumnManager = nullptr;
    hr = pShellItem->BindToHandler(NULL, BHID_ColumnManager, IID_PPV_ARGS(&pColumnManager));
    if (FAILED(hr))
    {
        std::wcout << L"Failed to get IColumnManager" << std::endl;
        pShellItem->Release();
        CoUninitialize();
        return 1;
    }

    UINT cColumns = 0;
    hr = pColumnManager->GetColumnCount(CM_ENUM_VISIBLE, &cColumns);
    if (FAILED(hr))
    {
        std::wcout << L"GetColumnCount failed" << std::endl;
        pColumnManager->Release();
        pShellItem->Release();
        CoUninitialize();
        return 1;
    }

    std::unique_ptr<PROPERTYKEY[]> rgColumns(new PROPERTYKEY[cColumns]);
    hr = pColumnManager->GetColumns(CM_ENUM_VISIBLE, rgColumns.get(), cColumns);
    if (FAILED(hr))
    {
        std::wcout << L"GetColumns failed" << std::endl;
        pColumnManager->Release();
        pShellItem->Release();
        CoUninitialize();
        return 1;
    }

    for (UINT i = 0; i < cColumns; i++)
    {
        IPropertyDescription* pPropDesc = nullptr;
        hr = PSGetPropertyDescription(rgColumns[i], IID_PPV_ARGS(&pPropDesc));
        if (SUCCEEDED(hr))
        {
            LPWSTR pszDisplayName = nullptr;
            hr = pPropDesc->GetDisplayName(&pszDisplayName);
            if (SUCCEEDED(hr))
            {
                std::wcout << pszDisplayName << std::endl;
                CoTaskMemFree(pszDisplayName);
            }
            pPropDesc->Release();
        }
    }

    pColumnManager->Release();
    pShellItem->Release();
    CoUninitialize();

    return 0;
}
