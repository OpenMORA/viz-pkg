/// element_types: indicates the type of element that a PaintBoardElement is. \todo Use it everywhere or eliminate it and test a dynamic_cast to determine type
/** Paint Board Element Configurator. This class provides a dialog that configures the elements of the Paint Board */

#ifndef PAINTBOARDELEMENTCONFIGURATOR_H
#define PAINTBOARDELEMENTCONFIGURATOR_H
#include "PaintBoardElement.h"
#include "PaintBoardAction.h"
//#include "PaintBoardLink.h"
#include <wx/notebook.h>
#include <vector>
#define BTN_ADD 10
#define BTN_DELETE 11
#define BTN_SAVE 12
#define LISTBOX_ACTIONS 13


enum element_types {LINKT, NODET};

/// Constants for the Configurator dialog
#define BTN_ADD 10              // Add button
#define BTN_DELETE 11           // Delete button
#define BTN_SAVE 12             // Save button
#define LISTBOX_ACTIONS 13      // List of actions

class PaintBoardElementConfigurator : public wxDialog
{
    /// The PaintBoardElementConfigurator class implements a configuration dialog for the elements of the Paint Board. The class behaves differently according to the type of the element.
    public:
        // Constructor
        PaintBoardElementConfigurator(const wxString& title, element_types typeConfig, PaintBoardElement* element); // title: title of the window. typeConfig: type of the configured element. element: configured element

        // GUI building methods
        void buildNodeConfigurator();       // create a Node properties configurator dialog
        void buildLinkConfigurator();       // create a Link properties configurator dialog

        // Variable fetching functions
        void getAvailableActions();         // get available actions from the Paint Board (only for node objects)
        void getAvailableEvents();          // get available events from the initial state (only for link objects)

        // Property reading and setting options
        void updateInfo();                                              // update the dialog contents according to the information of the element
        void setPropertiesAndExit(wxCommandEvent& WXUNUSED(event));     // set properties on the element and close the dialog

        // Functions managing the action list
        void AddAction(wxCommandEvent& WXUNUSED(event));                // add an action to the action list (only for Node configurators)
        void DeleteAction(wxCommandEvent& WXUNUSED(event));             // delete an action from the action list (only for Node configurators)
        void OpenAction(wxCommandEvent& WXUNUSED(event));               // open action from list and show the values of the properties (only for Node configurators)
        void SaveAction(wxCommandEvent& WXUNUSED(event));

    private:
        PaintBoardElement* element;                                     // parent element
        element_types element_type;                                     // type of parent element
        wxArrayString availableActions, availableEvents;                // list of actions and events (as string)
        std::vector<PaintBoardAction*> actionList;                      // list of available actions
        std::vector<PaintBoardEvent*> eventList;                        // list of available events

        // Graphical elements
        wxNotebook *mainTabs;
        wxPanel *panel, *panelGral, *panelFree, *panelActions;
        wxBoxSizer *mainSizer, *gralSizer, *freeSizer, *actionsSizer;
        wxTextCtrl *nameEntry, *IDEntry, *parametersEntry, *repetitionsEntry, *conditionEntry, *variableEntry;
        wxComboBox *actionEntry, *eventEntry, *typeEntry;
        wxCheckBox *finalCheck, *initialCheck;
        wxButton *buttonOK, *buttonCancel, *buttonAdd, *buttonDelete, *buttonSave;
        wxListBox *actionListEntry;
        wxStaticText *nodesLabel;

        /*

         conditionSizer->Add(variableEntry,1,wxEXPAND);
        conditionSizer->Add(conditionLabel);
        conditionSizer->Add(conditionEntry,1,wxEXPAND);
        conditionSizer->Add(typeLabel);
        conditionSizer->Add(typeEntry,1,wxEXPAND);
        */
};
#endif // PAINTBOARDELEMENTCONFIGURATOR_H

