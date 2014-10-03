#include "hcert.h"
#include <string.h>
#include "vstream.h"

HCert::HCert(){
	data=NULL;
	datalen=0;
}

HCert::~HCert(){
	if(data!=NULL)
		delete[] data;
}

HCert::HCert(const HCert& b){
	data=new char[b.datalen];
	strcpy(data,b.data);
	datalen=b.datalen;
	graph=b.graph;
}

vector<char> HCert::save(){
	VStream writer;
	vector<char> vgraph=graph.save();
	writer.write("HCERT");
	int p1=writer.vec.size();
	writer.write((short)(p1+4));
	writer.write((short)vgraph.size());
	writer.append(vgraph);
	writer.write(data,datalen);
	writer.write("END");
	return writer.vec;
}

bool HCert::load(const vector<char> &vec){
	VStream reader((vector<char>)vec);
	if(reader.pos+19>=reader.vec.size()||reader.readString()!="HCERT")
		return valid=false;
	int start=reader.readShort();
	int len=reader.readShort();
	if(start+len>=reader.vec.size())
		return valid=false;
	vector<char> tmp(vec.begin()+start,vec.begin()+start+len);
	bool res=graph.load(tmp);
	if(res==false)
		return valid=false;
	reader.pos=start+len;
	start=start+len+2;
	len=reader.readShort();
	if(start+len+2>=reader.vec.size())
		return valid=false;
	datalen=len;
	if(data!=NULL)
		delete[] data;
	data=new char[len];
	for(int i=0;i<len;i++)
		data[i]=vec[start+i];
	reader.pos=start+len;
	if(reader.pos>=reader.vec.size()||reader.readString()!="END")
		return valid=false;
	return valid=true;
}
