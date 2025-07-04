﻿// Lab5_server.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab5_server.h"

#define MAX_LOADSTRING 100

#define COLOR_MESSAGE L"ColorMessage"

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND hWnd_server;   // handle of server
UINT WM_COLOR;
int color = 0;
bool firstLoad = true;
int drawY = -1;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB5SERVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB5SERVER));
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB5SERVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB5SERVER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 235, 188, nullptr, nullptr, hInstance, nullptr);

    // UINT RegisterWindowMessageA(LPCSTR lpString)
    WM_COLOR = RegisterWindowMessageA((LPCSTR)COLOR_MESSAGE);
    // инициализация handlerа сервера
    hWnd_server = hWnd;
    
    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT r = { 0 }, r1 = { 0 }, r2 = { 0 }, r3 = { 0 };

    if (message == WM_COLOR)
        color = lParam;
    else
    {
        switch (message)
        {
        case WM_LBUTTONDOWN:  
        {
            GetClientRect(hWnd, &r);
            SetRect(&r1, r.left, r.top, r.right, r.bottom / 3);
            SetRect(&r2, r.left, r.bottom / 3, r.right, r.bottom / 3 * 2);
            SetRect(&r3, r.left, r.bottom / 3 * 2, r.right, r.bottom);

            drawY = HIWORD(lParam);
            if (drawY <= r.bottom / 3)
                InvalidateRect(hWnd, &r1, true);
            else if (drawY <= r.bottom / 3 * 2)
                InvalidateRect(hWnd, &r2, true);
            else
                InvalidateRect(hWnd, &r3, true);
        }
        break;
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: Add any drawing code that uses hdc here...
            GetClientRect(hWnd, &r);
            SetRect(&r1, r.left, r.top, r.right, r.bottom / 3);
            SetRect(&r2, r.left, r.bottom / 3, r.right, r.bottom / 3 * 2);
            SetRect(&r3, r.left, r.bottom / 3 * 2, r.right, r.bottom);

            if (firstLoad == true)
            {
                HBRUSH brush = CreateSolidBrush(RGB(225, 255, 255));
                SelectObject(hdc, brush);
                Rectangle(hdc, r.left, r.top, r.right, r.bottom);
                brush = CreateSolidBrush(RGB(225, 0, 0));
                SelectObject(hdc, brush);
                Rectangle(hdc, r1.left, r1.top, r1.right, r1.bottom);
                brush = CreateSolidBrush(RGB(0, 255, 0));
                SelectObject(hdc, brush);
                Rectangle(hdc, r2.left, r2.top, r2.right, r2.bottom);
                brush = CreateSolidBrush(RGB(0, 0, 255));
                SelectObject(hdc, brush);
                Rectangle(hdc, r3.left, r3.top, r3.right, r3.bottom);
                firstLoad = false;
            }
            HBRUSH brush = NULL;
            switch (color)
            {
            case 0:
                // белый
                brush = CreateSolidBrush(RGB(255, 255, 255));
                break;
            case 1:
                // красный
                brush = CreateSolidBrush(RGB(255, 0, 0));
                break;
            case 2:
                // оранжевый
                brush = CreateSolidBrush(RGB(255, 145, 0));
                break;
            case 3:
                // желтый
                brush = CreateSolidBrush(RGB(255, 255, 0));
                break;
            case 4:
                // зеленый
                brush = CreateSolidBrush(RGB(0, 255, 0));
                break;
            case 5:
                // голубой
                brush = CreateSolidBrush(RGB(0, 255, 255));
                break;
            case 6:
                // синий
                brush = CreateSolidBrush(RGB(0, 0, 255));
                break;
            case 7:
                // фиолетовый
                brush = CreateSolidBrush(RGB(204, 0, 255));
                break;
            }
            SelectObject(hdc, brush);
            if (drawY != -1)
            {
                if (drawY <= r.bottom / 3)
                    Rectangle(hdc, r1.left, r1.top, r1.right, r1.bottom);
                else if (drawY <= r.bottom / 3 * 2)
                    Rectangle(hdc, r2.left, r2.top, r2.right, r2.bottom);
                else
                    Rectangle(hdc, r3.left, r3.top, r3.right, r3.bottom);
            }
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


      


