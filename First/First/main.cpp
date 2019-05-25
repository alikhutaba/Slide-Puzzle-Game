#include <Windows.h>
#include "GLUT.h"
#include <math.h>
#include "PuzzleBoard.h"

#include <time.h>
#include <vector>
#include <queue>
#include <algorithm>
#include "CompareNodes.h"

using namespace std;

const int W = 600; // window width
const int H = 600; // window height

PuzzleBoard* pboard = new PuzzleBoard();
bool run_a_star = false, show_solution = false;
priority_queue <PuzzleBoard, vector <PuzzleBoard>, CompareNodes> pq;

vector <PuzzleBoard> black;
vector <PuzzleBoard> gray;
vector <PuzzleBoard> solution;

void init()
{
	int i, j;
	glClearColor(0.7, 0.7, 0.7, 0);

	srand(time(0));

//	pboard->InitBoard();
	pboard->InitAtDistance(50);
	// insert current board to priority queue
	pq.emplace(*pboard);
	gray.push_back(*pboard);


	glOrtho(-1, 1, -1, 1, -1, 1);
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);


	pboard->ShowBoard();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void A_star_iteration()
{
	PuzzleBoard board;
	PuzzleBoard* ptmp;
	PuzzleBoard* pb;
	vector<PuzzleBoard>::iterator black_iterator;
	vector<PuzzleBoard>::iterator gray_iterator;

	if (pq.empty())
	{
		printf("The queue is empty: no solution\n");
		run_a_star = false;
	}
	else // queue is not empty
	{
		board = pq.top();
		// ptmp will be parent of neghbours of board
		ptmp = new PuzzleBoard(board);
		pq.pop();
		// paint it black:
		black.push_back(board);
		gray_iterator = find(gray.begin(), gray.end(), board);
		gray.erase(gray_iterator);

		// The solution has been found
		if (board.h() == 0)
		{
			printf("The solution has been found\n");
			printf("It took %d steps\n", board.GetG());
			run_a_star = false;
			show_solution = true;
			while (ptmp->GetParent() != NULL)
			{
				solution.push_back(*ptmp);
				ptmp = ptmp->GetParent();
			}

		}
		else
		{
			// check neighbours

			// up
			if (board.GetI0() > 0)
			{
				board.MoveUp(); // g goes  UP
				pb = new PuzzleBoard(board);
				pb->SetParent(ptmp);
				// MoveUp updates G. We want to cancel it
				board.SetG(board.GetG() - 1);
				// check if pb not black and not gray
				black_iterator = find(black.begin(), black.end(), *pb);
				gray_iterator = find(gray.begin(), gray.end(), *pb);
				if (black_iterator == black.end() && gray_iterator == gray.end())
				{
					pq.emplace(*pb);
					// paint it gray
					gray.push_back(*pb);
				}
				// restore board
				board.MoveDown();
				board.SetG(board.GetG() - 1);
			}
			// check down
			if (board.GetI0() < N - 1)
			{
				board.MoveDown(); // g goes  up
				pb = new PuzzleBoard(board);
				pb->SetParent(ptmp);
				board.SetG(board.GetG() - 1);
				// check if pb not black and not gray
				black_iterator = find(black.begin(), black.end(), *pb);
				gray_iterator = find(gray.begin(), gray.end(), *pb);
				if (black_iterator == black.end() && gray_iterator == gray.end())
				{
					pq.emplace(*pb);
					// paint it gray
					gray.push_back(*pb);
				}
				// restore board
				board.MoveUp();
				board.SetG(board.GetG() - 1);
			}
			// left
			if (board.GetJ0() > 0)
			{
				board.MoveLeft(); // g goes  UP
				pb = new PuzzleBoard(board);
				pb->SetParent(ptmp);
				board.SetG(board.GetG() - 1);
				// check if pb not black and not gray
				black_iterator = find(black.begin(), black.end(), *pb);
				gray_iterator = find(gray.begin(), gray.end(), *pb);
				if (black_iterator == black.end() && gray_iterator == gray.end())
				{
					pq.emplace(*pb);
					// paint it gray
					gray.push_back(*pb);
				}
				// restore board
				board.MoveRight();
				board.SetG(board.GetG() - 1);
			}
			// right
			if (board.GetJ0() < N - 1)
			{
				board.MoveRight(); // g goes  UP
				pb = new PuzzleBoard(board);
				pb->SetParent(ptmp);
				board.SetG(board.GetG() - 1);
				// check if pb not black and not gray
				black_iterator = find(black.begin(), black.end(), *pb);
				gray_iterator = find(gray.begin(), gray.end(), *pb);
				if (black_iterator == black.end() && gray_iterator == gray.end())
				{
					pq.emplace(*pb);
					// paint it gray
					gray.push_back(*pb);
				}
				// restore board
				board.MoveLeft();
				board.SetG(board.GetG() - 1);
			}

			printf("h = %d, pq size is: %d\n", board.h(), pq.size());
		}
	}
}

void idle()
{
	if (run_a_star)
		A_star_iteration();
	glutPostRedisplay();// calls indirectly to display
}

void Menu(int choice)
{
	switch (choice)
	{
	case 1:
		run_a_star = true;
		break;
	}

}

void mouse(int button, int state, int x, int y)
{
	if (show_solution && button == GLUT_LEFT_BUTTON &&
		state == GLUT_DOWN && !solution.empty())
	{
		pboard = &(solution[solution.size() - 1]);
		solution.pop_back();
	}
}
void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Digits Example ");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens

	glutMouseFunc(mouse);
	glutCreateMenu(Menu);
	glutAddMenuEntry("Run A*", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	init();

	glutMainLoop();
}