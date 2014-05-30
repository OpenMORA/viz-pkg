#include "Instruccion.h"
#include <stdexcept>

using namespace std;
using namespace mrpt::utils;
using namespace InterfazSancho;

IMPLEMENTS_MRPT_OBJECT(Instruccion,CObject,InterfazSancho)

void Instruccion::parseMultiple(const string &str,vector<InstruccionPtr> &vec)	{
	vec.clear();
	if (str.length()==0) return;
	string tmp=str;
	for (;;)	{
		InstruccionPtr ins;
		int i=tmp.find_first_of(';');
		if (i<0)	{
			parse(tmp,ins);
			vec.push_back(ins);
			return;
		}	else	{
			parse(tmp.substr(0,i),ins);
			vec.push_back(ins);
			tmp=tmp.substr(i+1);
		}
	}
}

void Instruccion::parse(const string &str,InstruccionPtr &ins)	{
	static Instruccion i;
	const size_t i1=str.find_first_of(',');
	if (i1==string::npos) throw logic_error("Error parseando la instrucción.\n");
	const size_t i2=str.find_first_of(',',i1+1);
	if (i2==string::npos) throw logic_error("Error parseando la instrucción.\n");
	i.nombreVisible=str.substr(0,i1);
	i.nombreReal=str.substr(i1+1,i2);
	parseLista(str.substr(i2+1),i.listaTipos);
	ins=InstruccionPtr(new Instruccion(i));
}

void Instruccion::parseLista(const string &str,vector<string> &vec)	{
	if (str.length()<=0) throw logic_error("Error parseando la instrucción.\n");
	if ((str[0]!='[')||str[str.length()-1]!=']') throw logic_error("Error parseando la instrucción.\n");
	vec.clear();
	if (str.length()==2) return;
	string tmp=str.substr(1,str.length()-1);
	for (;;)	{
		const size_t i=tmp.find_first_of(',');
		if (i==string::npos)	{
			vec.push_back(tmp);
			return;
		}	else	{
			vec.push_back(tmp.substr(0,i));
			tmp=tmp.substr(i+1);
		}
	}
}

void ParametroInstruccion::getPorTipo(const vector<ParametroInstruccion> &in,const string &t,vector<ParametroInstruccion> &out)	{
	out.clear();
	out.reserve(in.size());
	for (vector<ParametroInstruccion>::const_iterator it=in.begin();it!=in.end();it++) if (it->tipo==t) out.push_back(*it);
}

void ParametroInstruccion::parseMultiple(const string &s,vector<ParametroInstruccion> &vec)	{
	vec.clear();
	string tmp=s;
	if (tmp.length()<=0) return;
	else for (;;)	{
		int i=tmp.find_first_of(';');
		vec.resize(vec.size()+1);
		if (i<0)	{
			parse(tmp,vec[vec.size()-1]);
			return;
		}	else	{
			parse(tmp.substr(0,i),vec[vec.size()-1]);
			tmp=tmp.substr(i+1);
		}
	}
}

void ParametroInstruccion::parse(const string &s,ParametroInstruccion &par)	{
	int i1=s.find_first_of(',');
	if (i1<0) throw std::logic_error("Error parseando la instrucción.\n");
	int i2=s.find_first_of(',',i1+1);
	if (i2<0) throw std::logic_error("Error parseando la instrucción.\n");
	par.nombreVisible=s.substr(0,i1);
	par.nombreReal=s.substr(i1+1,i2);
	par.tipo=s.substr(i2+1);
}
