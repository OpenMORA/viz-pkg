#include "TextoYCombo.h"

//(*InternalHeaders(TextoYCombo)
#include <wx/intl.h>
#include <wx/string.h>
#include <algorithm>
//*)

#include <mrpt/gui/WxUtils.h>

#include "Acciones.h"

using namespace std;
using namespace InterfazSancho;

//(*IdInit(TextoYCombo)
const long TextoYCombo::ID_STATICTEXT1 = wxNewId();
const long TextoYCombo::ID_CHOICE1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(TextoYCombo,wxPanel)
	//(*EventTable(TextoYCombo)
	//*)
END_EVENT_TABLE()

TextoYCombo::TextoYCombo(const string &t,wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size):tipo(t)
{
	//(*Initialize(TextoYCombo)
	wxFlexGridSizer* FlexGridSizer1;
	
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _U(tipo.c_str()), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Choice1 = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	FlexGridSizer1->Add(Choice1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	//*)

	//This code refills the parameter list with those belonging to the control's parameter type.
	vector<ParametroInstruccion> todos;
	AccionRobot::getArgumentosDisponibles(todos);
	ParametroInstruccion::getPorTipo(todos,tipo,params);
	for (vector<ParametroInstruccion>::iterator it=params.begin();it!=params.end();it++) Choice1->Append(_U(it->nombreVisible.c_str()),&(*it));
}

bool TextoYCombo::getSeleccion(ParametroInstruccion &pi) const	{
	int r=Choice1->GetCurrentSelection();
	if (r==wxNOT_FOUND) return false;
	ParametroInstruccion *p=static_cast<ParametroInstruccion *>(Choice1->GetClientData(r));
	if (p)	{
		pi=*p;
		return true;
	}	else return false;
}

bool TextoYCombo::setSeleccion(const ParametroInstruccion &pi)	{
	vector<ParametroInstruccion>::const_iterator b=params.begin(),e=params.end(),it=find(b,e,pi);
	if (it==e) return false;
	Choice1->SetSelection(it-b);
	return true;
}
