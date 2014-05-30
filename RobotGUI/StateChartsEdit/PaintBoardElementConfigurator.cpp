#include "PaintBoardElementConfigurator.h"
#include "PaintBoardNode.h"
#include "PaintBoardLink.h"
#include "PaintBoard.h"
PaintBoardElementConfigurator::PaintBoardElementConfigurator(const wxString& title, element_types typeConfig, PaintBoardElement* element)
: wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 300))
{
    /** Constructor
     * Creates the interface and starts all values to those given by the element*/

    this->element = element;                    // Parent PaintBoardElement
    this->element_type = typeConfig;            /// Parent element type \todo could be done automatically, not externally


    //    getAvailableEvents();                       /// Get available events from the widget: by now not implemented \todo implement. Also refresh list every time a new event is added

    panel = new wxPanel(this,-1);               // Start the main panel of the dialog

    // create OK and Cancel buttons
    buttonOK = new wxButton(panel,wxID_OK,_("OK"));
    buttonCancel = new wxButton(panel,wxID_CANCEL,_("Cancel"));

    // Create main sizer (used in the main panel)
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create bottom sizer, and add the OK and Cancel Buttons
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomSizer->Add(buttonOK,1);
    bottomSizer->Add(buttonCancel,1);

    // Depending on the object to configure, the dialog will have a specific set of fields
    if (typeConfig == NODET)
    {
        // in case it is a node
        getAvailableActions();                      /// Get available actions from the widget: by now not implemented \todo implement

        buildNodeConfigurator();
    } else {
        // in case it is a link
        buildLinkConfigurator();
    }

    // Finish the interface

    // add the bottom elements (ok/cancel buttons)
    mainSizer->Add(bottomSizer,0,wxBOTTOM);

    // set the main sizer
    panel->SetSizer(mainSizer);
//    panel->Show();
    wxBoxSizer* winSizer;
    winSizer = new wxBoxSizer(wxVERTICAL);
    winSizer->Add(panel,0,wxEXPAND);
    SetSizer(winSizer);

    // Connect the OK button to the proper handler function
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PaintBoardElementConfigurator::setPropertiesAndExit));

}


