/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	HillClimbSearch.cpp
Description	:	Hill Climb implementation file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#include "HillClimbSearch.h"


/***********************
* Constructor: constructs searcher
* @author: Joshua Tanner
* @parameter: none
* @return: 
********************/
HillClimbSearch::HillClimbSearch()
{
}

/***********************
* Destructor: destroy object and delete dynamic memory
* @author: Joshua Tanner
* @parameter: none
* @return: 
********************/
HillClimbSearch::~HillClimbSearch()
{
	for (unsigned int i = 0; i < Cities.size(); i++)
	{
		delete Cities[i];
		Cities[i] = 0;
	}
}


/***********************
* CalcNodeDist: calculates the distance between 2 nodes
* @author: Joshua Tanner
* @parameter: node A, node B
* @return: int
********************/
int HillClimbSearch::CalcNodeDist(Node* A, Node* B)
{
	int _iHorizontal = abs(A->_iX - B->_iX);
	int _iVertical = abs(A->_iY - B->_iY);
	return(sqrt(pow(_iHorizontal, 2) + pow(_iVertical, 2)));
}

/***********************
* Begin hill climb: finds shortest route for visiting all cities
* @author: Joshua Tanner
* @parameter:none
* @return: void
********************/
 void HillClimbSearch::BeginHillClimb()
{	
	BestDistance = VisitAllNodes();//Getting the starting distance between cities to compare
	int CurrentDistance = BestDistance;

	while (true)
	{
		//Swapping elements in vector and restesting distance
		int i = 0;
		int temp = VisitAllNodes();
		while (i < Cities.size())
		{
			//Swapping the nodes
			Node* back = Cities.back();
			Cities[Cities.size() - 1] = Cities[i];
			Cities[i] = back;
			CurrentDistance = VisitAllNodes();//Retesting the distance

			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;//Save the improved distance
				break;
			}
			else
			{
				//Swap back if distance is not improved
				back = Cities.back();
				Cities[Cities.size() - 1] = Cities[i];
				Cities[i] = back;
			}
			i++;
		}
		
		//The loop will exit when no improving moves are found
		//Current distance hasn't changed in loop
		if (CurrentDistance == temp)
		{
			break;
		}
	}

}

 /***********************
 * Visit all nodes: calculates distance between all cities
 * @author: Joshua Tanner
 * @parameter:none
 * @return: int
 ********************/
int HillClimbSearch::VisitAllNodes()
{
	int CurrentDistance = 0;	
	for (unsigned int i = 0; i < Cities.size(); i++)
	{
		if (i == Cities.size() - 1)//Check if last city, link back to first city
		{
			CurrentDistance += CalcNodeDist(Cities[i], Cities[0]);
			
		}
		else
		{
			CurrentDistance += CalcNodeDist(Cities[i], Cities[i + 1]);
		}
	}
	return(CurrentDistance);
}


