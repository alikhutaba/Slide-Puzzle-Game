#pragma once
const int N = 4;
class PuzzleBoard
{
public:
	PuzzleBoard();
	~PuzzleBoard();
private:
	int board[N][N];
	int i0, j0; // empty place coordinates
	int g; // distance from initial target
	int f;
	PuzzleBoard* parent;
public:
	void InitBoard();
	void ShowBoard();
	int h();
	int ManhattanDistance(int i, int j);
	int getF();
	void SetF();
	int GetI0();
	int GetJ0();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void SetG(int new_g);
	int GetG();
	bool operator==(const PuzzleBoard& other);
	void InitAtDistance(int distance);
	void SetParent(PuzzleBoard* parent);
	PuzzleBoard* GetParent();
};

