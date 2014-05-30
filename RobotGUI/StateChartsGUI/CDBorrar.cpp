#include "CDBorrar.h"

//(*InternalHeaders(CDBorrar)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/stattext.h>
//*)

#include <sstream>

#include "Estado.h"
#include "Transicion.h"

using namespace std;
using namespace InterfazSancho;

//(*IdInit(CDBorrar)
const long CDBorrar::ID_COMBO=wxNewId();
const long CDBorrar::ID_OK=wxNewId();
const long CDBorrar::ID_CANCEL=wxNewId();
//*)

BEGIN_EVENT_TABLE(CDBorrar,wxDialog)
	//(*EventTable(CDBorrar)
	//*)
END_EVENT_TABLE()

CDBorrar::CDBorrar(vector<DibujablePtr> &objetos,wxWindow* parent)
{
	//(*Initialize(CDBorrar)
	Create(parent, wxID_ANY,_("Object deletion"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	objs=objetos;
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	vector<wxString> arr(objetos.size());
	//Initialization of the combo box data.
	for (size_t i=0;i<objetos.size();++i) if (IS_DERIVED(objetos[i],Transicion))	{
		TransicionPtr t=static_cast<TransicionPtr>(objetos[i]);
		ostringstream oss("");
		EstadoPtr e;
		t->getSaliente(e);
		oss<<e->getNombre()<<" -> "<<t->getEntranteString();
		arr[i]=_U(oss.str().c_str());
	}	else arr[i]=_U((static_cast<EstadoPtr>(objetos[i]))->getNombre().c_str());
	//This call fails if "objetos" is empty. However, the precondition of this constructor is precisely that it contains two or more objects.
	combo=new wxComboBox(this,ID_COMBO,arr[0],wxDefaultPosition,wxDefaultSize,arr.size(),&arr[0],wxCB_READONLY|wxEXPAND);
	wxSizer *v=new wxBoxSizer(wxVERTICAL);
	wxStaticText *label=new wxStaticText(this,wxID_ANY,_("Select the object to delete: "),wxDefaultPosition,wxDefaultSize,wxEXPAND|wxALIGN_LEFT);
	v->Add(label,0,wxALIGN_CENTRE_VERTICAL);
	v->Add(combo,0,wxEXPAND);
	wxSizer *h=new wxBoxSizer(wxHORIZONTAL);
	h->AddStretchSpacer(1);
	wxButton *ac=new wxButton(this,ID_OK,_("OK"));
	h->Add(ac,1);
	h->AddStretchSpacer(1);
	wxButton *ca=new wxButton(this,ID_CANCEL,_("Cancel"));
	h->Add(ca);
	h->AddStretchSpacer(1);
	v->Add(h);
	SetSizer(v);
	v->Fit(this);
	v->SetSizeHints(this);
	//*)

	Connect(ID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDBorrar::OnOK);
	Connect(ID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDBorrar::OnCancel);
	ShowModal();
}

void CDBorrar::OnOK(wxEvent &event)	{
	int i=combo->GetSelection();
	if (i>=0&&i<static_cast<int>(objs.size()))	{
		selected=objs[i];
		EndModal(wxID_OK);
	}	else	{
		wxMessageBox(_("An object must be selected."),_("Error"),wxICON_ERROR);
		combo->SetFocus();
	}
}

void CDBorrar::OnCancel(wxEvent &event)	{
	selected=DibujablePtr(NULL);
	EndModal(wxID_CANCEL);
}
