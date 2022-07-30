#include "A_Star.h"


A_Star::A_Star()
{

}

A_Star::A_Star(int m[][MapCol], int r, int c)
{
	mapRow = r;
	mapCol = c;
	for (int i = 0; i < mapRow; i++)
	{
		for (int j = 0; j < mapCol; j++)
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
				startNode.row = i;
				startNode.col = j;
			}
			else if (m[i][j] == 2)
			{
				map[i][j].type = TileType::FINAL;
				targetNode.row = i;
				targetNode.col = j;
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

void A_Star::DealWithNearByTile(Node node)
{
	if (!(map[node.row][node.col].type == TileType::OBSTACLE ||      
		IsVisited(node) || 
		node.row < 0 || node.col < 0 || node.row >= mapRow || node.col >= mapCol
		))                                                                             //(temp.row - curPoint.row != 0 && temp.col - curPoint.col != 0) 这个条件要在他们附近有障碍物才有点用处
	{
		if (IsInOpenTable(node))  //在Open表中
		{
			if (map[node.row][node.col].gn > CalGn(node))
			{
				//改变P的父亲
				map[node.row][node.col].parRow = curPoint.row;
				map[node.row][node.col].parCol = curPoint.col;
				//从新计算P的Fn
				CalFn(node);
			}
		}
		else                 //不在Open表中
		{
			openTable.push_back(node);
			//改变P的父亲
			map[node.row][node.col].parRow = curPoint.row;
			map[node.row][node.col].parCol = curPoint.col;
			//从新计算P的Fn
			CalFn(node);
		}
	}
}

bool A_Star::IsVisited(Node node)
{
	for (int i = 0; i < closeTable.size();i++)
	{
		if (node.row == closeTable[i].row && node.col == closeTable[i].col)
			return true;
	}
	return false;
}

bool A_Star::IsInOpenTable(Node node)
{
	for (int i = 0; i < openTable.size(); i++)
	{
		if (node.row == openTable[i].row && node.col == openTable[i].col)
			return true;
	}
	return false;
}

int A_Star::CalGn(Node node)
{
	//说明在对角线上
	int gnAdd;
	if (node.row - curPoint.row != 0 && node.col - curPoint.col != 0)
	{
		gnAdd = 14;
	}
	else
	{
		gnAdd = 10;
	}

	return map[curPoint.row][curPoint.col].gn + gnAdd;
}

void A_Star::CalFn(Node temp)
{
	//计算gn
	//map[temp.row][temp.col].gn += CalGn(temp);
	map[temp.row][temp.col].gn = CalGn(temp);

	//计算hn
	map[temp.row][temp.col].hn = abs(temp.row - targetNode.row) + abs(temp.col - targetNode.col);

	//计算fn
	map[temp.row][temp.col].fn = map[temp.row][temp.col].gn + map[temp.row][temp.col].hn;
}

void A_Star::OutputResult()
{
	vector<char> result;
	result.resize(mapRow * mapCol);

	//扫描进行范围控制
	for (int i = 0; i < mapRow;i++)
	{
		for (int j = 0; j < mapCol;j++)
		{
			if (map[i][j].type == TileType::SPACE)
				result[i*mapCol + j] = ' ';
			else if (map[i][j].type == TileType::START)
				result[i*mapCol + j] = 'S';
			else if (map[i][j].type == TileType::FINAL)
				result[i*mapCol + j] = 'T';
			else if (map[i][j].type == TileType::OBSTACLE)
				result[i*mapCol + j] = 'B';
		}
	}

	//找出那条路径
	Node node = map[curPoint.row][curPoint.col];
	while (node.type != TileType::START)
	{
		if (map[node.parRow][node.parCol].type!=TileType::START)
		{
			result[node.parRow * mapCol + node.parCol] = '*';
		}
		node = map[node.parRow][node.parCol];
	}

	for (int i = 0; i < mapRow; i++)
	{
		for (int j = 0; j < mapCol; j++)
		{
			cout << result[i*mapCol + j] << " ";
		}
		cout << endl;
	}
}

void A_Star::StartPath()
{
	openTable.push_back(startNode);

	bool isFound = false;
	while (openTable.empty() != true)   //只要Open表不空，都可以继续搜索
	{
		int nodeIdxInOpenTableOfMinFn = 0;
		for (int i = 1; i < openTable.size(); i++)
		{
			Node point = openTable[i];
			Node pointOfMinFn = openTable[nodeIdxInOpenTableOfMinFn];
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

		Node temp = curPoint;		//这里就是要复制

		temp.row -= 1;
		temp.col -= 1;
		DealWithNearByTile(temp);

		temp.col += 1;
		DealWithNearByTile(temp);

		temp.col += 1;
		DealWithNearByTile(temp);


		temp.row += 1;
		DealWithNearByTile(temp);

		temp.col -= 2;
		DealWithNearByTile(temp);


		temp.row += 1;
		DealWithNearByTile(temp);

		temp.col += 1;
		DealWithNearByTile(temp);

		temp.col += 1;
		DealWithNearByTile(temp);
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