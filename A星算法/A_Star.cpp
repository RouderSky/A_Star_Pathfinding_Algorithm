#include "A_Star.h"


A_Star::A_Star()
{

}

A_Star::A_Star(int m[][MapCol], int r, int c)
{
	row = r;
	col = c;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			map[i][j].parRow = -1;
			map[i][j].parCol = -1;
			map[i][j].fn = 0;
			map[i][j].gn = 0;
			map[i][j].hn = 0;
			if (m[i][j] == 0)
			{
				map[i][j].type = TileType::SPACE;
			}
			else if (m[i][j] == 1)
			{
				map[i][j].type = TileType::START;
				startPoint.row = i;
				startPoint.col = j;
			}
			else if (m[i][j] == 2)
			{
				map[i][j].type = TileType::FINAL;
				targetPoint.row = i;
				targetPoint.col = j;
			}
			else if (m[i][j] == 3)
			{
				map[i][j].type = TileType::OBSTACLE;
			}
		}
	}
}

A_Star::~A_Star()
{
}

void A_Star::JudgeTheTile(Position pos)
{
	if (!(map[pos.row][pos.col].type == TileType::OBSTACLE ||      
		IsInClose(pos) || 
		pos.row < 0 || pos.col < 0 || pos.row >= row || pos.col >= col
		))                                                                             //(temp.row - curPoint.row != 0 && temp.col - curPoint.col != 0) 这个条件要在他们附近有障碍物才有点用处
	{
		if (IsInOpen(pos))  //在Open表中
		{
			if (map[pos.row][pos.col].gn > CalGn(pos))
			{
				//改变P的父亲
				map[pos.row][pos.col].parRow = curPoint.row;
				map[pos.row][pos.col].parCol = curPoint.col;
				//从新计算P的Fn
				SetTileFn(pos);
			}
		}
		else                 //不在Open表中
		{
			openTable.push_back(pos);
			//改变P的父亲
			map[pos.row][pos.col].parRow = curPoint.row;
			map[pos.row][pos.col].parCol = curPoint.col;
			//从新计算P的Fn
			SetTileFn(pos);
		}
	}
}

bool A_Star::IsInClose(Position temp)
{
	for (int i = 0; i < closeTable.size();i++)
	{
		if (temp.row == closeTable[i].row && temp.col == closeTable[i].col)
			return true;
	}
	return false;
}

bool A_Star::IsInOpen(Position temp)
{
	for (int i = 0; i < openTable.size(); i++)
	{
		if (temp.row == openTable[i].row && temp.col == openTable[i].col)
			return true;
	}
	return false;
}

int A_Star::CalGn(Position temp)
{
	//说明在对角线上
	int gnAdd;
	if (temp.row - curPoint.row != 0 && temp.col - curPoint.col != 0)
	{
		gnAdd = 14;
	}
	else
	{
		gnAdd = 10;
	}

	return map[curPoint.row][curPoint.col].gn + gnAdd;
}

void A_Star::SetTileFn(Position temp)
{
	//计算gn
	//map[temp.row][temp.col].gn += CalGn(temp);
	map[temp.row][temp.col].gn = CalGn(temp);

	//计算hn
	map[temp.row][temp.col].hn = abs(temp.row - targetPoint.row) + abs(temp.col - targetPoint.col);

	//计算fn
	map[temp.row][temp.col].fn = map[temp.row][temp.col].gn + map[temp.row][temp.col].hn;
}

void A_Star::OutputResult()
{
	vector<char> result;
	result.resize(row * col);

	//扫描进行范围控制
	for (int i = 0; i < row;i++)
	{
		for (int j = 0; j < col;j++)
		{
			if (map[i][j].type == TileType::SPACE)
				result[i*col + j] = ' ';
			else if (map[i][j].type == TileType::START)
				result[i*col + j] = 'S';
			else if (map[i][j].type == TileType::FINAL)
				result[i*col + j] = 'T';
			else if (map[i][j].type == TileType::OBSTACLE)
				result[i*col + j] = 'B';
		}
	}

	//找出那条路径
	Node node = map[curPoint.row][curPoint.col];
	while (node.type != TileType::START)
	{
		if (map[node.parRow][node.parCol].type!=TileType::START)
		{
			result[node.parRow * col + node.parCol] = '*';
		}
		node = map[node.parRow][node.parCol];
	}

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			cout << result[i*col + j] << " ";
		}
		cout << endl;
	}
}

void A_Star::StartPath()
{
	openTable.push_back(startPoint);

	bool isFound = false;
	while (openTable.empty() != true)   //只要Open表不空，都可以继续搜索
	{
		int nodeIdxInOpenTableOfMinFn = 0;
		for (int i = 1; i < openTable.size(); i++)
		{
			Position point = openTable[i];
			Position pointOfMinFn = openTable[nodeIdxInOpenTableOfMinFn];
			if (map[point.row][point.col].fn < map[pointOfMinFn.row][pointOfMinFn.col].fn)
				nodeIdxInOpenTableOfMinFn = i;
		}

		curPoint = openTable[nodeIdxInOpenTableOfMinFn];
		closeTable.push_back(curPoint);
		openTable.erase(openTable.begin() + nodeIdxInOpenTableOfMinFn);

		if (map[curPoint.row][curPoint.col].type == TileType::FINAL)
		{
			isFound = true;
			break;
		}

		Position temp = curPoint;		//这里就是要复制

		temp.row -= 1;
		temp.col -= 1;
		JudgeTheTile(temp);

		temp.col += 1;
		JudgeTheTile(temp);

		temp.col += 1;
		JudgeTheTile(temp);


		temp.row += 1;
		JudgeTheTile(temp);

		temp.col -= 2;
		JudgeTheTile(temp);


		temp.row += 1;
		JudgeTheTile(temp);

		temp.col += 1;
		JudgeTheTile(temp);

		temp.col += 1;
		JudgeTheTile(temp);
	}

	if (isFound)
	{
		OutputResult();
	}
	else
	{
		cout << "寻路失败";
	}

}