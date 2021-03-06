#ifndef CDESTADOCOMPUESTO_H
#define CDESTADOCOMPUESTO_H

//(*Headers(CDEstadoCompuesto)
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
//*)

#include "TabSentencias.h"
#include "EstadoCompuesto.h"


namespace InterfazSancho	{
/**
  * This dialog box manages the configuration of an EstadoCompuesto (compound state) object.
  * ShowModal is automatically called and pressing OK commits the changes, so the calling method does not need to worry about anything aside deletion.
  * The skeleton of this class is partly generated by Codeblocks.
  * \sa EstadoCompuesto
  */
class CDEstadoCompuesto: public wxDialog
{
	public:

		CDEstadoCompuesto(const EstadoCompuestoPtr &e,wxWindow* parent);	//!< Constructor.
		virtual ~CDEstadoCompuesto()	{}	//!< Virtual destructor.

		//(*Declarations(CDEstadoCompuesto)
		wxTextCtrl *nombreEstado;	//!< Text control to manage the name of the state.
		wxComboBox *combo;	//!< Combo box used to select the initial state.
		TabSentencias *tabCodigo;	//!< Panel containing the information relative to the sentences to be executed on entry and/or exit. \sa Accion, TabSentencias
		//*)

	protected:

		//(*Identifiers(CDEstadoCompuesto)
		static const long ID_NOMBREESTADO;
		static const long ID_COMBO;
		static const long ID_OK;
		static const long ID_CANCEL;
		//*)

	private:

		//(*Handlers(CDEstadoCompuesto)
		void OnOK(wxEvent &);
		void OnCancel(wxEvent &);
		//*)
		EstadoCompuestoPtr estado;	//!< The state to which this dialog box refers. Pressing OK in the dialog automatically commits the changes into the state.
		DECLARE_EVENT_TABLE()
};
}	//End of namespace
#endif
