/*
 * ����VStream������vector<char>�ж�д��������(ֻ�ܶ���д֮һ��)
 */

#ifndef VSTREAM_H
#define VSTREAM_H

#include <vector>
#include <string>
using namespace std;

class VStream{
public:
	vector<char> vec;	//����Ӧ��vector
	VStream();
	VStream(vector<char>&);
	size_t pos;	//Ŀǰλ�ã�ֻ�����ڶ�ȡģʽ��дģʽ����vec.size()
	void write(const int);	//дint(4byte)
	void write(const short);	//дshort(2byte)
	void write(const char);	//дchar(1byte)
	void write(const char*,int);	//дchar����������ȣ���ͷ����2byte��ʾ����
	void write(const string);	//дstring����ͷ����2byte��ʾ����
	int readInt();	//��int
	short readShort();	//��short
	char read();	//��char
	string readString();	//��string����ͷ����2byte��ʾ����
	bool eof();	//�ж��Ƿ��ȡģʽ�ѵ���β
	void append(const vector<char>&);	//���Դ�vec�󸽼�һ��vector
};

#endif
