/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	scene.cpp
Description	:	scene implementation file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#include "Scene.h"
#include <time.h>
Scene* Scene::s_pScene = 0;

/***********************
* Scene constructor: constructs scene
* @author: Joshua Tanner
* @parameter: none
* @return: 
********************/
Scene::Scene()
	:m_hAppInstance(0),
	m_pHillSearcher(0)
{


}

/***********************
* Scene destructor: destroy scene and deallocate memory
* @author: Joshua Tanner
* @parameter: none
* @return: 
********************/
Scene::~Scene()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_pHillSearcher;
	m_pHillSearcher = 0;

	for (unsigned int i = 0; i < vecCities.size(); i++)
	{
		delete  vecCities[i];
		vecCities[i] = 0;
	}

	for (unsigned int i = 0; i < vecLines.size(); i++)
	{
		delete  vecLines[i];
		vecLines[i] = 0;
	}
}

/***********************
* GetInstance: return the singleton instance of scene
* @author: Joshua Tanner
* @parameter: none
* @return: Scene pointer
********************/
Scene& Scene::GetInstance()
{
	if (s_pScene == 0)
	{
		s_pScene = new Scene();
	}
	return (*s_pScene);
}

/***********************
* Scene Initialise: generate scene components
* @author: Joshua Tanner
* @parameter: HInstance, HWindow, width & height of window
* @return: bool
********************/
bool Scene::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight)
{
	//Storing handles to instance and window
	m_hAppInstance = _hInstance; 
	m_hMainWindow = _hWnd;

	m_pHillSearcher = new HillClimbSearch();
	m_pBackBuffer = new CBackBuffer;
	m_pBackBuffer->Initialise(_hWnd, _iWidth, _iHeight);

	HINSTANCE hInstance = m_hAppInstance;
	if (!spriteHDC)
		spriteHDC = CreateCompatibleDC(NULL); //HDC used for all sprites

	srand(unsigned int(time(NULL))); //seeding on time

	m_hBackgroundBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BACKGROUND));
	GetObject(m_hBackgroundBitmap, sizeof(BITMAP), &m_BackgroundBitmap);

	//Generating 8 cities for the scene
	//X, Y positions are randomised
	for (int i = 0; i < 8; i++)
	{
		int X = (rand() % 800) + 100;
		int Y = (rand() % 400) + 200;
		City* newCity = new City(X, Y, 25, 25);
		vecCities.push_back(newCity);//Push the city into scene for rendering 
		Node* newNode = new Node(X, Y);
		m_pHillSearcher->Cities.push_back(newNode);//Push the node into HillClimber 
		m_pHillSearcher->BestDistance = m_pHillSearcher->VisitAllNodes();
	}

	//Generating lines to link the cities
	for (int i = 0; i < vecCities.size(); i++)
	{
		if (i == vecCities.size() - 1)//To link the last city to the first city
		{
			Line* newLine = new Line(RGB(255, 255,255), vecCities[i]->_iX, vecCities[i]->_iY, vecCities[0]->_iX, vecCities[0]->_iY);
			vecLines.push_back(newLine);
		}
		else
		{
			Line* newLine = new Line(RGB(255, 255, 255), vecCities[i]->_iX, vecCities[i]->_iY, vecCities[i + 1]->_iX, vecCities[i + 1]->_iY);
			vecLines.push_back(newLine);
		}
	}	

	for (unsigned int i = 0; i < vecCities.size(); i++)//Loading the bitmap
	{
		vecCities[i]->m_hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_CITY));
		GetObject(vecCities[i]->m_hBitmap, sizeof(BITMAP), &vecCities[i]->m_Bitmap);
	}

	return true;
}

