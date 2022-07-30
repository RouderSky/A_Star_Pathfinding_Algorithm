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
			nodesOfMap[i][j] = new Node();		//todo wht 记得delete
			nodesOfMap[i][j]->row = i;
			nodesOfMap[i][j]->col = j;
			nodesOfMap[i][j]->parRow = -1;
			nodesOfMap[i][j]->parCol = -1;
			nodesOfMap[i][j]->fn = 0;
			nodesOfMap[i][j]->gn = 0;
			nodesOfMap[i][j]->hn = 0;
			if (m[i][j] == 0)
			{
				nodesOfMap[i][j]->type = TileType::SPACE;
			}
			else if (m[i][j] == 1)
			{
				nodesOfMap[i][j]->type = TileType::START;
				startNode = nodesOfMap[i][j];
			}
			else if (m[i][j] == 2)
			{
				nodesOfMap[i][j]->type = TileType::FINAL;
				targetNode = nodesOfMap[i][j];
			}
			else if (m[i][j] == 3)
			{
				nodesOfMap[i][j]->type = TileType::OBSTACLE;
			}
		}
	}
}

A_Star::~A_Star()
{
}

void A_Star::DealWithNearByTile(int row, int col)
{
	if (row >= 0 && col >= 0 && row < mapRow && col < mapCol)
	{
		Node* node = nodesOfMap[row][col];

		if (node->type != TileType::OBSTACLE && !IsVisited(node))   //(temp.row - curPoint.row != 0 && temp.col - curPoint.col != 0) 这个条件要在他们附近有障碍物才有点用处
		{
			if (IsInOpenTable(node))  //在Open表中
			{
				if (node->gn > CalGn(node))
				{
					//改变P的父亲
					node->parRow = curNode->row;
					node->parCol = curNode->col;
					//从新计算P的Fn
					CalFn(node);
				}
			}
			else                 //不在Open表中
			{
				openTable.push_back(node);
				//改变P的父亲
				node->parRow = curNode->row;
				node->parCol = curNode->col;
				//从新计算P的Fn
				CalFn(node);
			}
		}
	}
}

bool A_Star::IsVisited(Node* node)
{
	for (int i = 0; i < closeTable.size();i++)
	{
		if (node->row == closeTable[i]->row && node->col == closeTable[i]->col)
			return true;
	}
	return false;
}

bool A_Star::IsInOpenTable(Node* node)
{
	for (int i = 0; i < openTable.size(); i++)
	{
		if (node->row == openTable[i]->row && node->col == openTable[i]->col)
			return true;
	}
	return false;
}

int A_Star::CalGn(Node* node)
{
	//说明在对角线上
	int gnAdd;
	if (node->row - curNode->row != 0 && node->col - curNode->col != 0)
	{
		gnAdd = 14;
	}
	else
	{
		gnAdd = 10;
	}

	return curNode->gn + gnAdd;		//todo wht 直接用curNode，怪怪的
}

void A_Star::CalFn(Node* temp)
{
	//计算gn
	//temp->gn += CalGn(temp);				//todo wht 为什么要加上旧的gn？
	temp->gn = CalGn(temp);

	//计算hn
	temp->hn = abs(temp->row - targetNode->row) + abs(temp->col - targetNode->col);

	//计算fn
	temp->fn = temp->gn + temp->hn;
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
			if (nodesOfMap[i][j]->type == TileType::SPACE)
				result[i*mapCol + j] = ' ';
			else if (nodesOfMap[i][j]->type == TileType::START)
				result[i*mapCol + j] = 'S';
			else if (nodesOfMap[i][j]->type == TileType::FINAL)
				result[i*mapCol + j] = 'T';
			else if (nodesOfMap[i][j]->type == TileType::OBSTACLE)
				result[i*mapCol + j] = 'B';
		}
	}

	//找出那条路径
	Node* curNode = this->curNode;	//todo wht 不要这样拿数据，通过targetNode来拿
	while (curNode->type != TileType::START)
	{
		if (curNode->type!=TileType::START)		//todo wht 可以去掉吧？
		{
			result[curNode->parRow * mapCol + curNode->parCol] = '*';
		}
		curNode = nodesOfMap[curNode->parRow][curNode->parCol];
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
		int minFnNodeIdxInOpenTable = 0;
		Node* minFnNodeInOpenTable = openTable[0];
		for (int i = 1; i < openTable.size(); i++)
		{
			//todo wht 每次都这样找最小很耗
			Node* point = openTable[i];
			if (point->fn < minFnNodeInOpenTable->fn)
			{
				minFnNodeInOpenTable = point;
				minFnNodeIdxInOpenTable = i;
			}
		}
		openTable.erase(openTable.begin() + minFnNodeIdxInOpenTable);

		curNode = minFnNodeInOpenTable;
		closeTable.push_back(curNode);

		if (curNode->type == TileType::FINAL)
		{
			isFound = true;
			break;
		}

		DealWithNearByTile(curNode->row - 1, curNode->col - 1);

		DealWithNearByTile(curNode->row - 1, curNode->col);

		DealWithNearByTile(curNode->row - 1, curNode->col + 1);


		DealWithNearByTile(curNode->row, curNode->col + 1);

		DealWithNearByTile(curNode->row, curNode->col -1);


		DealWithNearByTile(curNode->row + 1, curNode->col - 1);

		DealWithNearByTile(curNode->row + 1, curNode->col);

		DealWithNearByTile(curNode->row + 1, curNode->col + 1);
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