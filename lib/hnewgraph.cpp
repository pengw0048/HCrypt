#include "hnewgraph.h"
#include "vstream.h"
#include "md5.h"

vector<char> HNewGraph::save(){
	VStream writer;
	writer.write("HNEWGRAPH");
	writer.write((short)count);
	for(int i=0;i<count;i++){
		vector<char> nv=ngraph[i].save();
		writer.write((short)(nv.size()));
		writer.append(nv);
	}
	char *buf=new char[writer.vec.size()+2];
	for(int i=0;i<writer.vec.size();i++)
		buf[i]=writer.vec[i];
	char hexsum[36];
	md5_hash(buf,writer.vec.size(),hexsum);
	md5=string(hexsum);
	delete[] buf;
	writer.write(hexsum,32);
	writer.write("END");
	return writer.vec;
}

vector<char> HNewGraph::savemyans(){
	VStream writer;
	writer.write((short)count);
	writer.write((short)ngraph[0].n);
	for(int i=0;i<count;i++){
		for(int j=0;j<ngraph[i].n;j++)
			writer.write((short)(ngraph[i].vnumber[j]));
		for(int j=0;j<ngraph[i].n;j++)
			writer.write((short)(npath[i].edges[j]));
	}
	return writer.vec;
}

bool HNewGraph::load(const vector<char> &vec){
	VStream reader((vector<char>)vec);
	if(reader.pos+12>=reader.vec.size()||reader.readString()!="HNEWGRAPH")
		return false;
	count=reader.readShort();
	if(count>MAXNEWGRAPH)
		return false;
	int len=(unsigned short)reader.readShort();
	for(int i=0;i<count;i++){
		vector<char> tvec(vec.begin()+ reader.pos,vec.begin()+reader.pos+len);
		if(ngraph[i].load(tvec)==false)
			return false;
		reader.pos+=len+2;
		if(reader.pos+3>=reader.vec.size())
			return false;
	}
	reader.pos-=2;
	if(reader.pos+34>=reader.vec.size())
		return false;
	md5=reader.readString();
	if(reader.readString()!="END")
		return false;
	return true;
}
