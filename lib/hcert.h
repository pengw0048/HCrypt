/*
 * ��ʾһ��֤���ļ�HCert
 */

#ifndef HCERT_H
#define HCERT_H

#include "hgraph.h"
#include <vector>
using namespace std;

class HCert{
public:
	bool valid;	//�ڶ�ȡʱ������ʾ�Ƿ���Ч
	char *data;	//��������
	int datalen;	//�������ݳ���
	HGraph graph;	//���е�Hͼ
	HCert();
	~HCert();
	HCert(const HCert&);	//���
	vector<char> save();	//���л��������vector
	bool load(const vector<char>&);	//��vector�ж���HCert�������Ƿ�ɹ�
};

#endif
