/*
 * ��ʾһ�״��Һ��ͼ
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
	int count;	//ͼ��
	HGraph ngraph[MAXNEWGRAPH];	//��ͼ��
	HGraph graph;	//ԭͼ
	HPath npath[MAXNEWGRAPH];	//�»�·
	HPath path;	//ԭ��·
	string md5;	//��ͼ��md5
	vector<char> save();	//���л�
	vector<char> savemyans();	//����𰸣���Ӧ��ϵ�ͻ�·��
	bool load(const vector<char>&);	//��vector�ж�ȡ�������Ƿ���Ч
};

#endif