void PaintBoardElementConfigurator::buildNodeConfigurator()
{
    // Interface creator for a node configurator

    // get pointer to parent node
    PaintBoardNode* node;
    node = dynamic_cast<PaintBoardNode*>(element);

    if (node){
        // won't enter here if the element is not a node (should not fail)

        // Create tabbed container
        mainTabs = new wxNotebook(panel,-1);

        // Create tabbed panels
        panelGral = new wxPanel(mainTabs,-1);
        panelFree = new wxPanel(mainTabs,-1);
        wxPanel *panelGralL = new wxPanel(panelGral,-1);        // General Left
        wxPanel *panelGralR = new wxPanel(panelGral,-1);        // General Right



        nameEntry = new wxTextCtrl(panel,-1);           // Name entry control
        IDEntry = new wxTextCtrl(panelGralL,-1);         // Action ID entry control

        nameEntry->SetValue(node->getName());           // Set the name on the name entry control

        /*
        wxString *ID, *actionP, *actionT;
        int rep;

        ID = new wxString();
        actionT = new wxString();
        actionP = new wxString();*/

      //  node->getActions(*actionT,*ID,*actionP,rep);

        // Set the ID of the action
        //IDEntry->SetValue(*ID);

        parametersEntry = new wxTextCtrl(panelGralL,-1);     // parameters entry control

        //parametersEntry->SetValue(*actionP);            // set parameters in control

        repetitionsEntry = new wxTextCtrl(panelGralL,-1);        // number of repetitions entry
        //repetitionsEntry->SetValue(wxString::Format(_("%i"),rep));      // set number of repetitions

        // get the index of the action in the available actions list
        //int actionTID;
        //actionTID = availableActions.Index(*actionT);

        actionEntry = new wxComboBox(panelGralL,-1,_("Choose action"),wxDefaultPosition,wxDefaultSize,availableActions);    //Entry for action type
        //if (actionTID != wxNOT_FOUND){
            // if action exists, choose it (otherwise display "Choose Action" which is written by default
            //actionEntry->SetSelection(actionTID);
        //}

        finalCheck = new wxCheckBox(panel,-1,_("Final"));               // final action property (true if there are no actions afterwards) checkbox
        initialCheck = new wxCheckBox(panel,-1,_("Initial"));           // initial action property
        /// \todo: MARK IF FINAL (NOT CHECKED CURRENTLY)
        // Create boxSizers for the "general" tab
        wxBoxSizer *gralActionID = new wxBoxSizer(wxHORIZONTAL);        // action ID
        wxBoxSizer *gralAction = new wxBoxSizer(wxHORIZONTAL);          // action type
        wxBoxSizer *gralParameters = new wxBoxSizer(wxHORIZONTAL);      // parameters
        wxBoxSizer *gralRepetitions = new wxBoxSizer(wxHORIZONTAL);     // repetitions

        // Create the labels for the "general" tab
        wxStaticText *labelActionID = new wxStaticText(panelGralL,-1,_("Action ID"));
        wxStaticText *labelAction = new wxStaticText(panelGralL,-1,_("Action"));
        wxStaticText *labelParameters = new wxStaticText(panelGralL,-1,_("Action Parameters"));
        wxStaticText *labelRepetitions = new wxStaticText(panelGralL,-1,_("Loop repetitions"));


        // Add elements by pair (label + action) to their sizer
        gralActionID->Add(labelActionID,1);
        gralActionID->Add(IDEntry,0,wxEXPAND);

        gralAction->Add(labelAction,1);
        gralAction->Add(actionEntry,0,wxEXPAND);

        gralParameters->Add(labelParameters,1);
        gralParameters->Add(parametersEntry,0,wxEXPAND);

        gralRepetitions->Add(labelRepetitions,1);
        gralRepetitions->Add(repetitionsEntry,0,wxEXPAND);

        // create a sizer for the "general" tab (left part)
        wxBoxSizer *gralSizerL = new wxBoxSizer(wxVERTICAL);

        // add all elements to the sizer
        gralSizerL->Add(gralActionID,0);
        gralSizerL->Add(gralAction,0);
        gralSizerL->Add(gralParameters,0);
        gralSizerL->Add(gralRepetitions,0);

        // and set it as main sizer of the "general left" tab
        panelGralL->SetSizer(gralSizerL);

        // Right panel elements of the "general" tab
        actionListEntry = new wxListBox(panelGralR,LISTBOX_ACTIONS);        // List of actions in the state
        buttonAdd = new wxButton(panelGralR,BTN_ADD,_("Add"));              // Add new state button
        buttonDelete = new wxButton(panelGralR,BTN_DELETE,_("Delete"));     // Delete state button
        buttonSave = new wxButton(panelGralR,BTN_SAVE,_("Save"));           // Save changes button

        // Fill the action list with the available actions(if any). NOTE: In the current state of the widget, the action list will always be empty at this point, because there is no way of opening an existing state chart
        actionList = node->getActions();
        std::vector<PaintBoardAction*>::iterator actionIter = actionList.begin();       // iterate over the actions
        while(actionIter != actionList.end()){
            actionListEntry->Append((*actionIter)->GetType());
            actionIter++;
        }

        // Connect the buttons to their action handlers
        Connect(LISTBOX_ACTIONS, wxEVT_COMMAND_LISTBOX_SELECTED,wxCommandEventHandler(PaintBoardElementConfigurator::OpenAction));
        Connect(BTN_ADD, wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(PaintBoardElementConfigurator::AddAction));
        Connect(BTN_DELETE, wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(PaintBoardElementConfigurator::DeleteAction));
        Connect(BTN_SAVE, wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(PaintBoardElementConfigurator::SaveAction));

        // Add buttons to button row
        wxBoxSizer *buttonRowR = new wxBoxSizer(wxHORIZONTAL);
        buttonRowR->Add(buttonAdd,1);
        buttonRowR->Add(buttonDelete,1);
        buttonRowR->Add(buttonSave,1);

        // Add elements to the right panel
        wxBoxSizer *gralSizerR = new wxBoxSizer(wxVERTICAL);
        gralSizerR->Add(actionListEntry,1,wxEXPAND);
        gralSizerR->Add(buttonRowR,0);
        panelGralR->SetSizer(gralSizerR);   // set the sizer for the right panel

        // Add the right and left to the general tab
        gralSizer = new wxBoxSizer(wxHORIZONTAL);
        gralSizer->Add(panelGralL,1,wxEXPAND);
        gralSizer->Add(panelGralR,1,wxEXPAND);
        panelGral->SetSizer(gralSizer);


        /// \todo Implement the "free definition" panel
        wxStaticText *notimplemented = new wxStaticText(panelFree,-1,_("Not implemented"));
        freeSizer = new wxBoxSizer(wxHORIZONTAL);
        freeSizer->Add(notimplemented);
        panelFree->SetSizer(freeSizer);

        // add the pages (panels) to the tabbed container (notebook)
        mainTabs->AddPage(panelGral,_("General"));
        mainTabs->AddPage(panelFree,_("Free definition"));

        // Create a boxsizer for the name entry and the "final" checkbox
        wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);

        // Name entry label
        wxStaticText *labelName = new wxStaticText(panel,-1,_("Name"));

        // add elements to the name entry sizer
        nameSizer->Add(labelName,0);
        nameSizer->Add(nameEntry,0);
        nameSizer->Add(finalCheck,0);
        nameSizer->Add(initialCheck,0);

        // add the elements to the main sizer
        mainSizer->Add(nameSizer,0);
        mainSizer->Add(mainTabs,1,wxEXPAND);
    }
}

