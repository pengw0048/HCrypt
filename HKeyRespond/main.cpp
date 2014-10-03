/*
 * 回应挑战信息
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/hnewgraph.h"
#include "../lib/hcert.h"
#include "../lib/haction.h"
#include "../lib/hpath.h"
#include "../lib/vstream.h"

int main(int argc,char* argv[]){
	srand(time(NULL));
	if(argc==5&&strcmp(argv[1],"-a")==0){
		FILE *fp=fopen(argv[3],"rb");
		if(fp==NULL){
			printf("File open error.\n");
			return -1;
		}
		char in[50],buf[300];
		int count;
		fgets(in,50,fp);
		fclose(fp);
		sscanf(in+32,"%d",&count);
		if(count<=0||count>MAXNEWGRAPH){
			printf("File malformatted.\n");
			return -1;
		}
		in[32]=0;
		sprintf(buf,"%s\\cert\\%s.hcert",argv[4],in);
		fp=fopen(buf,"rb");
		if(fp==NULL){
			printf("This cert not fount.\n");
			return -1;
		}
		HNewGraph ng;
		HCert cert;
		cert.load(readVector(fp));
		fclose(fp);
		sprintf(buf,"%s\\key\\%s.hkey",argv[4],in);
		fp=fopen(buf,"rb");
		if(fp==NULL){
			printf("This key not fount.\n");
			return -1;
		}
		ng.path.load(readVector(fp));
		fclose(fp);
		ng.graph=cert.graph;
		ng.count=count;
		for(int i=0;i<count;i++)
			ng.ngraph[i]=scramble(ng.graph,ng.path,ng.npath[i]);
		fp=fopen(argv[2],"wb");
		if(fp==NULL){
			printf("Cannot open out file.\n");
			return -1;
		}
		writeVector(fp,ng.save());
		fclose(fp);
		sprintf(buf,"%s\\response\\%s.resp",argv[4],ng.md5.c_str());
		fp=fopen(buf,"wb");
		if(fp==NULL){
			printf("Cannot save response file.\n");
			return -1;
		}
		writeVector(fp,ng.savemyans());
		fclose(fp);
		return 0;
	}else if(argc==5&&strcmp(argv[1],"-b")==0){
		FILE *fp=fopen(argv[3],"rb");
		if(fp==NULL){
			printf("File open error.\n");
			return -1;
		}
		char in[100],buf[300];
		int count,n;
		fread(in,1,32,fp);
		in[32]=0;
		sprintf(buf,"%s\\response\\%s.resp",argv[4],in);
		FILE *fp2=fopen(buf,"rb");
		if(fp2==NULL){
			printf("Solution not found.\n");
			return -1;
		}
		VStream reader(readVector(fp2));
		fclose(fp2);
		count=reader.readShort();
		n=reader.readShort();
		fp2=fopen(argv[2],"wb");
		VStream writer;
		fseek(fp,32,SEEK_SET);
		for(int i=0;i<count;i++){
			char ch=fgetc(fp);
			int ans1[300],ans2[300];
			for(int j=0;j<n;j++)
				ans1[j]=reader.readShort();
			for(int j=0;j<n;j++)
				ans2[j]=reader.readShort();
			if(ch=='0'){	//对应关系（新点i在原图中的编号）
				for(int j=0;j<n;j++)
					writer.write((short)ans1[j]);
			}else{	//新的H回路
				for(int j=0;j<n;j++)
					writer.write((short)ans2[j]);
			}
		}
		writeVector(fp2,writer.vec);
		fclose(fp2);
		fclose(fp);
		return 0;
	}else{
		printf("HKeyRespond\n\nUsage: HKeyResp [-a {out1} {in1} {CertStore}] [-b {out1} {in1} {CertStore}]\n");
		printf("-a: Respond {in1} with a set of new graphs to {out1}, if available from directory {CertStore}.\n-b: Give solution to {in1}, writing it to {out1}, given {CertStore}.\n\n");
		return -1;
	}
	return 0;
}
