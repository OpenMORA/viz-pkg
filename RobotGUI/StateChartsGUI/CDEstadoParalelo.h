#ifndef CDESTADOPARALELO_H
#define CDESTADOPARALELO_H

//(*Headers(CDEstadoParalelo)
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
//*)

#include "TabSentencias.h"
#include "EstadoParalelo.h"

namespace InterfazSancho	{
/**
  * This dialog box is used to change the configuration of a parallel state. Note always that a parallel state must have at least two non-null children is incorrect.
  * The constructor of this class will automatically call ShowModal and pressing OK commits the changes, so no further operations are necessary, aside of deleting.
  * Note that the maximum amount of states is currently capped at 20, which is far more than it should be necessary, although it may be changed.
  * The skeleton of this class is partly generated by Codeblocks.
  * \sa EstadoParalelo
  */
class CDEstadoParalelo: public wxDialog
{
	public:

		CDEstadoParalelo(const EstadoParaleloPtr &ep,wxWindow* parent);	//!< Constructor.
		virtual ~CDEstadoParalelo()	{}	//!< Virtual destructor.

		//(*Declarations(CDEstadoParalelo)
		wxSpinCtrl *adj;	//!< Control for the amount of children.
		wxTextCtrl *nombreEstado;	//!< Text control for the state's name.
		TabSentencias *tabCodigo;	//!< Panel to configurate the sentences to execute on state entry and/or exit. \sa Accion, TabSentencias
		//*)

	protected:

		//(*Identifiers(CDEstadoParalelo)
		const static long ID_ADJ;
		const static long ID_NOMBREESTADO;
		const static long ID_OK;
		const static long ID_CANCEL;
		//*)

	private:

		//(*Handlers(CDEstadoParalelo)
		void OnOK(wxEvent &);
		void OnCancel(wxEvent &);
		//*)

		EstadoParaleloPtr estado;	//Pointer to the state which this dialog is configuring.

		DECLARE_EVENT_TABLE()
};
}	//End of namespace
#endif