void PaintBoardElementConfigurator::buildLinkConfigurator()
{
    // Interface creator for a link configurator

    this->SetSize(700,200);

    // get pointer to parent link
    PaintBoardLink* link;
    link = dynamic_cast<PaintBoardLink*>(element);

    if (link){
        // Create tabbed container
        mainTabs = new wxNotebook(panel,-1);

        // Create tabbed panels
        panelGral = new wxPanel(mainTabs,-1);
        panelActions = new wxPanel(mainTabs,-1);

        // create event entry combo box and select the current event
        eventEntry = new wxComboBox(panelGral,-1,_("Choose event"),wxDefaultPosition,wxDefaultSize,availableEvents);

        conditionEntry = new wxTextCtrl(panelGral,-1);     // condition entry control
//        conditionEntry->SetValue(*cond);            // set condition in control
        variableEntry = new wxTextCtrl(panelGral,-1);
        typeEntry = new wxComboBox(panelGral,-1);
        typeEntry->Append(_("RANDOM"));

        // Label indicating origin and destination states
        wxString *nodesLabelTxt;
      /*  char nodesLabelTxtBuf[1024];
        strcpy(nodesLabelTxtBuf,link->startNode->getName().mb_str());
        strcat(nodesLabelTxtBuf, " -> ");
        strcat(nodesLabelTxtBuf,link->endNode->getName().mb_str());
        //nodesLabelTxt = new wxString(_U(nodesLabelTxtBuf));
        wxString nodesLabelTxt(nodesLabelTxtBuf, wxConvUTF8);*/
        nodesLabelTxt = new wxString(_("Origin -> Destination"));

      //  *nodesLabelTxt = link->startNode->getName() + _(" -> ") + link->endNode->getName();
      /// \todo Separate "create interface" and "fill/update interface" in separate functions to be able to use the previous line
        // Labels in the link properties dialog
        nodesLabel = new wxStaticText(panelGral,-1,*nodesLabelTxt);
        wxStaticText *eventLabel = new wxStaticText(panelGral,-1,_("Event"));
        wxStaticText *conditionLabel = new wxStaticText(panelGral,-1,_("Condition"));
        wxStaticText *variableLabel = new wxStaticText(panelGral,-1,_("Variable"));
        wxStaticText *conditionTypeLabel = new wxStaticText(panelGral,-1,_("Type"));

        // Pair the labels with the text entries
        wxBoxSizer *eventSizer = new wxBoxSizer(wxHORIZONTAL);
        eventSizer->Add(eventLabel);
        eventSizer->Add(eventEntry,1,wxEXPAND);

        wxBoxSizer *conditionSizer = new wxBoxSizer(wxHORIZONTAL);
        conditionSizer->Add(variableLabel,0);
        conditionSizer->Add(variableEntry,0);
        conditionSizer->Add(conditionLabel,0);
        conditionSizer->Add(conditionEntry,0);
        conditionSizer->Add(conditionTypeLabel,0);
        conditionSizer->Add(typeEntry,0);

        // General panel sizer
        gralSizer = new wxBoxSizer(wxVERTICAL);
        gralSizer->Add(nodesLabel,2);
        gralSizer->Add(eventSizer,1);
        gralSizer->Add(conditionSizer,1);

        // Set the sizer on the general panel
        panelGral->SetSizer(gralSizer);

        // Not implemented: actions tab
        wxStaticText *notimplemented = new wxStaticText(panelActions,-1,_("Not implemented"));
        actionsSizer = new wxBoxSizer(wxHORIZONTAL);
        actionsSizer->Add(notimplemented);
        panelActions->SetSizer(actionsSizer);

        // add the pages (panels) to the tabbed container (notebook)
        mainTabs->AddPage(panelGral,_("General configuration"));
        mainTabs->AddPage(panelActions,_("Actions to take"));

        // Add the tab container to the main sizer
        mainSizer->Add(mainTabs,1,wxEXPAND);
    }
}

