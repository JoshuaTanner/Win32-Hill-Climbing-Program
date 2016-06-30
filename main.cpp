/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	main.cpp
Description	:	main implementation file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

//Library Includes
#include <windows.h>
#include <windowsx.h>
//For detecting memory leaks
#include <crtdbg.h>

//Local Includes
#include "Scene.h"
//Global pointer to scene singleton
Scene* rScene;

#define WINDOW_CLASS_NAME L"Hillclimbing"

/***********************
*WindowProc: handle button presses
* @author: Joshua Tanner
* @parameter: HInstance, HWindow, wParam & lParam
* @return: LReselt
********************/
LRESULT CALLBACK
WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_uiMsg)
	{
	case WM_LBUTTONDOWN:
	{
		//When user presses left mouse button,
		//path is recalculated and scene is updated
		rScene->GetInstance().m_pHillSearcher->BeginHillClimb();
		rScene->GetInstance().Update();
		return(0);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		//When user presses right mouse button, 
		//new positions are generated for the cities
		rScene->GetInstance().Reset();
		return(0);
		break;
	}

	case WM_DESTROY:
	{
		//Singleton instance is destroyed when 
		//window is closed
		rScene->DestroyInstance();
		PostQuitMessage(0);
		return TRUE;
		return(0);
	}
	break;

	default:break;
	}

	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}


/***********************
*CreateAndRegisterWindow: set up the window
* @author: Joshua Tanner
* @parameter: HInstance, window width & height and title of window
* @return: Handle to window
********************/
HWND
CreateAndRegisterWindow(HINSTANCE _hInstance, int _iWidth, int _iHeight, const wchar_t* _pcTitle)
{
	//Defining the window
	HBRUSH WhiteScreen = CreateSolidBrush(RGB(0, 0, 0));
	WNDCLASSEX winclass;
	winclass.hInstance = _hInstance;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, NULL);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = static_cast<HBRUSH> (GetStockObject(WHITE_BRUSH));
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_INFORMATION);//Small icon

	if (!RegisterClassEx(&winclass))
	{
		// Failed to register.
		return (0);
	}

	HWND hwnd;
	hwnd = CreateWindowEx(NULL,
		WINDOW_CLASS_NAME,
		_pcTitle,
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		_iWidth, _iHeight,
		NULL,
		NULL,
		_hInstance,
		NULL);

	if (!hwnd)
	{
		// Failed to create.
		return (0);
	}

	return (hwnd);
}


/***********************
* WinMain: run the program
* @author: Joshua Tanner
* @parameter: HInstance, handle to previous instance, _lpCmdline, _iCmdshow
* @return: int
********************/
int WINAPI
WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdline, int _iCmdshow)
{
	MSG msg;
	RECT _rect;
	ZeroMemory(&msg, sizeof(MSG));
	//Width and height set
	const int kiWidth = 1000;
	const int kiHeight = 800;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Creating window
	HWND hwnd = CreateAndRegisterWindow(_hInstance, kiWidth, kiHeight, L"Hillclimbing - Travelling Salesman Optimum Path by Josh Tanner");

	//Populating global pointer with reference to singleton
	rScene = &Scene::GetInstance();

	GetClientRect(hwnd, &_rect);

	//Check for succesful initialisation
	if (!rScene->Initialise(_hInstance, hwnd, kiWidth, kiHeight))
		return 0;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Output
			rScene->Draw();
		}
	}


	return (static_cast<int>(msg.wParam));
}
