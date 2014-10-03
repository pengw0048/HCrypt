#include "haction.h"
#include "md5.h"
#include <stdio.h>
#include <stdlib.h>

string hgraphTrait(HGraph &graph){
	vector<char> vec=graph.save();
	char hexsum[36];
	char *buff=new char[vec.size()];
	for(size_t i=0;i<vec.size();i++)
		buff[i]=vec[i];
	md5_hash(buff,vec.size(),hexsum);
	delete[] buff;
	return string(hexsum);
}

bool validateHPath(HGraph& graph,HPath& path){
	if(graph.valid==false||path.edges.size()!=graph.n)
		return false;
	for(int i=0;i<graph.n;i++)
		if(path.edges[i]<0||path.edges[i]>=graph.m)
			return false;
	if(graph.edges[path.edges[0]].from!=graph.edges[path.edges[graph.n-1]].to)
		return false;
	bool *mark=new bool[graph.n],ret=true;
	for(int i=0;i<graph.n;i++)
		mark[i]=false;
	for(int i=0;i<graph.n;i++){
		if(mark[graph.edges[path.edges[i]].to]==true){
			ret=false;
			break;
		}
		mark[graph.edges[path.edges[i]].to]=true;
	}
	delete[] mark;
	return ret;
}


void writeVector(FILE *fp,vector<char> &vec){
	for(size_t i=0;i<vec.size();i++)
		fputc(vec[i],fp);
}

vector<char> readVector(FILE *fp){
	vector<char> ret;
	int c;
	while((c=fgetc(fp))!=EOF)
		ret.push_back((char)c);
	return ret;
}

inline void swap(int &a,int &b){
	int t=a;
	a=b;
	b=t;
}

HGraph scramble(HGraph &graph,HPath &path,HPath &npath){
	HGraph ret(graph.n),ret2;
	ret.m=graph.m;
	int *seq=new int[ret.n];	//原点i在新图中的下标为seq[i]
	for(int i=0;i<ret.n;i++)
		seq[i]=i;
	for(int i=ret.n*20;i>=0;i--)
		swap(seq[rand()%ret.n],seq[rand()%ret.n]);
	for(int i=0;i<ret.n;i++)
		ret.vnumber[seq[i]]=i;
	for(int i=0;i<ret.m;i++)
		ret.add(seq[graph.edges[i].from],seq[graph.edges[i].to],i);
	delete[] seq;
	seq=new int[ret.m];	//原边i在新图中的下标为seq[i]
	for(int i=0;i<ret.m;i++)
		seq[i]=i;
	for(int i=ret.m*10;i>=0;i--)
		swap(seq[rand()%ret.m],seq[rand()%ret.m]);
	for(int i=0;i<ret.m;i++)
		ret.edges[i].number=seq[i];
	for(int i=0;i<path.edges.size();i++)
		npath.edges.push_back(seq[path.edges[i]]);
	delete[] seq;
	ret2=HGraph(ret.save());
	for(int i=0;i<ret.n;i++)
		ret2.vnumber.push_back(ret.vnumber[i]);
	return ret2;
}
