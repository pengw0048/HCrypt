/*
 * ��ʾһ��H��·
 */

#ifndef HPATH_H
#define HPATH_H

#include <vector>
using namespace std;

class HPath{
public:
	vector<int> edges;	//�ߵı��
	HPath(){};
	vector<char> save();	//���л�
	bool load(vector<char>&);	//��vector�ж�ȡ�������Ƿ���Ч
};

#endif
