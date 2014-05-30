#include "ContainerMultiple.h"

//(*InternalHeaders(ContainerMultiple)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/string.h>
//*)

#include <stdexcept>

using namespace std;
using namespace InterfazSancho;

//(*IdInit(ContainerMultiple)
const long ContainerMultiple::ID_LABEL=wxNewId();
//*)

BEGIN_EVENT_TABLE(ContainerMultiple,wxPanel)
	//(*EventTable(ContainerMultiple)
	//*)
END_EVENT_TABLE()

ContainerMultiple::ContainerMultiple(const InstruccionPtr &p,wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size):instr(p),tycs(p->listaTipos.size(),NULL)	{
	//(*Initialize(ContainerMultiple)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	wxFlexGridSizer *sizer;
	size_t N=p->listaTipos.size();
	if (N==0)	{
		sizer=new wxFlexGridSizer(1,1,0,0);
		label=new wxStaticText(this,ID_LABEL,_("This instruction doesn't have any parameter."),wxDefaultPosition,wxDefaultSize,0,_("ID_LABEL"));
		sizer->Add(label,wxALIGN_CENTRE_VERTICAL);
	}	else	{
		sizer=new wxFlexGridSizer(N,1,0,0);
		for (size_t i=0;i<N;++i)	{
			tycs[i]=new TextoYCombo(p->listaTipos[i],this);
			sizer->Add(tycs[i]);
		}
	}
	SetSizer(sizer);
	sizer->Fit(this);
	sizer->SetSizeHints(this);
	//*)

}

AccionRobotPtr ContainerMultiple::getAccionRobot() const	{
	vector<ParametroInstruccion> a;
	ParametroInstruccion p;
	for (vector<TextoYCombo *>::const_iterator it=tycs.begin();it!=tycs.end();++it)	{
		(*it)->getSeleccion(p);
		a.push_back(p);
	}
	return AccionRobotPtr(new AccionRobot(instr,a));
}

void ContainerMultiple::setValores(const vector<ParametroInstruccion> &a)	{
	if (a.size()!=tycs.size()) throw logic_error("Error when parsing instruction parameters.");
	for (size_t i=0;i<a.size();++i) tycs[i]->setSeleccion(a[i]);
}
