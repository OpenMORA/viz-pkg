#ifndef CDPEDIRESTADODESTINO_H
#define CDPEDIRESTADODESTINO_H

//(*Headers(CDPedirEstadoDestino)
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
//*)

#include <string>

namespace InterfazSancho	{
class PanelGraf;
/**
  * This dialog box is called whenever, for any reason, the user must select one of the existing states. It contains only a combo box and an OK/Cancel pair of buttons.
  * This dialog automatically calls ShowModal when created, but no action is performed on OK. After creating the dialog, a call to getSelection must be performed to
  * get the selected state and act accordingly. The returned string will be empty if Cancel has been pressed.
  * The skeleton of this class is partly Codeblocks generated.
  * \sa PanelGraf, Estado
  */
class CDPedirEstadoDestino: public wxDialog
{
	public:
		/**
		  * Constructor. The dialog box won't hace a cancel button if the boolean parameter is set to true.
		  * Note that the parent dialog must be a PanelGraf object.
		  */
		CDPedirEstadoDestino(PanelGraf *parent,const wxString &t=_(""),bool conCancel=false);
		virtual ~CDPedirEstadoDestino()	{}	//!< Virtual, empty constructor.
		inline std::string getSelection() const	{	//!< Gets the name of the selected state. Call PanelGraf::getEstadoPorNombre to obtain a smart pointer to the actual state.
			return selection;
		}

		//(*Declarations(CDPedirEstadoDestino)
		wxComboBox *combo;	//!< Combo box to choose between the existing states.
		//*)

	protected:

		//(*Identifiers(CDPedirEstadoDestino)
		const static long ID_COMBO;
		const static long ID_OK;
		const static long ID_CANCEL;
		//*)

	private:

		//(*Handlers(CDPedirEstadoDestino)
		//*)

		PanelGraf *panel;	//!< Parent object, used mainly to get a list of the existing states.
		void OnOK(wxEvent &);
		void OnCancel(wxEvent &);
		std::string selection;	//!< Selected string, updated only when the user presses OK.

		DECLARE_EVENT_TABLE()
};
}	//End of namespace
#endif
