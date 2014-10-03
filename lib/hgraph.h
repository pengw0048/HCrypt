/*
 * 定义H图类：HGraph
 */

#ifndef HGRAPH_H
#define HGRAPH_H

#include <vector>
#include "md5.h"
using namespace std;

struct Edge{	//表示一条有向边
	int from,to,number;
	Edge(){};
	Edge(const int,const int,const int);	//直接赋值from,to,number
	bool operator <(const Edge &b)const{
		return number<b.number;
	}
};

class HGraph{
public:
	int n,m;	//点数、边数
	bool valid;	//读取时表示是否有效
	vector<int> vnumber;	//新图中顶点对应的原图编号，如果适用
	vector<vector<bool> > edge;	//邻接矩阵
	vector<Edge> edges;	//边列表
	HGraph(const int);
	HGraph(const vector<char>&);	//直接读vector
	HGraph(){}	//this should not be called
	vector<char> save();	//序列化，保存进vector
	bool load(const vector<char>&);	//从vector中恢复
	void add(int,int,int);	//添加一条边
};

#endif