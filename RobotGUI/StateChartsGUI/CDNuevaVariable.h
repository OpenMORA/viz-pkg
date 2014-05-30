#ifndef CDNUEVAVARIABLE_H
#define CDNUEVAVARIABLE_H

//(*Headers(CDNuevaVariable)
#include <wx/dialog.h>
#include <wx/textctrl.h>
//*)

#include "Main.h"

namespace InterfazSancho	{
/**
  * This simple dialog box is used to create a new variable. Creting the dialog automatically calls ShowModal, and pressing OK already creates the variable. No 
  * additional external action must be performed.
  * Some parts of this class' skeleton have been created with Codeblocks.
  * \sa Main
  */
class CDNuevaVariable: public wxDialog
{
	public:

		CDNuevaVariable(Main* parent);	//!< Constructor. The father is required to be a "Main" object.
		virtual ~CDNuevaVariable()	{}	//!< Virtual destructor.

		//(*Declarations(CDNuevaVariable)
		wxTextCtrl *nombre;	//!< Name of the variable.
		wxTextCtrl *valor;	//!< Initial value of the variable.
		//*)

	protected:

		//(*Identifiers(CDNuevaVariable)
		const static long ID_NOMBRE;
		const static long ID_VALOR;
		const static long ID_OK;
		const static long ID_CANCEL;
		//*)

	private:

		//(*Handlers(CDNuevaVariable)
		//*)

		Main *panel;	//!< Parent object.

		void OnOK(wxEvent &);
		void OnCancel(wxEvent &);

		DECLARE_EVENT_TABLE()
};
}	//End of namespace
#endif
