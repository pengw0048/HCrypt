/*
 * ������Hͼ�Ĳ����ĺ���
 */

#ifndef HACTION_H
#define HACTION_H

#include "hgraph.h"
#include "hpath.h"
#include <string>
#include <vector>
using namespace std;

string hgraphTrait(HGraph&);	//����HGraph������Hash
bool validateHPath(HGraph&,HPath&);	//��֤HPath�Ƿ���HGraph��H��·
void writeVector(FILE*,vector<char>&);	//��vectorд��FILE
vector<char> readVector(FILE*);	//��FILEȫ�����ݣ����غ������ݵ�vector
HGraph scramble(HGraph&,HPath&,HPath&);	//����HGraph��������֪��HPath��������ͼ���»�·���Դ����÷�ʽ��

#endif
