/*
 * 定义一个签名
 */

#ifndef HSIGN_H
#define HSIGN_H

#include "hnewgraph.h"
#include "hpath.h"
#include <vector>
#include <string>
using namespace std;

class HSign{
public:
	string nmd5,cmd5;
	HNewGraph ng;
	char chlg[MAXNEWGRAPH];
	//HPath path; 因为HNewGraph里已有存放HPath的地方
	//vector<int> ans2[MAXNEWGRAPH]; 以及对应关系
	bool load(vector<char>&);
	vector<char> save();
};

#endif