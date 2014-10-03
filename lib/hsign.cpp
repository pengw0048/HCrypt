#include "hsign.h"
#include "vstream.h"

bool HSign::load(vector<char> &vec){
	VStream reader((vector<char>)vec);
	if(reader.pos+76>=reader.vec.size()||reader.readString()!="HSIGN")
		return false;
	nmd5=reader.readString();
	cmd5=reader.readString();
	int length=reader.readInt();
	vector<char> tmp(reader.vec.begin()+reader.pos,reader.vec.begin()+reader.pos+length);
	reader.pos+=length;
	if(reader.pos>=reader.vec.size())
		return false;
	if(ng.load(tmp)==false)
		return false;
	for(int i=0;i<ng.count;i++)
		chlg[i]=reader.read();
	for(int i=0;i<ng.count;i++){
		if(reader.pos+ng.ngraph[i].n>=reader.vec.size())
			return false;
		if(chlg[i]=='0')
			for(int j=0;j<ng.ngraph[i].n;j++)
				ng.ngraph[i].vnumber.push_back(reader.readShort());
		else
			for(int j=0;j<ng.ngraph[i].n;j++)
				ng.npath[i].edges.push_back(reader.readShort());
	}
	if(reader.pos+5>reader.vec.size()||reader.readString()!="END")
		return false;
	return true;
}

vector<char> HSign::save(){
	VStream writer;
	writer.write("HSIGN");
	writer.write(nmd5);
	writer.write(cmd5);
	vector<char> tmp=ng.save();
	writer.write((int)tmp.size());
	writer.append(tmp);
	for(int i=0;i<ng.count;i++)
		writer.write(chlg[i]);
	for(int i=0;i<ng.count;i++){
		if(chlg[i]=='0')
			for(int j=0;j<ng.ngraph[i].n;j++)
				writer.write((short)(ng.ngraph[i].vnumber[j]));
		else
			for(int j=0;j<ng.ngraph[i].n;j++)
				writer.write((short)(ng.npath[i].edges[j]));
	}
	writer.write("END");
	return writer.vec;
}
