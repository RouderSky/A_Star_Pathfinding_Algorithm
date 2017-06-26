#include "A_Star.h"


A_Star::A_Star()
{

}

A_Star::A_Star(int a[][MapCol], int m, int n)
{
	InitAStarMap(a, m, n);
}

A_Star::~A_Star()
{
}

void A_Star::InitAStarMap(int a[][MapCol], int m, int n)
{
	row = m;
	col = n;
	for (int i = 0; i < row;i++)
	{
		for (int j = 0; j < col;j++)
		{
			aStarMap[i][j].parRow = -1;
			aStarMap[i][j].parCol = -1;
			aStarMap[i][j].fn = 0;
			aStarMap[i][j].gn = 0;
			aStarMap[i][j].hn = 0;
			if (a[i][j] == 0)
				aStarMap[i][j].type = TileType::SPACE;
			else if (a[i][j] == 1)
				aStarMap[i][j].type = TileType::START;
			else if (a[i][j] == 2)
				aStarMap[i][j].type = TileType::FINAL;
			else if (a[i][j] == 3)
				aStarMap[i][j].type = TileType::OBSTACLE;
			
		}
	}
}

void A_Star::ReadyPath()
{
	Node startNode;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			//判断是不是起点
			if (aStarMap[i][j].type == TileType::START)
			{
				Position temp;
				temp.row = i;
				temp.col = j;
				openTable.push_back(temp);
			}

			//判断是不是终点
			if (aStarMap[i][j].type == TileType::FINAL)
			{
				finalPoint.row = i;
				finalPoint.col = j;
			}
		}
	}
}

void A_Star::FindMixFnPoint()
{
	//寻找fn值最小的节点
	int mixFnNumInOpen = 0;
	for (int i = 1; i < openTable.size(); i++)
	{
		//如果当前点的fn比最小要小就记录
		if (aStarMap[openTable[i].row][openTable[i].col].fn<aStarMap[openTable[mixFnNumInOpen].row][openTable[mixFnNumInOpen].col].fn)
		{
			mixFnNumInOpen = i;
		}
	}

	//压进Close表
	closeTable.push_back(openTable[mixFnNumInOpen]);

	//从Open表中删除
	openTable.erase(openTable.begin() + mixFnNumInOpen);
}

void A_Star::JudgeTheTile(Position temp)
{
	if (!(aStarMap[temp.row][temp.col].type == TileType::OBSTACLE ||      
		IsInClose(temp) || 
		temp.row < 0 || temp.col < 0 || temp.row >= row || temp.col >= col
		))                                                                             //(temp.row - closeTable.back().row != 0 && temp.col - closeTable.back().col != 0) 这个条件要在他们附近有障碍物才有点用处
	{
		if (IsInOpen(temp))  //在Open表中
		{
			if (aStarMap[temp.row][temp.col].gn > CalGn(temp))
			{
				//改变P的父亲
				aStarMap[temp.row][temp.col].parRow = closeTable.back().row;
				aStarMap[temp.row][temp.col].parCol = closeTable.back().col;
				//从新计算P的Fn
				SetTileFn(temp);
			}
		}
		else                 //不在Open表中
		{
			openTable.push_back(temp);
			//改变P的父亲
			aStarMap[temp.row][temp.col].parRow = closeTable.back().row;
			aStarMap[temp.row][temp.col].parCol = closeTable.back().col;
			//从新计算P的Fn
			SetTileFn(temp);
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
	if (temp.row - closeTable.back().row != 0 && temp.col - closeTable.back().col != 0)
	{
		gnAdd = 14;
	}
	else
	{
		gnAdd = 10;
	}

	return aStarMap[closeTable.back().row][closeTable.back().col].gn + gnAdd;
}

void A_Star::SetTileFn(Position temp)
{
	//计算gn
	aStarMap[temp.row][temp.col].gn += CalGn(temp);

	//计算hn
	aStarMap[temp.row][temp.col].hn = abs(temp.row - finalPoint.row) + abs(temp.col - finalPoint.col);

	//计算fn
	aStarMap[temp.row][temp.col].fn = aStarMap[temp.row][temp.col].gn + aStarMap[temp.row][temp.col].hn;
}

void A_Star::DealWithTileNearBy()
{
	Position temp = closeTable.back();

	temp.row -= 1;
	temp.col -= 1;
	JudgeTheTile(temp);

	temp.col += 1;
	JudgeTheTile(temp);

	temp.col += 1;
	JudgeTheTile(temp);

	temp.row += 1;
	JudgeTheTile(temp);

	temp.col -= 1;
	JudgeTheTile(temp);

	temp.col -= 1;
	JudgeTheTile(temp);

	temp.row += 1;
	JudgeTheTile(temp);

	temp.col += 1;
	JudgeTheTile(temp);

	temp.col += 1;
	JudgeTheTile(temp);
}

void A_Star::OutputResult()
{
	vector<int> result;
	result.resize(row*col);

	//扫描进行范围控制
	for (int i = 0; i < row;i++)
	{
		for (int j = 0; j < col;j++)
		{
			if (aStarMap[i][j].type == TileType::SPACE)
				result[i*col + j] = 0;
			else if (aStarMap[i][j].type == TileType::START)
				result[i*col + j] = 1;
			else if (aStarMap[i][j].type == TileType::FINAL)
				result[i*col + j] = 2;
			else if (aStarMap[i][j].type == TileType::OBSTACLE)
				result[i*col + j] = 3;
		}
	}


	//找出那条路径
	Position temp = closeTable.back();
	Node node = aStarMap[temp.row][temp.col];
	while (node.type != TileType::START)
	{
		if (aStarMap[node.parRow][node.parCol].type!=TileType::START)
		{
			result[node.parRow * col + node.parCol] = 6;
		}
		node = aStarMap[node.parRow][node.parCol];
	}

	//
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
	//调用func0,完成准备
	ReadyPath();

	//主控逻辑
	bool find = 0;
	while (openTable.empty() != 1)   //只要Open表不空，都可以继续搜索
	{
		//调用func1，寻找fn最小的那个点
		FindMixFnPoint();

		//判断刚刚被压进Close表的格子（即当前格）是不是终点
		if (aStarMap[closeTable.back().row][closeTable.back().col].type == TileType::FINAL)
		{
			find = 1;
			break;
		}

		//调用func4
		DealWithTileNearBy();
	}

	//find等于1就证明找到了那条路径
	if (find == 1)
	{
		OutputResult();
	}
	else
	{
		cout << "寻路失败";
	}

}