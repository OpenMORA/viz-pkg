#ifndef PAINTBOARDACTION_H
#define PAINTBOARDACTION_H
#include <wx/wx.h>
#include "PaintBoardEvent.h"

class PaintBoardAction
{
    /// The PaintBoardAction class represents an action of a state in the State Chart
    public:
        // Constructor
        PaintBoardAction();

        // Getter and Setter methods
        wxString GetID() { return ID; }                                 // Get the ID of the action
        void SetID(wxString val);                                       // Set the ID of the action
        wxString GetType() { return Type; }                             // Get the action type (command)
        void SetType(wxString val);                                     // Set the action type (command)
        wxString GetParameters() { return Parameters; }                 // Get the parameters of the action
        void SetParameters(wxString val) { Parameters = val; }          // Set the parameters of the action
        int GetRepetitions() { return repetitions; }                    // Get the number of repetitions of the action
        void SetRepetitions(int val) { repetitions = val; }             // Set the number of repetitions of the action
        PaintBoardEvent* GetDoneEvent(){return doneEvent;}              // Get the event launched when the action is finished
        void SetDoneEvent(PaintBoardEvent* val) { doneEvent = val; }    // Set the event launched when the action is finished

    private:
        wxString ID;                    // ID of the action
        wxString Type;                  // Type of action (command)
        wxString Parameters;            // Parameters
        PaintBoardEvent *doneEvent;     // Event on end of the action
        int repetitions;                // Number of repetitions
};

#endif // PAINTBOARDACTION_H
