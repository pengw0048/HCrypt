/*
 * 检验文件签署有效性
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/hcert.h"
#include "../lib/hgraph.h"
#include "../lib/hnewgraph.h"
#include "../lib/haction.h"
#include "../lib/hsign.h"
#include <string>
using namespace std;

int main(int argc,char* argv[]){
	if(argc!=4){
		printf("HKeyVerify\n\nUsage: HKeyVeri {in1} {in2} {CertStore}\nVerify signature file {in2} of {in1}, looking for HCert in {CertStore}.\n");
		return -1;
	}
	char buf[500],nmd5[33];
	sprintf(buf,"fciv \"%s\" > md5.tmp",argv[1]);
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
	fp=fopen(argv[2],"rb");
	if(fp==NULL){
		printf("File open error.\n");
		return -1;
	}
	HSign sign;
	if(sign.load(readVector(fp))==false){
		printf("HSign file not valid.\n");
		return -1;
	}
	fclose(fp);
	if(sign.nmd5!=string(nmd5)){
		printf("HSign file not for this file.\n");
		return -1;
	}
	char hexsum[36];
	vector<char> tv=sign.ng.save();
	char *buff=new char[tv.size()+40];
	for(int i=0;i<tv.size();i++)
		buff[i]=tv[i];
	strcpy(buff+tv.size(),nmd5);
	md5_hash(buff,tv.size()+40,hexsum);
	delete[] buff;
	for(int i=0;i<sign.ng.count;i++)
		if(sign.chlg[i]!=hexsum[i]%2+'0'){
			printf("HSign file malformatted.\n");
			return -1;
		}
	sprintf(buf,"%s\\cert\\%s.hcert",argv[3],sign.cmd5.c_str());
	HCert cert;
	fp=fopen(buf,"rb");
	if(fp==NULL){
		printf("Corresponding HCert file not found.\n");
		return -1;
	}
	cert.load(readVector(fp));
	fclose(fp);
	for(int i=0;i<sign.ng.count;i++){
		if(sign.chlg[i]=='0'){
			for(int j=0;j<cert.graph.n;j++)
				for(int k=0;k<cert.graph.n;k++)
					if(cert.graph.edge[sign.ng.ngraph[i].vnumber[j]][sign.ng.ngraph[i].vnumber[k]]!=sign.ng.ngraph[i].edge[j][k]){
						printf("File not valid.\n");
						return -1;
					}
		}else{
			if(validateHPath(sign.ng.ngraph[i],sign.ng.npath[i])==false){
				printf("File not valid.\n");
				return -1;
			}
		}
	}
	printf("OK.\nHCert info: ");
	if(cert.datalen>50)cert.datalen=50;
	for(int i=0;i<cert.datalen;i++)
		printf("%c",cert.data[i]);
	printf("\n");
	return 0;
}
