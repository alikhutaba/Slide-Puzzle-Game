#include "PuzzleBoard.h"
#pragma once
class CompareNodes
{
public:
	CompareNodes();
	~CompareNodes();
	bool operator () (PuzzleBoard& b1, PuzzleBoard& b2)
	{
		return b1.getF() > b2.getF();
	}
};

