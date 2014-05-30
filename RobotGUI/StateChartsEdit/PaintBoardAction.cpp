#include "PaintBoardAction.h"

PaintBoardAction::PaintBoardAction()
{
    // Constructor
    // initialize variables
    doneEvent = new PaintBoardEvent();
}

void PaintBoardAction::SetType(wxString val)
{
    // Set the action type (command)
    Type = val;

    // create the "done" event
   // wxString* eventStr = new wxString();
   // *eventStr = val + _(".done");
   // doneEvent->SetEvent(*eventStr);
}

void PaintBoardAction::SetID(wxString val) {
    ID = val;
    // create the "done" event
    wxString* eventStr = new wxString();
    *eventStr = val + _(".done");
    doneEvent->SetEvent(*eventStr);
	delete eventStr;
    }

