#ifndef PAINTBOARDEVENT_H
#define PAINTBOARDEVENT_H
#include <wx/wx.h>

class PaintBoardEvent
{
    /// The PaintBoardEvent class represents an event in the State Chart
    public:
        // Constructor
        PaintBoardEvent();

        // Getter and Setter methods
        wxString* GetEvent() { return event; }                      // Return the event represented by the object
        void SetEvent(wxString val) { *event = val; }               // Set the event
        wxString* GetCondition() { return condition; }              // Return the condition
        void SetCondition(wxString val) { *condition = val; }       // Set the condition
        wxString* GetVariable() { return variable; }                      // Return the event represented by the object
        void SetVariable(wxString val) { *variable = val; }               // Set the event
        wxString* GetConditionType() { return conditionType; }              // Return the condition
        void SetConditionType(wxString val) { *conditionType = val; }       // Set the condition
    private:
        wxString *event;            // String representing the event
        wxString *condition;        // String representing the condition
        wxString *variable;         // String representing the variable of the condition
        wxString *conditionType;    // String representing the type of condition (by the moment only random is accepted)
};
#endif // PAINTBOARDEVENT_H
