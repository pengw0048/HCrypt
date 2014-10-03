/*
 * 给文件（实际为MD5）用特定的证书签署
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../lib/hcert.h"
#include "../lib/hgraph.h"
#include "../lib/hnewgraph.h"
#include "../lib/haction.h"
#include "../lib/hsign.h"
#include <string>
using namespace std;

#define CHTIME 24

int main(int argc,char* argv[]){
	srand(time(NULL));
	if(argc!=5){
		printf("HKeySign\n\nUsage: HKeySign {out} {in1} {in2} {in3}\nSign {in1} with HCert {in2} and HKey {in3}, write to {out}.\n");
		return -1;
	}
	char buf[500],nmd5[33];
	sprintf(buf,"fciv \"%s\" > md5.tmp",argv[2]);
	system(buf);
	FILE *fp=fopen("md5.tmp","r");
	if(fp==NULL){
		printf("File open error.\n");
		return -1;
	}
	fgets(buf,500,fp);
	fgets(buf,500,fp);
	fgets(buf,500,fp);
	buf[32]=0;
	fgets(buf,500,fp);
	if(buf[32]!=' '||buf[31]<'0'||buf[31]>'f'){
		printf("File open error.\n");
		return -1;
	}
	buf[32]=0;
	strcpy(nmd5,buf);
	fclose(fp);
	fp=fopen(argv[3],"rb");
	if(fp==NULL){
		printf("This cert not fount.\n");
		return -1;
	}
	HNewGraph ng;
	HCert cert;
	cert.load(readVector(fp));
	fclose(fp);
	fp=fopen(argv[4],"rb");
	if(fp==NULL){
		printf("This key not fount.\n");
		return -1;
	}
	ng.path.load(readVector(fp));
	fclose(fp);
	ng.graph=cert.graph;
	ng.count=CHTIME;
	for(int i=0;i<CHTIME;i++)
		ng.ngraph[i]=scramble(ng.graph,ng.path,ng.npath[i]);
	HSign sign;
	sign.ng=ng;
	sign.nmd5=string(nmd5);
	sign.cmd5=hgraphTrait(cert.graph);
	char hexsum[36];
	vector<char> tv=sign.ng.save();
	char *buff=new char[tv.size()+40];
	for(int i=0;i<tv.size();i++)
		buff[i]=tv[i];
	strcpy(buff+tv.size(),nmd5);
	md5_hash(buff,tv.size()+40,hexsum);
	delete[] buff;
	for(int i=0;i<CHTIME;i++)
		sign.chlg[i]=hexsum[i]%2+'0';
	fp=fopen(argv[1],"wb");
	if(fp==NULL){
		printf("Cannot write to file.\n");
		return -1;
	}
	writeVector(fp,sign.save());
	fclose(fp);
	return 0;
}
