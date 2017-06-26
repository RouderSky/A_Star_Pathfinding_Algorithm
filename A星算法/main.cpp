#include <iostream>
#include <string>
#include <sstream>
#include "A_Star.h"

using namespace std;

int main()
{
	string s0 = "5 7 ";
	string s1 = "0 0 0 0 0 0 0 ";       
	string s2 = "0 0 0 3 3 3 0 ";
	string s3 = "0 1 0 3 0 3 0 ";
	string s4 = "0 0 0 3 2 3 0 ";
	string s5 = "0 0 0 0 0 0 0";

	stringstream ss(s0 + s1 + s2 + s3 + s4 + s5);

	int row;
	int col;

	ss >> row;
	ss >> col;

	int a[MapRow][MapCol] = { 0 };

	for (int i = 0; i < row;i++)
	{
		for (int j = 0; j < col;j++)
		{
			ss >> a[i][j];
		}
	}

// 	int a[5][7] = { 0, 0, 0, 0, 0, 0, 0,
// 					3, 3, 3, 0, 0, 0, 0,
// 					0, 1, 0, 3, 0, 2, 0,
// 					3, 3, 3, 3, 0, 0, 0,
// 					0, 0, 0, 0, 0, 0, 0 };

	A_Star test(a, row, col);

	test.StartPath();

	cin.get();

	return 0;
}