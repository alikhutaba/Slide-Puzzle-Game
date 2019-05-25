#include "PuzzleBoard.h"
#include <stdlib.h>
#include "GLUT.H"



PuzzleBoard::PuzzleBoard()
{
	g = 0;
	parent = NULL;
}


PuzzleBoard::~PuzzleBoard()
{
}



void PuzzleBoard::InitBoard()
{
	int num, max = N*N;
	int i, j;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			board[i][j] = -1; // the place is free

	for (num = 0; num < max; num++)
	{
		do
		{
			i = rand() % N;
			j = rand() % N;
		} while (board[i][j] != -1);
		// board [i][j] is free!!!!
		board[i][j] = num;
		if (num == 0) // save i0 and j0
		{
			i0 = i;
			j0 = j;
		}
	}// for
	g = 0;
}


void PuzzleBoard::ShowBoard()
{
	int i, j;
	double dx=2.0/N, dy=2.0/N;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
		{
			glColor3d(0.8, 0.8, 1);
			glBegin(GL_POLYGON);
				glVertex2d(-1 + j*dx, 1 -i*dy );
				glVertex2d(-1 + (j + 1)*dx, 1 - i*dy);
				glVertex2d(-1 + (j + 1)*dx, 1 - (i+1)*dy);
				glVertex2d(-1 + j *dx, 1 - (i + 1)*dy);
			glEnd();
			glColor3d(0.0, 0.0, 0.2);

			glBegin(GL_LINE_LOOP);
			glVertex2d(-1 + j*dx, 1 - i*dy);
			glVertex2d(-1 + (j + 1)*dx, 1 - i*dy);
			glVertex2d(-1 + (j + 1)*dx, 1 - (i + 1)*dy);
			glVertex2d(-1 + j *dx, 1 - (i + 1)*dy);
			glEnd();

			if (board[i][j] != 0)
			{
				glRasterPos2d(-1 + j*dx + 0.25, 1 - i*dy - 0.25);
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, board[i][j] % 10 + '0');
				if (board[i][j] >= 10)
				{
					glRasterPos2d(-1 + j*dx + 0.20, 1 - i*dy - 0.25);
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '1');

				}
			}
		}
}

// Aggregated Manhattan Distance
int PuzzleBoard::h()
{
	int i, j;
	int total = 0;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			total += ManhattanDistance(i, j);

	return total;
}




int PuzzleBoard::ManhattanDistance(int i, int j)
{
	int target_i, target_j;
	if (board[i][j] != 0)
	{
		target_i = (board[i][j] - 1) / N;
		target_j = (board[i][j] - 1) % N;
	}
	else // it is 0
	{
		target_i = target_j = N - 1;
	}
	return abs(i-target_i)+abs(j-target_j);
}


int PuzzleBoard::getF()
{
	return f;
}

// f is the evaluation of current board
void PuzzleBoard::SetF()
{
	f = g + h(); // check the weights....
}


int PuzzleBoard::GetI0()
{
	return i0;
}


int PuzzleBoard::GetJ0()
{
	return j0;
}


void PuzzleBoard::MoveUp()
{
	int tmp;
	if (i0 > 0)
	{
		tmp = board[i0][j0];
		board[i0][j0] = board[i0 - 1][j0];
		board[i0 - 1][j0] = tmp;
		i0--;
		g++;
		SetF();
	}
}


void PuzzleBoard::MoveDown()
{
	int tmp;
	if (i0 < N-1)
	{
		tmp = board[i0][j0];
		board[i0][j0] = board[i0 + 1][j0];
		board[i0 + 1][j0] = tmp;
		i0++;
		g++;
		SetF();
	}

}


void PuzzleBoard::MoveLeft()
{
	int tmp;
	if (j0 > 0)
	{
		tmp = board[i0][j0];
		board[i0][j0] = board[i0][j0 - 1];
		board[i0][j0 - 1] = tmp;
		j0--;
		g++;
		SetF();
	}

}


void PuzzleBoard::MoveRight()
{
	int tmp;
	if (j0 < N-1)
	{
		tmp = board[i0][j0];
		board[i0][j0] = board[i0][j0 + 1];
		board[i0][j0 + 1] = tmp;
		j0++;
		g++;
		SetF();
	}
}


void PuzzleBoard::SetG(int new_g)
{
	g = new_g;
}


int PuzzleBoard::GetG()
{
	return g;
}


bool PuzzleBoard::operator==(const PuzzleBoard& other)
{
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			if (board[i][j] != other.board[i][j])
				return false;

	return true;
}


void PuzzleBoard::InitAtDistance(int distance)
{
	int i, j,count;

	// final state
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			board[i][j] = i*N + j + 1;

	board[N - 1][N - 1] = 0;
	i0 = j0 = N - 1; // Initial Space position
	// move SPACE distance random steps from the final state
	for (count = 0; count < distance; count++)
	{
		switch (rand() % 4)
		{
		case 0: // up
			MoveUp();
			break;
		case 1: // down
			MoveDown();
			break;
		case 2: // left
			MoveLeft();
			break;
		case 3: // right
			MoveRight();
			break;
		} // switch
	}// for
	SetG(0); // this will be the initial state
}


void PuzzleBoard::SetParent(PuzzleBoard* parent)
{
	this->parent = parent;
}


PuzzleBoard* PuzzleBoard::GetParent()
{
	return parent;
}
