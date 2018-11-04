# A星算法
## 步骤
1. 将寻路空间划分为`node`
	- ![](1.png)
	~~~
	struct Node 
	{
		TileType type;  //记录这个节点的类型
		int parRow;    	//用于记录父节点的行位置
		int parCol;    	//用于记录父节点的列位置
		int fn;
		int gn;  		//从起点到当前node的路径移动代价，一般表示距离
		int hn;  		//启发值(估算值)，启发值的计算方法有很多；这里只使用最简单的计算方法——曼哈顿算法；
				 		//曼哈顿计算方法：忽略地图中的一切，当做只有该节点和目标节点,让后去计算出走到终点（不允许对角线行走，不是不给，这样做的话会很麻烦，而且对算法效率提升的作用不大）所需要的步数；
				 		//启发值是对该节点走到终点需要代价的一个预估值；
	};
	~~~
2. 从`openTable`中取出fn值最低的`nodeX`，并放入`closeTable`
	- if nodeX == None
		- 找不到最短路径
	- else
		- if nodeX == targetNode
			- 最短路径已经找到，终止以下操作
3. 从`nodeX`外一层筛选出满足一下条件的`nodes`
	- 不在`closeTable`
	- 可到达
4. 处理`nodes`
	- for node in nodes
		1. 将`nodes`的父亲都指向`nodeX`
		2. 计算gn
		3. 计算hn
			- 可采用**曼哈顿方法**：从当前node到targetNode的水平和垂直方格的数量总和 乘以 10
		4. 计算fn = gn + hn
5. 将`nodes`处理进`openTable`
	- for node in nodes
		- if node not in opentable
			- 直接放进`openTable`
		- else
			- if node.gn < nodeInopenTable.gn
				- 将nodeInopenTable的数据修改为node的数据
6. 回到第二步
## 参考资料
- [参考资料1](http://zhyt710.iteye.com/blog/739803)