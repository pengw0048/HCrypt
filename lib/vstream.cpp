#include "vstream.h"

VStream::VStream(vector<char> &vec){
	this->vec=vec;
	pos=0;
}

VStream::VStream(){
	pos=0;
}

void VStream::write(const int value){	//big-endian编码
	vec.push_back((char)((value>>24)&0xff));
	vec.push_back((char)((value>>16)&0xff));
	vec.push_back((char)((value>>8)&0xff));
	vec.push_back((char)((value)&0xff));
}

void VStream::write(const short value){	//big-endian编码
	vec.push_back((char)((value>>8)&0xff));
	vec.push_back((char)((value)&0xff));
}

void VStream::write(const char value){
	vec.push_back(value);
}

void VStream::write(const char *p,int len){
	write((short)len);
	for(int i=0;i<len;i++)
		vec.push_back(p[i]);
}

void VStream::write(const string value){	//写入一个字符串，首先有2位的长度，正如Java所做的
	write((short)value.length());
	for(size_t i=0;i<value.length();i++)
		vec.push_back(value[i]);
}

int VStream::readInt(){
	int ret=0;
	ret=((unsigned short)readShort())<<16;
	ret|=(unsigned short)readShort();
	return ret;
}

short VStream::readShort(){
	short ret=0;
	ret=((unsigned char)read())<<8;
	ret|=(unsigned char)read();
	return ret;
}

char VStream::read(){
	return vec[pos++];
}

string VStream::readString(){
	string ret="";
	int length=readShort();
	for(int i=0;i<length;i++)
		ret.append(1,vec[pos++]);
	return ret;
}

bool VStream::eof(){
	return pos>=vec.size();
}

void VStream::append(const vector<char> &b){
	vec.insert(vec.end(),b.begin(),b.end());
}
