/*
 * 定义VStream类来在vector<char>中读写基本类型(只能读或写之一！)
 */

#ifndef VSTREAM_H
#define VSTREAM_H

#include <vector>
#include <string>
using namespace std;

class VStream{
public:
	vector<char> vec;	//所对应的vector
	VStream();
	VStream(vector<char>&);
	size_t pos;	//目前位置，只适用于读取模式。写模式请用vec.size()
	void write(const int);	//写int(4byte)
	void write(const short);	//写short(2byte)
	void write(const char);	//写char(1byte)
	void write(const char*,int);	//写char数组给定长度，开头带有2byte表示长度
	void write(const string);	//写string，开头带有2byte表示长度
	int readInt();	//读int
	short readShort();	//读short
	char read();	//读char
	string readString();	//读string，开头带有2byte表示长度
	bool eof();	//判断是否读取模式已到结尾
	void append(const vector<char>&);	//在自带vec后附加一个vector
};

#endif
