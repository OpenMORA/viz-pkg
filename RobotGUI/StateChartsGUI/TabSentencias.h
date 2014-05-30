#ifndef TABSENTENCIAS_H
#define TABSENTENCIAS_H

//(*Headers(TabSentencias)
#include <wx/panel.h>
//*)

#include "Accion.h"
#include "Estado.h"
#include "TabSentenciasSimple.h"

/**
  * This class contains a pair of TabSentenciasSimple panels, and is used to configure both the onentry and onexit actions of a state.
  * Despite its name, TabSentenciasSimple contains much more code than TabSentencias, since is there where all the processing is actually performed.
  * This class's skeleton is partly done with Codeblocks.
  * \sa TabSentenciasSimple, Estado, Accion
  */
class TabSentencias: public wxPanel
{
	public:
		TabSentencias(EstadoPtr &e,wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);	//!< Constructor
		virtual ~TabSentencias()	{}	//!< Virtual destructor

		//(*Declarations(TabSentencias)
		TabSentenciasSimple *onentry;	//!< Leftmost instructions panel.
		TabSentenciasSimple *onexit;	//!< Rightmost instructions panel.
		//*)

		inline void getOnentry(vector<AccionPtr> &sen) const	{	//!< Gets the first set of actions.
			onentry->getSentencias(sen);
		}
		inline void getOnexit(vector<AccionPtr> &sen) const	{	//!< Gets the second set of actions.
			onexit->getSentencias(sen);
		}

		inline void setOnentry(const vector<AccionPtr> &sen)	{	//!< Sets the first set of actions.
			onentry->setSentencias(sen);
		}
		inline void setOnexit(const vector<AccionPtr> &sen)	{	//!< Sets the second set of actions.
			onexit->setSentencias(sen);
		}

	protected:

		//(*Identifiers(TabSentencias)
		static const long ID_ONENTRY;
		static const long ID_ONEXIT;
		//*)

	private:

		//(*Handlers(TabSentencias)
		//*)

		DECLARE_EVENT_TABLE()
		EstadoPtr &estado;	//!< State relating to this panel. It's used to automatically label the instructions.
};

#endif
