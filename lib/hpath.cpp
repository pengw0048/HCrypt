#include "hpath.h"
#include <string>
#include "vstream.h"
using namespace std;

vector<char> HPath::save(){
	VStream writer;
	writer.write("HPATH");
	writer.write((short)edges.size());
	for(size_t i=0;i<edges.size();i++)
		writer.write((short)edges[i]);
	writer.write("END");
	return writer.vec;
}

bool HPath::load(vector<char> &vec){
	bool valid;
	VStream reader((vector<char>)vec);
	if(reader.pos+9>=reader.vec.size()||reader.readString()!="HPATH")
		return valid=false;
	int n=(unsigned short)reader.readShort();
	if(reader.pos+2*n>=reader.vec.size())
		return valid=false;
	for(int i=0;i<n;i++)
		edges.push_back((unsigned short)reader.readShort());
	if(reader.readString()!="END")
		return valid=false;
	return valid=true;
}
