#include "hgraph.h"
#include <string>
#include <algorithm>
#include "vstream.h"
using namespace std;

Edge::Edge(const int from,const int to,const int number){
	this->from=from;
	this->to=to;
	this->number=number;
}

HGraph::HGraph(const int n){
	this->n=n;
	edge=vector<vector<bool> >(n,vector<bool>(n,false));
	for(int i=0;i<n;i++)
		vnumber.push_back(i);
	valid=true;
}

HGraph::HGraph(const vector<char> &vec){
	load(vec);
}

vector<char> HGraph::save(){	//返回包含序列化结果的char数组（vector<char>）
	VStream writer;
	writer.write("HGRAPH");
	writer.write((short)n);	//用边列表记录
	writer.write((short)m);
	for(int i=0;i<m;i++){
		writer.write((short)(edges[i].from));
		writer.write((short)(edges[i].to));
		writer.write((short)(edges[i].number));
	}
	writer.write("END");
	return writer.vec;
}

bool HGraph::load(const vector<char> &vec){
	VStream reader((vector<char>)vec);
	if(reader.pos+12>=reader.vec.size()||reader.readString()!="HGRAPH")
		return valid=false;
	n=(unsigned short)reader.readShort();
	m=(unsigned short)reader.readShort();
	if(reader.pos+6*m>=reader.vec.size())
		return valid=false;
	edges.clear();
	edge=vector<vector<bool> >(n,vector<bool>(n,false));
	for(int i=0;i<m;i++){
		Edge tedge;
		tedge.from=(unsigned short)reader.readShort();
		tedge.to=(unsigned short)reader.readShort();
		tedge.number=(unsigned short)reader.readShort();
		edges.push_back(tedge);
		edge[tedge.from][tedge.to]=true;
	}
	if(reader.readString()!="END")
		return valid=false;
	//现在需要把边列表按照边的编号排序，以防万一
	sort(edges.begin(),edges.end());
	return valid=true;
}

void HGraph::add(int from,int to,int num){
	Edge tedge;
	tedge.from=from;
	tedge.to=to;
	tedge.number=num;
	edges.push_back(tedge);
	edge[from][to]=true;
}
