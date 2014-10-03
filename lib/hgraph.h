/*
 * ����Hͼ�ࣺHGraph
 */

#ifndef HGRAPH_H
#define HGRAPH_H

#include <vector>
#include "md5.h"
using namespace std;

struct Edge{	//��ʾһ�������
	int from,to,number;
	Edge(){};
	Edge(const int,const int,const int);	//ֱ�Ӹ�ֵfrom,to,number
	bool operator <(const Edge &b)const{
		return number<b.number;
	}
};

class HGraph{
public:
	int n,m;	//����������
	bool valid;	//��ȡʱ��ʾ�Ƿ���Ч
	vector<int> vnumber;	//��ͼ�ж����Ӧ��ԭͼ��ţ��������
	vector<vector<bool> > edge;	//�ڽӾ���
	vector<Edge> edges;	//���б�
	HGraph(const int);
	HGraph(const vector<char>&);	//ֱ�Ӷ�vector
	HGraph(){}	//this should not be called
	vector<char> save();	//���л��������vector
	bool load(const vector<char>&);	//��vector�лָ�
	void add(int,int,int);	//���һ����
};

#endif