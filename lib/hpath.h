/*
 * 表示一条H回路
 */

#ifndef HPATH_H
#define HPATH_H

#include <vector>
using namespace std;

class HPath{
public:
	vector<int> edges;	//边的编号
	HPath(){};
	vector<char> save();	//序列化
	bool load(vector<char>&);	//从vector中读取，返回是否有效
};

#endif
