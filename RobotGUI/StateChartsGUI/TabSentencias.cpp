#include "TabSentencias.h"

//(*InternalHeaders(TabSentencias)
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/string.h>
//*)

//(*IdInit(TabSentencias)
//*)

BEGIN_EVENT_TABLE(TabSentencias,wxPanel)
	//(*EventTable(TabSentencias)
	//*)
END_EVENT_TABLE()

TabSentencias::TabSentencias(EstadoPtr &e,wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size):estado(e)
{
	//(*Initialize(TabSentencias)
	wxBoxSizer *b;
	vector<AccionPtr> snts;	//Temporary variable for transactions between state and subpanels.

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetMaxSize(wxSize(-1,-1));
	b=new wxBoxSizer(wxHORIZONTAL);
	onentry=new TabSentenciasSimple(estado,_("Actions to take on state entry: "),this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
	estado->getOnentry(snts);
	onentry->setSentencias(snts);
	b->Add(onentry,1,wxEXPAND|wxALIGN_CENTRE_HORIZONTAL|wxALIGN_CENTRE_VERTICAL,1);
	onexit=new TabSentenciasSimple(estado,_("Actions to take on state exit: "),this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
	estado->getOnexit(snts);
	onexit->setSentencias(snts);
	b->Add(onexit,1,wxEXPAND|wxALIGN_CENTRE_HORIZONTAL|wxALIGN_CENTRE_HORIZONTAL,1);
	SetSizer(b);
	b->Fit(this);
	b->SetSizeHints(this);
	//*)
}