void PaintBoardElementConfigurator::updateInfo()
{
    /// Update the information of the dialog with the information contained in the parent element
    /// \todo Implement for a link object

    if (element_type == NODET)
    {
        getAvailableActions();                      /// Get available actions from the widget: by now not implemented \todo implement
		
        // Get a pointer to the parent object
        PaintBoardNode *node;
        node = static_cast<PaintBoardNode*>(element);

        // get properties
        nameEntry->SetValue(node->getName());

        if (node->isFinal()){
            finalCheck->SetValue(true);
        } else {
            finalCheck->SetValue(false);
        }

        if (node->isInitial()){
            initialCheck->SetValue(true);
        } else {
            initialCheck->SetValue(false);
        }

        // get the actions from the node and fill the actions list
        actionList = node->getActions();
        std::vector<PaintBoardAction*>::iterator actionIter = actionList.begin();
        actionListEntry->Clear();
        while(actionIter != actionList.end()){
            actionListEntry->Append((*actionIter)->GetType());
            actionIter++;
        }
		
        // if the list is not empty, open the first one
        if (!actionList.empty()){
            actionListEntry->SetSelection(0);
            OpenAction(*(new wxCommandEvent())); // call Openaction with a dummy event (not processed by OpenAction
        }
		
    } else {
        // get The information of the action of the node
        getAvailableEvents();
        std::vector<PaintBoardEvent*>::iterator evtIter = eventList.begin();
        eventEntry->Clear();

        while(evtIter != eventList.end()){
            eventEntry->Append(*((*evtIter)->GetEvent()));
            evtIter++;
        }


         PaintBoardLink* link;
        link = dynamic_cast<PaintBoardLink*>(element);
        wxString *evtname, *cond, *varname, *condtype;
        PaintBoardEvent* evt;

        evt = new PaintBoardEvent();
        //cond = new wxString();
        evt = link->getEvent();
        evtname = evt->GetEvent();
        cond = evt->GetCondition();
        condtype = evt->GetConditionType();
        varname = evt->GetVariable();

        // get the index of the event in the available events list.
        int eventTID;

        eventTID = availableEvents.Index(*evtname);

        // create event entry combo box and select the current event
       // eventEntry = new wxComboBox(panelGral,-1,_("Choose event"),wxDefaultPosition,wxDefaultSize,availableEvents);
        if (eventTID != wxNOT_FOUND){
            // if action exists, choose it (otherwise display "Choose Action" which is written by default
            eventEntry->SetSelection(eventTID);
            //wxString err;
            //err.Format(_("Num %f"),eventTID);
            //wxString mystring;
            //mystring << eventTID;
            //wxMessageBox(mystring);
        } else {
            eventEntry->SetValue(*evtname);
        }

        int condTID;

        condTID = availableEvents.Index(*condtype);

        // create condition type entry combo box and select the current event

        if (condTID != wxNOT_FOUND){
            typeEntry->SetSelection(condTID);
        } else {
            typeEntry->SetValue(*condtype);
        }




       // conditionEntry = new wxTextCtrl(panelGral,-1);     // condition entry control
        conditionEntry->SetValue(*cond);            // set condition in control

        variableEntry->SetValue(*varname);

        // Label indicating origin and destination states
        wxString *nodesLabelTxt;      //
        nodesLabelTxt = new wxString();
        *nodesLabelTxt = link->startNode->getName() + _(" ---> ") + link->endNode->getName();

        // Labels in the link properties dialog
        nodesLabel->SetLabel(*nodesLabelTxt); // = new wxStaticText(panelGral,-1,*nodesLabelTxt);
		delete nodesLabelTxt;
    }
	
}

