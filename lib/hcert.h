/*
 * 表示一个证书文件HCert
 */

#ifndef HCERT_H
#define HCERT_H

#include "hgraph.h"
#include <vector>
using namespace std;

class HCert{
public:
	bool valid;	//在读取时用来表示是否有效
	char *data;	//附加数据
	int datalen;	//附加数据长度
	HGraph graph;	//带有的H图
	HCert();
	~HCert();
	HCert(const HCert&);	//深拷贝
	vector<char> save();	//序列化，保存进vector
	bool load(const vector<char>&);	//从vector中读出HCert，返回是否成功
};

#endif
