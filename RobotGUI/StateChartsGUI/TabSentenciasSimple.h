#ifndef TABSENTENCIASSIMPLE_H
#define TABSENTENCIASSIMPLE_H

//(*Headers(TabSentenciasSimple)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/button.h>
//*)

#include "Estado.h"
#include "Accion.h"
#include <vector>

using namespace std;
using namespace InterfazSancho;
/**
  * This panel contains the necessary controls to configure an arbitrary set of instructions. This class has a decent amount of Codeblocks-generated code.
  * \sa TabSentencias, Estado
  */
class TabSentenciasSimple: public wxPanel
{
	public:

		TabSentenciasSimple(const EstadoPtr &e,const wxString &labelText,wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);	//!< Constructor
		virtual ~TabSentenciasSimple()	{}	//!< Destructor.

		//(*Declarations(TabSentenciasSimple)
		wxButton* arriba;	//!< Button to move an instruction one position up.
		wxListBox* sentencias;	//!< List of current sentences, appearing in their most readable format.
		wxButton* anadirControl;	//!< Button to add a SCXML action.
		wxButton* quitar;	//!< Button to remove the selected instruction.
		wxButton* limpiar;	//!< Button to clean the instruction list. This is currently not undoable.
		wxButton* anadirRobot;	//!< Button to add a Robot instruction.
		wxButton* editar;	//!< Button to edit the selected instruction.
		wxButton* abajo;	//!< Button to move an instruction one position down.
		//*)

		void anadirSentencia(const AccionPtr &);	//!< Adds an action to the list.
		void getSentencias(vector<AccionPtr> &) const;	//!< Gets the full list of actions.
		void setSentencias(const vector<AccionPtr> &);	//!< Sets the full list of actions.

	protected:

		//(*Identifiers(TabSentenciasSimple)
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_LISTBOX1;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_BUTTON7;
		//*)

	private:

		//(*Handlers(TabSentenciasSimple)
		void OnanadirRobotClick(wxCommandEvent& event);
		void OnanadirControlClick(wxCommandEvent& event);
		void OnquitarClick(wxCommandEvent& event);
		void OnlimpiarClick(wxCommandEvent& event);
		void OnarribaClick(wxCommandEvent& event);
		void OneditarClick(wxCommandEvent& event);
		void OnabajoClick(wxCommandEvent& event);
		void OnsentenciasSelect(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
		int indiceEnEspera;	//!< Lastly selected instruction (used with edit instruction). This is in fact a somewhat deprecated variable that could be refactored into disappearing.
		EstadoPtr estado;	//!< State related to this instruction list. It's used to automatically assign labels to the robot instructions.
		vector<AccionPtr> instrs;	//!< Current list of instructions.

		void setSentencias();	//!< Updates the GUI with the current list of actions.
		void updateButtons();	//!< Enables or disables buttons, based on the current selection.
};

#endif
