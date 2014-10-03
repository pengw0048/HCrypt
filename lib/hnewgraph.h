/*
 * 表示一套打乱后的图
 */

#ifndef HNEWGRAPH_C
#define HNEWGRAPH_C

#include "hgraph.h"
#include "hpath.h"
#include <vector>
#include <string>
using namespace std;

#define MAXNEWGRAPH 64

class HNewGraph{
public:
	int count;	//图数
	HGraph ngraph[MAXNEWGRAPH];	//新图集
	HGraph graph;	//原图
	HPath npath[MAXNEWGRAPH];	//新回路
	HPath path;	//原回路
	string md5;	//新图的md5
	vector<char> save();	//序列化
	vector<char> savemyans();	//保存答案（对应关系和回路）
	bool load(const vector<char>&);	//从vector中读取，返回是否有效
};

#endif