void PaintBoardElementConfigurator::setPropertiesAndExit(wxCommandEvent& WXUNUSED(event))
{
    /// This function calls the configuration methods of the element and then hides the dialog
    if (element_type == NODET){
        // if the element is a node
        // take a pointer to the node
        PaintBoardNode* node;
        node = dynamic_cast<PaintBoardNode*>(element);
        if(node){
            // Set properties
            node->setName(nameEntry->GetValue());
            node->setFinal(finalCheck->GetValue());
            node->setInitial(initialCheck->GetValue());
            node->setActionList(actionList);
            node->repaint();
        }
    } else {
        // if the element is a link
        PaintBoardLink* link;
        link = dynamic_cast<PaintBoardLink*>(element);
        if(link){
            // set properties and engage
            PaintBoardEvent *newEvent = new PaintBoardEvent();
            newEvent->SetEvent(eventEntry->GetValue());
			if (conditionEntry->GetValue() != _("")){
				newEvent->SetCondition(conditionEntry->GetValue());
				newEvent->SetConditionType(typeEntry->GetValue());
				newEvent->SetVariable(variableEntry->GetValue());
				link->addVariable(variableEntry->GetValue());
			}

            link->setEvent(newEvent);
            link->engage();
            link->repaint();

        }
    }
    // hide the dialog
    this->Hide();
}

void PaintBoardElementConfigurator::AddAction(wxCommandEvent& WXUNUSED(event))
{
    /// Add an action from the properties inserted in the dialog to the action list

    // Create new action
    PaintBoardAction *newAction = new PaintBoardAction();

    // Set the properties
    newAction->SetID(IDEntry->GetValue());
    newAction->SetType(availableActions[actionEntry->GetCurrentSelection()]);
    newAction->SetParameters(parametersEntry->GetValue());
    newAction->SetRepetitions(wxAtoi(repetitionsEntry->GetValue()));

    // Save the action to the action list
    actionList.push_back(newAction);

    // Add an entry in the actions list widget
	wxString entryStr = availableActions[actionEntry->GetCurrentSelection()] + _(" ") + parametersEntry->GetValue() + _(" (") + IDEntry->GetValue() + _(")");
    actionListEntry->Append(entryStr);

    // Clear all the fields
    IDEntry->Clear();
    actionEntry->SetSelection(0);
    parametersEntry->Clear();
    repetitionsEntry->Clear();
}

void PaintBoardElementConfigurator::DeleteAction(wxCommandEvent& WXUNUSED(event))
{
    /// Delete an action selected in the action list

    // Delete the selected action from action list
    actionList.erase(actionList.begin() + actionListEntry->GetSelection());

    // Delete the selected action from the action list entry widget
    int selection = actionListEntry->GetSelection();
    actionListEntry->Delete(selection);
    actionListEntry->SetSelection(wxNOT_FOUND);
}

