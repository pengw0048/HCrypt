/*
 * 生成一个的H图及其回路，存储到证书文件
 */

#define MAXN 100
#define MINEDGE 200
#define MAXEDGE 300
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../lib/hgraph.h"
#include "../lib/hpath.h"
#include "../lib/haction.h"
#include "../lib/hcert.h"
#include "../lib/md5.h"
#include <vector>
using namespace std;

int rnd[MAXEDGE+MAXN*2];
bool mark[MAXN][MAXN]={0};

int cmp(const void *a,const void *b){
	return rnd[*(int*)a]<rnd[*(int*)b];
}

int main(int argc,char* argv[]){
	if(argc!=3){
		printf("HKeygen\n\nUsage: HKeygen {in} {out}\n{in} - Additional data file.\n{out} - Output file name. Do not add any extension.\n        eg. mykey1\n");
		return -1;
	}
	printf("Initializing environment...\n");
	FILE *fin=fopen(argv[1],"rb");
	if(fin==NULL){
		printf("Input file open error - %s",argv[1]);
		return -1;
	}
	char buf[1024];
	strcpy(buf,argv[2]);
	strcat(buf,".hcert");
	FILE *fout=fopen(buf,"wb");
	if(fout==NULL){
		printf("Output file open error - %s",argv[1]);
		fclose(fin);
		return -1;
	}
	fseek(fin,0,SEEK_END);
	size_t inlen=ftell(fin);
	fseek(fin,0,SEEK_SET);
	char *infile=new char[inlen];
	fread(infile,1,inlen,fin);
	fclose(fin);
	printf("Generating graph...\n");
	HGraph graph(MAXN);
	HPath path;
	srand(time(NULL));
	int seq[MAXEDGE+MAXN*2+1];	//回路的点的顺序
	for(int i=0;i<MAXN;i++){
		seq[i]=i;
		rnd[i]=rand();
	}
	qsort(seq,MAXN,sizeof(int),cmp);
	seq[MAXN]=seq[0];
	for(int i=0;i<MAXN;i++){
		graph.add(seq[i],seq[i+1],i);
		path.edges.push_back(i);
	}
	int edgecount=MINEDGE+rand()%(MAXEDGE-MINEDGE+1);
	//先随机添加边。这不是一个高效的方法，但对于稀疏图，还可以
	for(int i=MAXN;i<edgecount;i++){
		int from,to;
		do{
			from=rand()%MAXN;
			to=rand()%MAXN;
		}while(to==from||mark[from][to]==true);
		mark[from][to]=true;
		graph.add(from,to,i);
	}
	//给度为2的点帮个忙
	for(int i=0;i<MAXN;i++){
		int to;
		do{
			to=rand()%MAXN;
		}while(to==i||mark[i][to]==true);
		mark[i][to]=true;
		graph.add(i,to,graph.edges.size());
		int from;
		do{
			from=rand()%MAXN;
		}while(from==i||mark[from][i]==true);
		mark[from][i]=true;
		graph.add(from,i,graph.edges.size());
	}
	graph.m=graph.edges.size();
	//还需要随机打乱边的编号。此时seq是映射表
	for(int i=0;i<graph.m;i++){
		seq[i]=i;
		rnd[i]=rand();
	}
	qsort(seq,graph.m,sizeof(int),cmp);
	for(size_t i=0;i<path.edges.size();i++)
		path.edges[i]=seq[path.edges[i]];
	for(int i=0;i<graph.m;i++)
		graph.edges[i].number=seq[graph.edges[i].number];
	//为了重建邻接矩阵，不如直接这样...虽然暂时不用它
	graph=HGraph(graph.save());
	printf("Validating this H-path...");
	if(validateHPath(graph,path)==true)
		printf("success\n");
	else{
		printf("failed!\n**Fatal error. Look for bugs.\n");
		return -1;
	}
	printf("Writing files...\n");
	HCert cert;
	cert.data=infile;
	cert.datalen=inlen;
	cert.graph=graph;
	writeVector(fout,cert.save());
	fclose(fout);
	char buf2[1024];
	strcpy(buf2,argv[2]);
	strcat(buf2,".hkey");
	fout=fopen(buf2,"wb");
	writeVector(fout,path.save());
	fclose(fout);
	printf("All tasks have finished successfully.\nHCert file (public): %s\nHKey file (private): %s\nTrait: %s\n",buf,buf2,hgraphTrait(cert.graph).c_str());
	return 0;
}
