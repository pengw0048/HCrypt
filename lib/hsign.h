/*
 * ����һ��ǩ��
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
	//HPath path; ��ΪHNewGraph�����д��HPath�ĵط�
	//vector<int> ans2[MAXNEWGRAPH]; �Լ���Ӧ��ϵ
	bool load(vector<char>&);
	vector<char> save();
};

#endif