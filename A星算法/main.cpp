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
	string s5 = "0 0 0 3 0 0 0 ";

	stringstream input(s0 + s1 + s2 + s3 + s4 + s5);

	int mapRow;
	int mapCol;

	input >> mapRow;
	input >> mapCol;

	int map[MapRow][MapCol] = { 0 };

	for (int i = 0; i < mapRow;i++)
	{
		for (int j = 0; j < mapCol;j++)
		{
			input >> map[i][j];
		}
	}

// 	int a[5][7] = { 0, 0, 0, 0, 0, 0, 0,
// 					3, 3, 3, 0, 0, 0, 0,
// 					0, 1, 0, 3, 0, 2, 0,
// 					3, 3, 3, 3, 0, 0, 0,
// 					0, 0, 0, 0, 0, 0, 0 };

	A_Star test(map, mapRow, mapCol);

	test.StartPath();

	cin.get();

	return 0;
}