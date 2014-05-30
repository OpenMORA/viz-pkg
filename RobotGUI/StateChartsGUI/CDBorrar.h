#ifndef CDBORRAR_H
#define CDBORRAR_H

//(*Headers(CDBorrar)
#include <wx/dialog.h>
#include <wx/combobox.h>
//*)

#include "Dibujable.h"

namespace InterfazSancho	{
/**
  * This dialog box is used in a "PanelGraf" panel, whenever the user is trying to delete an object but the point in which (s)he clicked contains more than one object.
  * The user is then prompted to select one of them to delete it. The constructor automatically calls ShowModal, so after construction the only task to do are calling
  * getSelected (which will be a null smart pointer if the user pressed cancel) and then delete the dialog.
  * The skeleton of this class is partly Codeblocks-generated.
  */
class CDBorrar: public wxDialog
{
	public:
		CDBorrar(std::vector<DibujablePtr> &objetos,wxWindow* parent);	//!< Constructor. Automatically calls ShowModal.
		virtual ~CDBorrar()	{}	//!< Virtual destructor

		//(*Declarations(CDBorrar)
		wxComboBox *combo;	//!< Combo box containing the list of objects to possibly delete.
		//*)

		inline DibujablePtr getSelected() const	{	//!< Gets the object to be deleted (deletion takes course outside this class).
			return selected;
		}

	protected:

		//(*Identifiers(CDBorrar)
		const static long ID_COMBO;
		const static long ID_OK;
		const static long ID_CANCEL;
		//*)

	private:

		//(*Handlers(CDBorrar)
		void OnOK(wxEvent &);
		void OnCancel(wxEvent &);
		//*)

		std::vector<DibujablePtr> objs;	//!< List of deleteable objects.
		DibujablePtr selected;	//!< Selected object. NULL if none.

		DECLARE_EVENT_TABLE()
};
}	//End of namespace
#endif