/***********************
* Scene Draw: output the lines, cities and text
* @author: Joshua Tanner
* @parameter: none
* @return: void
********************/
void Scene::Draw()
{
	m_pBackBuffer->Clear();//Blank screen

	//Drawing the background to backbuffer
	HGDIOBJ hOldObj = SelectObject(spriteHDC, m_hBackgroundBitmap);
	BitBlt(m_pBackBuffer->GetBFDC(), 0, 0, 1000, 800, spriteHDC, 0, 0, SRCCOPY);
	SelectObject(spriteHDC, hOldObj);
	
	for (unsigned int i = 0; i < vecLines.size(); i++)//Drawing lines first to not overlap cities
	{
		//Drawing lines to the backbuffer
		HPEN linePen = CreatePen(PS_SOLID, 2, vecLines[i]->m_color);
		HPEN old_Pen = static_cast<HPEN>(SelectObject(m_pBackBuffer->GetBFDC(), linePen));
		MoveToEx(m_pBackBuffer->GetBFDC(), vecLines[i]->m_iStartX + 12, vecLines[i]->m_iStartY + 12, NULL);
		LineTo(m_pBackBuffer->GetBFDC(), vecLines[i]->m_iEndX + 12, vecLines[i]->m_iEndY + 12);
		SelectObject(m_pBackBuffer->GetBFDC(), old_Pen);
		DeleteObject(linePen);
	}

	int _iWidth, _iHeight, _iX, _iY;
	for (unsigned int i = 0; i < vecCities.size(); i++)//Drawing cities
	{
		//Drawing cities to the backbuffer
		_iWidth = vecCities[i]->_iWidth;
		_iHeight = vecCities[i]->_iHeight;
		_iX = vecCities[i]->_iX;
		_iY = vecCities[i]->_iY;
		HGDIOBJ hOldObj = SelectObject(spriteHDC, vecCities[i]->m_hBitmap);
		BitBlt(m_pBackBuffer->GetBFDC(), _iX, _iY, _iWidth, _iHeight, spriteHDC, 0, 0, SRCCOPY);
		SelectObject(spriteHDC, hOldObj);
	}

	TextOut(m_pBackBuffer->GetBFDC(), 10, 5, L"Left click to begin Hill Climb", 31);//Outputting text instructions
	TextOut(m_pBackBuffer->GetBFDC(), 10, 25, L"Right click to reset", 21);
	TCHAR DistText[256];
	swprintf_s(DistText, 256, L"Distance: %d", m_pHillSearcher->BestDistance);
	TextOut(m_pBackBuffer->GetBFDC(), 10, 45, DistText, wcslen(DistText));//Outputting distance

	m_pBackBuffer->Present();//The screen is shown
}

/***********************
* Scene Update: Update the line positions after hillclimbing
* @author: Joshua Tanner
* @parameter: none
* @return: void
********************/
void  Scene::Update()
{
	for (int i = 0; i < vecCities.size(); i++)
	{
		if (i == vecCities.size() - 1)//Check if last city to link to first city
		{
			vecLines[i]->m_iEndX = m_pHillSearcher->Cities[0]->_iX;
			vecLines[i]->m_iEndY = m_pHillSearcher->Cities[0]->_iY;
			vecLines[i]->m_iStartX = m_pHillSearcher->Cities[i]->_iX;
			vecLines[i]->m_iStartY = m_pHillSearcher->Cities[i]->_iY;
		}
		else
		{
			vecLines[i]->m_iEndX = m_pHillSearcher->Cities[i + 1]->_iX;
			vecLines[i]->m_iEndY = m_pHillSearcher->Cities[i + 1]->_iY;
			vecLines[i]->m_iStartX = m_pHillSearcher->Cities[i]->_iX;
			vecLines[i]->m_iStartY = m_pHillSearcher->Cities[i]->_iY;
		}
	}
}

/***********************
* Scene Reset: Generates new positions for cities
* @author: Joshua Tanner
* @parameter: none
* @return: void
********************/
void Scene::Reset()
{
	for (int i = 0; i < vecCities.size(); i++)
	{
		int X = (rand() % 800) + 100;//Randomise XY positions
		int Y = (rand() % 400) + 200;
		vecCities[i]->_iX = X;
		vecCities[i]->_iY = Y;
		m_pHillSearcher->Cities[i]->_iX = X;
		m_pHillSearcher->Cities[i]->_iY = Y;
		m_pHillSearcher->BestDistance = m_pHillSearcher->VisitAllNodes();
	}

	Update();//Call update so lines change appropriately
}

/***********************
* Destroy Scene Instance: destroy the singleton scene
* @author: Joshua Tanner
* @parameter: none
* @return: void
********************/
void Scene::DestroyInstance()
{
	delete s_pScene;
	s_pScene = 0;
}