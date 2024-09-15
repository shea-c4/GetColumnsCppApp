#include <Windows.h>
#include <Shlobj.h>
#include <Shobjidl.h>
#include <Propkey.h>
#include <iostream>
#include <string>

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

    // Create hidden window
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = DefWindowProcW;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"HiddenWindowClass";

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(wc.lpszClassName, L"Hidden Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, wc.hInstance, NULL);

    if (!hwnd)
    {
        std::wcout << L"Failed to create hidden window" << std::endl;
        CoUninitialize();
        return 1;
    }

    // Get IShellFolder for the directory
    IShellFolder* pDesktopFolder = NULL;
    hr = SHGetDesktopFolder(&pDesktopFolder);
    if (FAILED(hr))
    {
        std::wcout << L"SHGetDesktopFolder failed" << std::endl;
        DestroyWindow(hwnd);
        CoUninitialize();
        return 1;
    }

    LPITEMIDLIST pidlFolder = NULL;
    hr = SHParseDisplayName(argv[1], NULL, &pidlFolder, 0, NULL);
    if (FAILED(hr))
    {
        std::wcout << L"SHParseDisplayName failed for " << argv[1] << std::endl;
        pDesktopFolder->Release();
        DestroyWindow(hwnd);
        CoUninitialize();
        return 1;
    }

    IShellFolder* pFolder = NULL;
    hr = pDesktopFolder->BindToObject(pidlFolder, NULL, IID_PPV_ARGS(&pFolder));
    if (FAILED(hr))
    {
        std::wcout << L"BindToObject failed" << std::endl;
        CoTaskMemFree(pidlFolder);
        pDesktopFolder->Release();
        DestroyWindow(hwnd);
        CoUninitialize();
        return 1;
    }

    // Create IShellView
    IShellView* pShellView = NULL;
    hr = pFolder->CreateViewObject(hwnd, IID_PPV_ARGS(&pShellView));
    if (FAILED(hr))
    {
        std::wcout << L"CreateViewObject failed" << std::endl;
        pFolder->Release();
        CoTaskMemFree(pidlFolder);
        pDesktopFolder->Release();
        DestroyWindow(hwnd);
        CoUninitialize();
        return 1;
    }

    // Get IFolderView2
    IFolderView2* pFolderView2 = NULL;
    hr = pShellView->QueryInterface(IID_PPV_ARGS(&pFolderView2));
    if (FAILED(hr))
    {
        std::wcout << L"QueryInterface for IFolderView2 failed" << std::endl;
        pShellView->Release();
        pFolder->Release();
        CoTaskMemFree(pidlFolder);
        pDesktopFolder->Release();
        DestroyWindow(hwnd);
        CoUninitialize();
        return 1;
    }

    // Get IColumnManager
    IColumnManager* pColumnManager = NULL;
    hr = pFolderView2->QueryInterface(IID_PPV_ARGS(&pColumnManager));
    if (FAILED(hr))
    {
        std::wcout << L"QueryInterface for IColumnManager failed" << std::endl;
        pFolderView2->Release();
        pShellView->Release();
        pFolder->Release();
        CoTaskMemFree(pidlFolder);
        pDesktopFolder->Release();
        DestroyWindow(hwnd);
        CoUninitialize();
        return 1;
    }

    UINT cColumns = 0;
    hr = pColumnManager->GetColumnCount(CM_ENUM_VISIBLE, &cColumns);
    if (FAILED(hr))
    {
        std::wcout << L"GetColumnCount failed" << std::endl;
        pColumnManager->Release();
        pFolderView2->Release();
        pShellView->Release();
        pFolder->Release();
        CoTaskMemFree(pidlFolder);
        pDesktopFolder->Release();
        DestroyWindow(hwnd);
        CoUninitialize();
        return 1;
    }

    PROPERTYKEY* rgColumns = new PROPERTYKEY[cColumns];
    hr = pColumnManager->GetColumns(CM_ENUM_VISIBLE, rgColumns, cColumns);
    if (FAILED(hr))
    {
        std::wcout << L"GetColumns failed" << std::endl;
        delete[] rgColumns;
        pColumnManager->Release();
        pFolderView2->Release();
        pShellView->Release();
        pFolder->Release();
        CoTaskMemFree(pidlFolder);
        pDesktopFolder->Release();
        DestroyWindow(hwnd);
        CoUninitialize();
        return 1;
    }

    for (UINT i = 0; i < cColumns; i++)
    {
        IPropertyDescription* pPropDesc = NULL;
        hr = PSGetPropertyDescription(rgColumns[i], IID_PPV_ARGS(&pPropDesc));
        if (SUCCEEDED(hr))
        {
            LPWSTR pszDisplayName = NULL;
            hr = pPropDesc->GetDisplayName(&pszDisplayName);
            if (SUCCEEDED(hr))
            {
                std::wcout << pszDisplayName << std::endl;
                CoTaskMemFree(pszDisplayName);
            }
            pPropDesc->Release();
        }
    }

    delete[] rgColumns;
    pColumnManager->Release();
    pFolderView2->Release();
    pShellView->Release();
    pFolder->Release();
    CoTaskMemFree(pidlFolder);
    pDesktopFolder->Release();
    DestroyWindow(hwnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    CoUninitialize();

    return 0;
}
