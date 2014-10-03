/*
 * 包含对H图的操作的函数
 */

#ifndef HACTION_H
#define HACTION_H

#include "hgraph.h"
#include "hpath.h"
#include <string>
#include <vector>
using namespace std;

string hgraphTrait(HGraph&);	//返回HGraph的特征Hash
bool validateHPath(HGraph&,HPath&);	//验证HPath是否是HGraph的H回路
void writeVector(FILE*,vector<char>&);	//将vector写进FILE
vector<char> readVector(FILE*);	//读FILE全部内容，返回含有内容的vector
HGraph scramble(HGraph&,HPath&,HPath&);	//打乱HGraph，带有已知的HPath，返回新图和新回路（以传引用方式）

#endif
