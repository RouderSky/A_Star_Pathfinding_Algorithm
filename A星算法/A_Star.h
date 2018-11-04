#pragma once
#ifndef A_STAR__H_
#define A_STAR__H_

//个人理解：A*算法是基于广度搜索进行升级的一种算法；
//优点在于不用像广度搜索一样需要将已经搜到的节点全部张开，每一次都只是展开其中一个节点；这个节点就是当前Open表中Fn值最小的那个节点；
//Fn值由两部分组成:Fn = Gn + Hn

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define MapRow 128
#define MapCol 128

enum TileType{SPACE,START,FINAL,OBSTACLE};

struct Node 
{
	TileType type;  //记录这个节点的类型
	int parRow;		//用于记录父节点的行位置
	int parCol;		//用于记录父节点的列位置
	int fn;
	int gn;			//从起点到当前node的路径移动代价，一般表示距离
	int hn;			//启发值(估算值)，启发值的计算方法有很多；这里只使用最简单的计算方法——曼哈顿算法；
					//曼哈顿计算方法：忽略地图中的一切，当做只有该节点和目标节点,让后去计算出走到终点（不允许对角线行走，不是不给，这样做的话会很麻烦，而且对算法效率提升的作用不大）所需要的步数；
					//启发值是对该节点走到终点需要代价的一个预估值；
};

struct Position
{
	int row;
	int col;
};

class A_Star
{
public:
	A_Star();         //这个构造函数没有什么用处    
	A_Star(int a[][MapCol], int m, int n);  //需要传进来一个二维数组（类型是int即可），代表地图（空地设置为0，起点设置为1，终点设置为2；障碍物设置为3；），其中调用func即可
	~A_Star();

	//一个二维数组（类型是Node）：A*算法需要的特殊节点地图
	Node aStarMap[MapRow][MapCol];
	int row;    //记录行数
	int col;	//记录列数
	Position finalPoint;

// 	//一个二维数组（类型是int）：原型地图
// 	int (*originMap)[MapWidth];

	//Open表（仅存储节点在二维数组中的位置）：存储已经被搜索到的点，且这些点已经被链接好了;所有在open表中的点数据都是完整的
	vector<Position> openTable;

	//Close表（仅存储节点在二维数组中的位置）：存储被尝试过行走的点(使用vector来存储，到时直接调用back方法就可以得到当前所在的节点)
	vector<Position> closeTable;

	//总控函数：总体运行框架
	void StartPath();

	//func：初始化函数：将原型地图转换成A*算法所需要的地图（）
	void InitAStarMap(int a[][MapCol], int m, int n);

	//func0:开始时的准备：将起点放进Open表中,把终点的坐标也找出来，方便以后Gn值的计算
	void ReadyPath();

	//func1:寻找Open表中Fn值最小的那个节点,从Open中删除，将它压进Close表中，即back出来就可以得到当前所在的格子
	void FindMixFnPoint();

	//func2：判断当前格附近的8个格子中其中一个的状态：共有3种情况，用三个 if else if 或者 嵌套的if（优先） 都可以；设其中一个点为P
		//如果 不可行走 或者 已在Close表 中则忽略掉(可不可以行走是可以自己定义的)
		//如果 不在Open表中 则直接添加进Open表中，并且将 P的父亲 指向 当前格，并且调用函数func3来计算P的Fn
		//如果 在Open表中 则判断 表中的P的Gn值 是否比 当前节点到P的Gn值 大:1.是则将Open表中 P的父亲 改成 当前节点，并调用func3来重新计算P的值
	void JudgeTheTile(Position temp);

	//func3:自动计算传来的格子的Fn，其中Gn的计算要以它的父节点为依据，Hn用曼哈顿算法
	void SetTileFn(Position temp);

	//func4:检测Close顶部格子的相邻8个格子
		//依次对8个格子调用func2即可
	void DealWithTileNearBy();

	//func5：检测一个点是不是已经在Close表中
	bool IsInClose(Position temp);

	//func6:检测一个点是不是已经在Open表中
	bool IsInOpen(Position temp);

	//func7:临时计算当前格附近某点到当前格的gn值
	int CalGn(Position temp);

	//func8:输出寻路结果结果
	void OutputResult();
};


#endif // !1



