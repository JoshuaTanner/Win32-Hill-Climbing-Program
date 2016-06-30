/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	scene.h
Description	:	scene header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#pragma once

#include <Windows.h>
//Header includes
#include "resource.h"
#include "HillClimbSearch.h"
#include "BackBuffer.h"

//Line struct for linking cities
struct Line {
	Line() {} //Default constructor
	Line(COLORREF color, int StartX, int StartY, int EndX, int EndY)//Overloaded constructor for ease of use
		:m_color(color), m_iStartX(StartX), m_iStartY(StartY),
		m_iEndX(EndX), m_iEndY(EndY) {}
	COLORREF m_color;
	HPEN m_Pen;
	int m_iStartX;
	int m_iEndX;
	int m_iStartY;
	int m_iEndY;
};

//City struct which holds positions and bitmaps
struct City {
	City() {}//Default constructor
	City(int X, int Y, int W, int H)//Overloaded constructor for ease of use
		:_iX(X), _iY(Y), _iWidth(W), _iHeight(H) {
	}
	~City() {}
	HBITMAP m_hBitmap;//Handle to bitmap
	BITMAP m_Bitmap;//Bitmap
	int _iX;
	int _iY;
	int _iWidth;
	int _iHeight;
};

class Scene
{
	//Member functions
private:
	//Singleton class
	Scene();
	~Scene();
	Scene& operator= (const Scene& _kr);
protected:
public:	
	static Scene& GetInstance();
	bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight);
	void Draw();
	void Update();
	void Reset();
	void DestroyInstance();

	//Member variables
private:
	std::vector<Line*> vecLines;
	std::vector<City*> vecCities;
	HBITMAP m_hBackgroundBitmap;
	BITMAP m_BackgroundBitmap;
protected:
	HINSTANCE m_hAppInstance;
	HWND m_hMainWindow;
	HDC spriteHDC;
	CBackBuffer* m_pBackBuffer;
	static Scene* s_pScene;
public:
	HillClimbSearch* m_pHillSearcher;


};