void PaintBoardElementConfigurator::OpenAction(wxCommandEvent& WXUNUSED(event))
{
    /// Open an action and set the values of the fields to the settings of the action.
    PaintBoardAction *selectedAction;

    // get the entry from the action list widget
    if (actionListEntry->GetSelection() != wxNOT_FOUND)
    {
        // get the pointer to the action
        selectedAction = actionList[actionListEntry->GetSelection()];

        // set the values
        IDEntry->SetValue(selectedAction->GetID());
        parametersEntry->SetValue(selectedAction->GetParameters());
        repetitionsEntry->SetValue(wxString::Format(_("%i"),selectedAction->GetRepetitions()));
        int actionTID = availableActions.Index(selectedAction->GetType());
        if (actionTID != wxNOT_FOUND){
            // if action exists, choose it (otherwise display "Choose Action" which is written by default
            actionEntry->SetSelection(actionTID);
        }
    }
}

void PaintBoardElementConfigurator::SaveAction(wxCommandEvent& WXUNUSED(event))
{
        /// Open an action and set the values of the fields to the settings of the action.
    PaintBoardAction *selectedAction;

    // get the entry from the action list widget
    if (actionListEntry->GetSelection() != wxNOT_FOUND)
    {
        // get the pointer to the action
        selectedAction = actionList[actionListEntry->GetSelection()];

        // set the values
        selectedAction->SetID(IDEntry->GetValue());
        //IDEntry->SetValue(selectedAction->GetID());
        selectedAction->SetParameters(parametersEntry->GetValue());
        //parametersEntry->SetValue(selectedAction->GetParameters());
        selectedAction->SetRepetitions(wxAtoi(repetitionsEntry->GetValue()));
        //repetitionsEntry->SetValue(wxString::Format(_("%i"),selectedAction->GetRepetitions()));
        selectedAction->SetType(availableActions[actionEntry->GetCurrentSelection()]);

    }
}

void PaintBoardElementConfigurator::getAvailableActions()
{
    /** \todo On integration with OpenMORA implement this function */
 //   availableActions.push_back(*());
  //  availableActions.push_back(*(new wxString(_("ACTION2"))));
  PaintBoardNode *node;
  node = dynamic_cast<PaintBoardNode*>(element);

  PaintBoard *board;
  board = dynamic_cast<PaintBoard*>(node->board);

  std::vector<wxString> actions = board->actions;
  std::vector<wxString>::iterator actioniter = actions.begin();

  while (actioniter != actions.end()){
    availableActions.Add(*actioniter);
    actioniter ++;
  }

   // availableActions.Add(*(new wxString(_("ACTION1"))));
   // availableActions.Add(*(new wxString(_("ACTION2"))));
    // TEMPORARILY DISABLED availableActions = element->board->actions;
}

void PaintBoardElementConfigurator::getAvailableEvents()
{
    /** \todo On integration with OpenMORA implement this function */
 //   availableActions.push_back(*());
  //  availableActions.push_back(*(new wxString(_("ACTION2"))));
  //  availableEvents.Add(*(new wxString(_("EVENT1"))));
  //  availableEvents.Add(*(new wxString(_("EVENT2"))));

    // Get start node pointer
    PaintBoardLink *link;
    PaintBoardNode *node;
    link = dynamic_cast<PaintBoardLink*>(element);
    if (link) {
        node = static_cast<PaintBoardNode*>(link->startNode);
        // now get actions of the node (reusing actionList variable that is not used in the link configurator in the same way as in the node configurator):
        actionList = node->getActions();

        //iterate over the actions and generate the events
        std::vector<PaintBoardAction*>::iterator actionIter = actionList.begin();
        while (actionIter != actionList.end()){
            eventList.push_back((*actionIter)->GetDoneEvent());
            availableEvents.Add(*((*actionIter)->GetDoneEvent()->GetEvent()));
            actionIter++;
        }
    }




    // TEMPORARILY DISABLED availableActions = element->board->actions;
}

