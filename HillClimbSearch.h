/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	HillClimbSearch.h
Description	:	Hill Climb header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#pragma once
#include <vector>

//Node struct to represent cities
struct Node {
	Node() {}
	Node(int X, int Y) :_iX(X), _iY(Y){}
	int _iX;//X Postion of node or 'city'
	int _iY;//Y Postion of node or 'city'	
};


//Hillclimb search
class HillClimbSearch
{

private:	

protected:
	
public:
	HillClimbSearch();
	~HillClimbSearch();

	int BestDistance;
	int VisitAllNodes();//Get distance between all nodes

	void BeginHillClimb();//Hill climb happens here
	
	int CalcNodeDist(Node* A, Node* B);
	std::vector<Node*> Cities;

};

