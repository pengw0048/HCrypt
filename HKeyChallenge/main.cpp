/*
 * 对于给出的HCert文件，生成一个挑战信息
 * 对于服务器返回的一组新图，生成一个挑战序列
 * 检验服务器给出的一组解，判断身份是否真实
 */

#define CHTIME 24

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/hcert.h"
#include "../lib/haction.h"
#include "../lib/hnewgraph.h"
#include "../lib/vstream.h"

void exWrite(char* filename,vector<char> v){
	FILE *fp=fopen(filename,"w");
	writeVector(fp,v);
	fclose(fp);
	exit(0);
}

void exWrite(char* filename,char* str){
	vector<char> v;
	while(*str)
		v.push_back(*(str++));
	exWrite(filename,v);
}

int main(int argc,char* argv[]){
	srand(time(NULL));
	if(argc==4&&strcmp(argv[1],"-s")==0){
		FILE *fp=fopen(argv[3],"rb");
		if(fp==NULL)
			exWrite(argv[2],"File open error.\n");
		HCert cert;
		cert.load(readVector(fp));
		fclose(fp);
		if(cert.valid==false)
			exWrite(argv[2],"HCert file not valid.\n");
		string trait=hgraphTrait(cert.graph);
		char buf[40];
		for(int i=0;i<32;i++)
			buf[i]=trait[i];
		sprintf(buf+32,"%d",CHTIME);
		exWrite(argv[2],buf);
	}else if(argc==5&&strcmp(argv[1],"-c")==0){
		FILE *fp=fopen(argv[4],"rb");
		if(fp==NULL)
			exWrite(argv[2],"File open error.\n");
		HNewGraph ng;
		if(ng.load(readVector(fp))==false)
			exWrite(argv[2],"HNewGraph file not valid.\n");
		fclose(fp);
		string trait=ng.md5;
		char buf[35+CHTIME];
		for(int i=0;i<32;i++)
			buf[i]=trait[i];
		for(int i=0;i<CHTIME;i++)
			buf[32+i]='0'+rand()%2;
		buf[32+CHTIME]=0;
		fp=fopen(trait.c_str(),"wb");
		for(int i=0;i<CHTIME;i++)
			fputc(buf[32+i],fp);
		fclose(fp);
		exWrite(argv[2],buf);
	}else if(argc==6&&strcmp(argv[1],"-v")==0){
		FILE *fp=fopen(argv[3],"rb");
		if(fp==NULL)
			exWrite(argv[2],"File open error.\n");
		HCert cert;
		cert.load(readVector(fp));
		fclose(fp);
		if(cert.valid==false)
			exWrite(argv[2],"HCert file not valid.\n");
		fp=fopen(argv[4],"rb");
		if(fp==NULL)
			exWrite(argv[2],"File open error.\n");
		HNewGraph ng;
		if(ng.load(readVector(fp))==false)
			exWrite(argv[2],"HNewGraph file not valid.\n");
		fclose(fp);
		fp=fopen(ng.md5.c_str(),"rb");
		if(fp==NULL)
			exWrite(argv[2],"Challenge record not found.\n");
		char cseq[CHTIME];
		for(int i=0;i<CHTIME;i++)
			cseq[i]=fgetc(fp);
		fclose(fp);
		fp=fopen(argv[5],"rb");
		if(fp==NULL)
			exWrite(argv[2],"File open error.\n");
		VStream reader(readVector(fp));
		fclose(fp);
		if(reader.vec.size()!=cert.graph.n*CHTIME*2)
			exWrite(argv[2],"File not valid.\n");
		for(int i=0;i<CHTIME;i++){
			int ans[300];
			for(int j=0;j<cert.graph.n;j++)
				ans[j]=reader.readShort();
			if(cseq[i]=='0'){
				for(int j=0;j<cert.graph.n;j++)
					for(int k=0;k<cert.graph.n;k++)
						if(cert.graph.edge[ans[j]][ans[k]]!=ng.ngraph[i].edge[j][k])
							exWrite(argv[2],"File not valid.\n");
			}else{
				HPath tpath;
				for(int j=0;j<cert.graph.n;j++)
					tpath.edges.push_back(ans[j]);
				if(validateHPath(ng.ngraph[i],tpath)==false)
					exWrite(argv[2],"File not valid.\n");
			}
		}
		exWrite(argv[2],"OK.\n");
	}else{
		printf("HKeyChallenge\n\nUsage: HKeyChlg [-s {out1} {in1}] [-c {out1} {in1} {in2}] [-v {out1} {in1} {in2} {in3}]\n");
		printf("-s: Generate initial challenge info from HCert file {in1}.\n-c: Generate challenge sequence of {in1}, given new graph set {in2}.\n-v: Verify response of HCert {in1}, new graph set {in2}, solution {in3}.\n\n");
		return -1;
	}
	return 0;
}
