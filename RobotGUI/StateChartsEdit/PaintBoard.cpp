
#include "PaintBoard.h"
#include <algorithm>
#include <map>

// Definition of the event table. Not all events are used, but they are kept for possible future functions
BEGIN_EVENT_TABLE(PaintBoard, wxPanel)


 EVT_MOTION(PaintBoard::mouseMoved)                 // The mouse moves on the surface of the widget
 EVT_LEFT_DOWN(PaintBoard::mouseDown)               // Click with the left button of the mouse
 EVT_LEFT_UP(PaintBoard::mouseReleased)             // Left button of the mouse released
 EVT_RIGHT_DOWN(PaintBoard::rightClick)             // Click with the right button of the mouse
 EVT_LEAVE_WINDOW(PaintBoard::mouseLeftWindow)      // Mouse exits the window
 EVT_KEY_DOWN(PaintBoard::keyPressed)               // Key pressed
 EVT_KEY_UP(PaintBoard::keyReleased)                // Key released
 EVT_MOUSEWHEEL(PaintBoard::mouseWheelMoved)        // Mouse wheel moved

 //EVT_PAINT(PaintBoard::paintEvent)                  // The widget got repainted

END_EVENT_TABLE()




/*
PaintBoard::PaintBoard(wxPanel* parent, std::vector<wxString> actions) :
wxPanel(parent)
{
    // Constructor of the board
    this->actions = actions;
    currentState = NONE;    // Start in a neutral state
}
*/
PaintBoard::~PaintBoard()
{
	//while(!bar.empty()) delete bar.back(), bar.pop_back();

	
    // Destructor
     std::vector<PaintBoardElement*>::iterator elementIter = presentElements.begin();
  //  elementIter = find(presentElements.begin(),presentElements.end(),clickedElement);
    while (elementIter != presentElements.end()){

            (*elementIter)->close();
           // presentElements.erase(elementIter);
            //delete *elementIter;


        elementIter++;
    }





	while(!presentElements.empty())
	{

		delete presentElements.back();
		presentElements.pop_back();
	}
	
	delete fileName;


	
	/*
    // Destructor
     std::vector<PaintBoardElement*>::iterator elementIter = presentElements.begin();
  //  elementIter = find(presentElements.begin(),presentElements.end(),clickedElement);
    while (elementIter != presentElements.end()){

            (*elementIter)->close();
            presentElements.erase(elementIter);
            //delete *elementIter;


        elementIter++;
    }*/
	
}

PaintBoard::PaintBoard(wxPanel* _parent) :
wxPanel(_parent)
{
    // Constructor of the board
    currentState = NONE;    // Start in a neutral state
    saved = false;
    parent = _parent;
    fileName = new wxString();
	linkRefreshCount = 0;
}

void PaintBoard::mouseMoved(wxMouseEvent& event)
{
    // Mouse moved event handler

    if (currentState==LINKSTOP){
        // If a link is being painted at the moment, its end is updated to be right under the mouse pointer
        updateActualLink(event);
		linkRefreshCount ++;
		if (linkRefreshCount == 10){
			repaint();
			linkRefreshCount = 0;
		}
    }
}

void PaintBoard::setMState(machine_states _state){
    // set state machine to _state
    currentState = _state;

    // special actions for some states
    if (_state == LINK){
        // Show the docks of the nodes
        std::vector<PaintBoardElement*>::iterator elements = this->presentElements.begin();

        PaintBoardNode *node;

        // explore the list of objects
        while(elements != this->presentElements.end()){
            node = dynamic_cast<PaintBoardNode*>(*elements);
            if (node){ // if element is a node
                // show the docks
                node->showDocks();
            }
            elements ++;
        }
    }

}



void PaintBoard::mouseDown(wxMouseEvent& event)
{
    // Left button clicked event handler
    long *x,*y;
    x = new long(0.0);
    y = new long(0.0);
    event.GetPosition(x,y);
    wxPoint *position;
    position = new wxPoint(*x,*y);

    // Test the state of the widget to take the appropriate action
    switch (currentState){
        case STATE:     // An external order was given to the widget to paint states
			if(findClickedObject(clickedElement,*position)){
				launchCtxMenu();
			} else {
				drawAState(event);  // Therefore paint a state
			}
            break;
        case LINK:      // An external order was given to the widget to paint a transition
            drawALinkStart(event);  // Therefore start painting the link on a nearby dock of a state
            break;
        case LINKSTOP:  // The mouse was clicked on the desired destination of the transition
			drawALinkStop(event);   // Therefore stop painting the link and attach to a nearby dock
            break;
        case MOVE:
            moveNodeStop(static_cast<PaintBoardNode*>(clickedElement),*position);
            break;
        case NONE:
            // If clicked on (or near enough to) an element painted on the board, open the configuration window
            //
            //PaintBoardElement* clickedElement;
            if(findClickedObject(clickedElement,*position)){
				launchCtxMenu();
            }
            break;
        default:
            break;
    }
    // repaint the widgets
    repaint();
	delete x;
	delete y;
	delete position;
}

void PaintBoard::launchCtxMenu()
{
	wxMenu ctxMenu;
	ctxMenu.Append(4114,_("Properties"));
	ctxMenu.Append(4124,_("Delete"));
	if (dynamic_cast<PaintBoardNode*>(clickedElement)){
		ctxMenu.Append(4134,_("Move"));
	}
	ctxMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&PaintBoard::onPopupMenuClick, NULL, this);
	PopupMenu(&ctxMenu);
}


void PaintBoard::onPopupMenuClick(wxMouseEvent& event)
{


	//void *data=static_cast<wxMenu *>(event.GetEventObject())->GetClientData();
	switch(event.GetId()) {
		case 4114:
			configureElement();
			break;
		case 4124:
			deleteElement();
			break;
		case 4134:
			moveNode();
			break;
	}

}
 void PaintBoard::mouseWheelMoved(wxMouseEvent& event) {} // NOT USED
 void PaintBoard::mouseReleased(wxMouseEvent& event) {} // NOT USED
 void PaintBoard::mouseLeftWindow(wxMouseEvent& event) {} // NOT USED
 void PaintBoard::keyPressed(wxKeyEvent& event) {} // NOT USED
 void PaintBoard::keyReleased(wxKeyEvent& event) {} // NOT USED

 void PaintBoard::rightClick(wxMouseEvent& event)
 {
    // Right click event handler
    switch(currentState){
        case LINKSTOP:
            // Undo a link if it has not been attached by its end
            // set state to NONE
            setMState(NONE);

            // delete link from list
            presentElements.pop_back();

            //repaint widget to delete eliminated link
            repaint();
            break;
        case STATE:
        case NONE:

            // If clicked on (or near enough to) an element painted on the board, open the configuration window
            //
            //PaintBoardElement* clickedElement;
 /*           long *x,*y;
            x = new long(0.0);
            y = new long(0.0);
            event.GetPosition(x,y);
            //!< \todo pass this function (getting the current point from the event) to a function
            // Create a point with the position information
*/
 /*           wxPoint *position;
            position = new wxPoint(*x,*y);

            if(findClickedObject(clickedElement,*position)){
                wxMenu* ctxMenu = new wxMenu();
                ctxMenu->Append(11,_("Properties"));
                ctxMenu->Append(12,_("Delete"));
                ctxMenu->Connect(11, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&PaintBoard::configureElement, NULL, this);
                ctxMenu->Connect(12, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&PaintBoard::deleteElement, NULL, this);
                PopupMenu(ctxMenu);
            }
*/

            break;

			// Some cases are not handled: (JL)
		default:
			break;
     }

 }



void PaintBoard::configureElement()
{
    clickedElement->launchConfigurator();
}

void PaintBoard::moveNode()
{
    setMState(MOVE);
}


void PaintBoard::moveNodeStop(PaintBoardNode *node, wxPoint position)
{
    // repaint the node
    node->drawForm(position);

    // repaint the links
    PaintBoardLink *link;
    std::vector<dock_element>::iterator dockiter = (node->docks).begin();
    //wxPoint dockPos;
    // iterate through all the docks of the node
    while (dockiter!=(node->docks).end()){
        if (!(*dockiter).IsFree){
            // get a reference to the link
            link = static_cast<PaintBoardLink*>((*dockiter).link);

            // get the new position of the dock
           // dockPos = (*dockiter).location;

            // repaint the link
            if ((*dockiter).incoming){
                link->setEnd(*((*dockiter).location));
            }
            else
            {
                link->setStart(*((*dockiter).location));
            }
        }
        dockiter ++;
    }
    setMState(NONE);
}


void PaintBoard::deleteElement()
{
    std::vector<PaintBoardElement*>::iterator elementIter = presentElements.begin();
  //  elementIter = find(presentElements.begin(),presentElements.end(),clickedElement);
    while (elementIter != presentElements.end()){
        if (*elementIter == clickedElement){
            clickedElement->close();
          //  presentElements.erase(elementIter);
            repaint();
            break;
        }
        elementIter++;
    }

  //  clickedElement->launchConfigurator();
}


void PaintBoard::paintEvent(wxPaintEvent & evt)
{
    // Repaint event handler
  //  repaint();
}

void PaintBoard::drawAState(wxMouseEvent& event)
{
    /** Function to draw a state on the board based on a mouse event
      */

    // Get the position where the right button was clicked (info contained in event)
    long *x,*y;
    x = new long(0.0);
    y = new long(0.0);
    event.GetPosition(x,y);

    // Create a point with the position information
    wxPoint *centre;
    centre = new wxPoint(*x,*y);

    // Call the real drawing function
    drawAStateFromPos(centre,false);
	delete x;
	delete y;
	delete centre;
/*
    // Create the node and paint it on the board
    PaintBoardNode *node;
    node = new PaintBoardNode(this);
    node->drawForm(*centre);

    // Add it to the vector of objects
    presentElements.push_back((PaintBoardElement*)node);    // the state is upcasted to a general element
*/
    // Optionally reset the state of the widget. Activated for usability reasons
    //setMState(NONE);
}

PaintBoardNode* PaintBoard::drawAStateFromPos(wxPoint *location, bool configured){
    /** Function to draw a state on the board and save a reference to it in the list of objects
      * A state is represented by a PaintBoardNode object (It is represented as a node in the graph).
      * This object has the capability to paint itself so this function only creates the object,
      * orders it to paint itself and saves a reference
      */

    // Create the node and paint it on the board
    PaintBoardNode *node;
    node = new PaintBoardNode(this);
    if (configured){
        node->setConfigured();
    }
    node->drawForm(*location);

    // Add it to the vector of objects
    presentElements.push_back((PaintBoardElement*)node);    // the state is upcasted to a general element

    // Optionally reset the state of the widget. Deactivated for usability reasons
    // setMState(NONE);
    return node;
}

void PaintBoard::drawALinkStart(wxMouseEvent& event)
{
    /** Function to paint a link between two nodes representing a transition between two states
      * This function will fixate the beginning of the link to the nearest dock of the nearest node.
      * \todo Set policy when there is no nearby dock
      */

    // Get the position where the right button was clicked (info contained in event)
    long *x,*y;
    x = new long(0.0);
    y = new long(0.0);
    event.GetPosition(x,y);

    // Create a point with the position information
    wxPoint *startPoint;
    startPoint = new wxPoint(*x,*y);

    // Create the node and paint it on the board
    drawALinkFromPosStart(*startPoint,false);

	delete x;
	delete y;
	delete startPoint;
}


PaintBoardLink* PaintBoard::drawALinkFromPosStart(wxPoint location,bool configured)
{
    /** Function to paint a link between two nodes representing a transition between two states
      * This function will fixate the beginning of the link to the nearest dock of the nearest node.
      * \todo Set policy when there is no nearby dock
      */

    // Create the node and paint it on the board
    PaintBoardLink *link;
    link = new PaintBoardLink(this);
    if (link->setStart(location)){
        // Start looking for a destination
        setMState(LINKSTOP);
        // If the element is configured, prevent the configurator from launching
        if (configured){
            link->setConfigured();
        }
        // Add the object to the elements vector
        presentElements.push_back((PaintBoardElement*)link);
    } else {
     //   wxMessageBox(_("Inexistent or busy dock. Link not created"),_("Information"),wxICON_INFORMATION,NULL,100,100);
    }

    return link;
}



void PaintBoard::drawALinkStop(wxMouseEvent& event)
{
    /** Function to paint a link between two nodes representing a transition between two states
      * This function will fixate the end of the link to the nearest dock of the nearest node.
      * It will also engage the nodes, making them aware of the transition (handled by the link object).
      * \todo Set policy when there is no nearby dock
      */

    // Redraw the link in the actual position of the mouse or the nearest dock. At any moment there is only one link active.
    updateActualLink(event);

    // Retrieve the link from the objects vector
    PaintBoardElement *getLink;
    PaintBoardLink *link;
    getLink = presentElements.back(); // The last object added in the vector is the actual link
    link = static_cast<PaintBoardLink*>(getLink); // Downcast the reference (PaintBoardElement) to match the type of a link (PaintBoardLink)

    // Engage the nodes at the ends of the link (returns false if a dock is busy)
    if(link->fixate()){
        // Reset the state of the widget
        setMState(NONE);


        // hide the docks
        std::vector<PaintBoardElement*>::iterator elements = this->presentElements.begin();
        PaintBoardNode *node;

        while(elements != this->presentElements.end()){
            node = dynamic_cast<PaintBoardNode*>(*elements);
            if (node){ // if element is a node
                node->hideDocks();
            }
            elements ++;
        }


    } else {
      //  wxMessageBox(_("Inexistent or busy dock. Link not created"),_("Information"),wxICON_INFORMATION,NULL,100,100);
    }


}


void PaintBoard::drawALinkFromPosStop(wxPoint location)
{
    /** Function to paint a link between two nodes representing a transition between two states
      * This function will fixate the end of the link to the nearest dock of the nearest node.
      * It will also engage the nodes, making them aware of the transition (handled by the link object).
      * \todo Set policy when there is no nearby dock
      */

    // Redraw the link in the actual position of the mouse or the nearest dock. At any moment there is only one link active.
    updateActualLink(location);

    // Retrieve the link from the objects vector
    PaintBoardElement *getLink;
    PaintBoardLink *link;
    getLink = presentElements.back(); // The last object added in the vector is the actual link
    link = static_cast<PaintBoardLink*>(getLink); // Downcast the reference (PaintBoardElement) to match the type of a link (PaintBoardLink)

    // Engage the nodes at the ends of the link (returns false if a dock is busy)
    if(link->fixate()){
        // Reset the state of the widget
        setMState(NONE);
        link->engage();
    } else {
      //  wxMessageBox(_("Inexistent or busy dock. Link not created"),_("Information"),wxICON_INFORMATION,NULL,100,100);
    }

}


void PaintBoard::updateActualLink(wxMouseEvent& event)
{
    /** Function to paint a link between its origin and the actual position of the mouse cursor (or the nearest dock)
      */

    // Get the position where the right button was clicked (info contained in event)
    long *x,*y;
    x = new long(0.0);
    y = new long(0.0);
    event.GetPosition(x,y);

    // Create a point with the position information
    wxPoint *endPoint;
    endPoint = new wxPoint(*x,*y);

    // Retrieve the link from the objects vector
    PaintBoardElement *getLink;
    PaintBoardLink *link;
    getLink = presentElements.back(); // The last object added in the vector is the actual link
    link = static_cast<PaintBoardLink*>(getLink); // Downcast the reference (PaintBoardElement) to match the type of a link (PaintBoardLink)

    // Set the end at the present point or a nearby dock
    link->setEnd(*endPoint);

	delete x;
	delete y;
	delete endPoint;
}

void PaintBoard::updateActualLink(wxPoint endPoint)
{
    /** Function to paint a link between its origin and the actual position of the mouse cursor (or the nearest dock)
      */

    // Retrieve the link from the objects vector
    PaintBoardElement *getLink;
    PaintBoardLink *link;
    getLink = presentElements.back(); // The last object added in the vector is the actual link
    link = static_cast<PaintBoardLink*>(getLink); // Downcast the reference (PaintBoardElement) to match the type of a link (PaintBoardLink)

    // Set the end at the present point or a nearby dock
    link->setEnd(endPoint);
}




void PaintBoard::repaint()
{
    /** Function to repaint the widget. It will order all the present objects to repaint themselves
      */

    // Create a wxClientDC object and use it to clear the widget
    wxClientDC *dc = new wxClientDC(this);
    dc->Clear();

    // Iterate through the objects and send them a repaint order
    std::vector<PaintBoardElement*>::iterator elementsIter = presentElements.begin();
    while( elementsIter != presentElements.end() ) {
        (*elementsIter)->repaint();
        ++elementsIter;
    }

	delete dc;
}


std::string PaintBoard::SaveXML()
{


    wxString *name = new wxString();
    //wxString *nameDest = new wxString();
    wxString *nameInitial = new wxString();
    PaintBoardNode *node; // *linked;
    //bool incoming;
    std::vector<PaintBoardElement*>::iterator elements = this->presentElements.begin();
    std::vector<dock_element> links;
    wxString *linkedNodesString = new wxString();
    // wxString *tmpString = new wxString();
    TiXmlDocument doc;
    TiXmlElement *nodeXml=NULL;
    TiXmlElement *scxml;
    TiXmlElement *datamodel=NULL;
    TiXmlElement *varXml=NULL,*varValXml=NULL;
    TiXmlText *varValTxt=NULL;
    scxml = new TiXmlElement("scxml");
    // start variables
	if (variables.size() > 0){
		datamodel = new TiXmlElement("datamodel");
		varValTxt = new TiXmlText("0");
		std::vector<std::string>::iterator dit;
		dit = variables.begin();
		while (dit!=variables.end()){
			varXml = new TiXmlElement("data");
			varValXml = new TiXmlElement("val");
			varXml->SetAttribute("name",(*dit).c_str());
			varValXml->InsertEndChild(*varValTxt);
			varXml->LinkEndChild(varValXml);
			datamodel->LinkEndChild(varXml);
			dit++;
		}
		scxml->LinkEndChild(datamodel);
	}
    // check all elements (states)
    while(elements != this->presentElements.end()){
        *linkedNodesString = _("");
        node = dynamic_cast<PaintBoardNode*>(*elements);

        if (node){ // if element is a node


            *name = node->getName();         // get general info of the node
            nodeXml = node->SCXMLEntry();
            scxml->LinkEndChild(nodeXml);

            if (node->isInitial()){
                *nameInitial = *name;
            }

        }

        elements ++;
    }
    scxml->SetAttribute("initialstate",nameInitial->mb_str());
    doc.LinkEndChild(scxml);
    //wxFileDialog *fileD = new wxFileDialog((wxWindow*)parent, *(new wxString(_("Choose file"))),*(new wxString(_(""))),*(new wxString(_(""))),*(new wxString(_("SCXML files|*.scxml|"))),wxSAVE,wxDefaultPosition);
	wxFileDialog *fileD = new wxFileDialog((wxWindow*)parent, *(new wxString(_("Choose file"))),*(new wxString(_(""))),*(new wxString(_(""))),*(new wxString(_("SCXML files|*.scxml|"))),0,wxDefaultPosition);
    if (!saved){
        int didSave;
        didSave = fileD->ShowModal();
        if (didSave == wxID_OK){
            saved = true;
            *fileName = fileD->GetPath();
        }
    }
	
wxMessageBox(_("Pasa"));
    if (saved){     // cannot do "else" here, because this "if" must be evaluated after the previous
        doc.SaveFile(fileName->mb_str());
    }
  /*  delete fileD;
	delete name;
	delete nameInitial;
	delete linkedNodesString;
	delete scxml;
	delete datamodel;
	delete varValTxt;
	delete varXml;
	delete varValXml;*/
    return std::string(fileName->mb_str());
}

void PaintBoard::addVariable(wxString var)
{
    std::vector<std::string>::iterator it;
    it = std::find(variables.begin(),variables.end(),std::string(var.mb_str()));
    if(it == variables.end()){
        variables.push_back(std::string(var.mb_str()));
    }
}


struct link_read_info {
    //wxPoint start,end;
     std::string event,condition,conditionType,varName,ID_Orig,ID_Dest;
    int Dock_Orig,Dock_Dest;
};



void PaintBoard::openStateChart()
{
    presentElements.clear();
    int didOpen;
    //wxFileDialog *fileD = new wxFileDialog((wxWindow*)parent, *(new wxString(_("Choose file"))),*(new wxString(_(""))),*(new wxString(_(""))),*(new wxString(_("SCXML files|*.scxml|"))),wxOPEN,wxDefaultPosition);
	    wxFileDialog *fileD = new wxFileDialog((wxWindow*)parent, *(new wxString(_("Choose file"))),*(new wxString(_(""))),*(new wxString(_(""))),*(new wxString(_("SCXML files|*.scxml|"))),1,wxDefaultPosition);
    didOpen = fileD->ShowModal();
    if (didOpen == wxID_OK){
        saved = true;
    }
    *fileName = fileD->GetPath();
    delete fileD;

    TiXmlDocument doc( fileName->mb_str() );
    bool loadOkay = doc.LoadFile();
    std::vector<link_read_info> links;
    link_read_info linkTmp;
    // wxMouseEvent *fakeClick;
    if ( loadOkay )
    {

        TiXmlElement  *node,*nodeSave,*scxml, *link, *onentry, *action, *variable;
        TiXmlNode *comment;
        scxml = doc.FirstChildElement( "scxml" );


        std::string commentStr;
        //wxString initialStateName;
        char commentPtr[1024],*commentVal;
        int x,y;
		int x0 = 50;
		int y0 = 50;
        std::vector<PaintBoardAction*> actionList;

        if ( scxml )
        {
            // ignore the variables section

            node = scxml->FirstChildElement("datamodel");

			if (node){
				variable = node->FirstChildElement("data");

				// parse the variables
				while (variable){
					variables.push_back(std::string(variable->Attribute("name")));
					variable = variable->NextSiblingElement("data");
					// values are ignored by the moment
					printf("VAR\n");
				}
			}

            // Parse states
            node = scxml->FirstChildElement("state");
            wxString initialStateName(scxml->Attribute("initialstate"), wxConvUTF8);
            bool final = false;

            while ( node )
            {
                //cout << "Parameter: name= '" << pParm->Attribute("name") << "', value='" << pParm->Attribute("value") << "'" << endl;
                // Draw the state

                comment = node->FirstChild();

				while(comment && comment->Type()!=2){   // Should be TINYXML_COMMENT, but won't compile that way
					comment = comment->NextSibling();
					/*wxString err;
					err = wxString::Format(_("Type %i"),comment->Type());
					wxMessageBox(err);*/
				}
				if (comment && comment->Type()==2){
				   //commentPtr = free(1024);

					strcpy(commentPtr,comment->Value());
				   // *commentPtr = str.c_str();

					commentVal = strtok(commentPtr,";");    // returns "NODE"

				   // commentVal = strtok(NULL,";");          // returns "NODE"

					commentVal = strtok(NULL,";");          // returns the value of the X coord
					x = atoi(commentVal);


					commentVal = strtok(NULL,";");          // returns the value of the Y coord
					y = atoi(commentVal);
				} else {

					x = x0;
					x0 = x0 + 100;
					y = y0;
					y0 = y0 + 100;
				}

                wxPoint *centre;
                centre = new wxPoint(x,y);

                PaintBoardNode *newNode;
                newNode = drawAStateFromPos(centre,true);

				delete centre;

                // Configure the properties of the node

                //PaintBoardElement *tmp;
                //tmp = presentElements.back();
                //newNode = dynamic_cast<PaintBoardNode*>(tmp);

                wxString newNodeName(node->Attribute("id"), wxConvUTF8);

                newNode->setName(newNodeName);
                if (newNodeName == initialStateName){
                    newNode->setInitial(true);
                }


                newNode->setFinal(final);
                final = false;

                onentry = node->FirstChildElement("onentry");
                // parse the variables
                variable = onentry->FirstChildElement("assign");
               // std::vector<std::string>::iterator assi;
                while (variable){
                    //assi = std::find(variables.begin(),variables.end(),std::string(variable->Attribute("name")));
                    variable = onentry->NextSiblingElement("assign");
                }

                action = onentry->FirstChildElement("log");
                PaintBoardAction *newAction;
                std::string expr;
                char exprPtr[1024],*exprVal;
                while (action){

                    newAction = new PaintBoardAction();
                    newAction->SetID(*(new wxString(action->Attribute("label"),wxConvUTF8)));
                    expr = action->Attribute("expr");
                    strcpy(exprPtr,expr.c_str());
                    exprVal = strtok(exprPtr+1," ");
                    newAction->SetType(*(new wxString(exprVal,wxConvUTF8)));
                    exprVal = strtok(NULL," ");
                    strcpy(exprPtr,"\0");
                    while (exprVal){
                        strcat(exprPtr,exprVal);
                        exprVal = strtok(NULL," ");
                        if (exprVal){
                            strcat(exprPtr," ");
                        } else {
                            int leng = strlen(exprPtr);
                            exprPtr[leng-1] = '\0';
                        }
                    }
                   // std::string exprFinal = exprPtr.substr(0,exprPtr.length()-1);

                    newAction->SetParameters(*(new wxString(exprPtr,wxConvUTF8)));
                    // TODO: repetitions (still not written on the file)
                    actionList.push_back(newAction);
                    action = action->NextSiblingElement("log");
                }
                newNode->setActionList(actionList);
                actionList.clear();

                // scan for links
				if (comment){
					comment = comment->NextSibling();
				} else {
					comment = node->FirstChildElement("transition");

				}
                char bufLink[1024];
                char varname[256];
                while (comment){

                    // Each link is represented by a usual XML element and a comment
                    link = comment->ToElement();
                    //std::string eventTmp = link->Attribute("event");
                    //linkTmp.event=eventTmp.substr(1,eventTmp.length());
                    linkTmp.event = link->Attribute("event");
                    if(link->Attribute("cond")){

                        strcpy(bufLink,link->Attribute("cond"));
                        strcpy(varname,strtok(bufLink,"."));
                        linkTmp.varName = std::string(varname);
                        linkTmp.condition = std::string(bufLink+strlen(varname)+4); // to eliminate the "val"
                        linkTmp.conditionType = std::string("RANDOM");

                    } else {
                        linkTmp.condition = "";
                    }
                    comment = comment->NextSibling();

					if ((comment) && (comment->Type() == 2)){

						strcpy(commentPtr, comment->Value());
						//*commentPtr = str.c_str();
						commentVal = strtok(commentPtr,";");    // returns ""
					   // commentVal = strtok(NULL,";");          // returns "NODE"
						commentVal = strtok(NULL,";");          // returns the name of the origin node
						linkTmp.ID_Orig = commentVal;
						commentVal = strtok(NULL,";");          // returns the name of the destination node
						linkTmp.ID_Dest = commentVal;
						commentVal = strtok(NULL,";");          // returns the value of the dock in the origin node
						linkTmp.Dock_Orig = atoi(commentVal);
						commentVal = strtok(NULL,";");          // returns the value of the dock in the destination node
						linkTmp.Dock_Dest = atoi(commentVal);
						links.push_back(linkTmp);
						comment = comment->NextSibling();

					} else {

						linkTmp.ID_Orig = std::string(node->Attribute("id"));

						linkTmp.ID_Dest = std::string(link->Attribute("target"));

						linkTmp.Dock_Orig = DOCKS + 1;
						linkTmp.Dock_Dest = DOCKS + 1;
						links.push_back(linkTmp);

					}
                }

                nodeSave = node;
                node = node->NextSiblingElement( "state" );
                if (!node){
                    node = nodeSave->NextSiblingElement("final");
                    final = true;
                }
            }





            // At this point, all the states are created and drawn.
            // It is time to start drwaing the transitions
            std::vector<link_read_info>::iterator linkIter = links.begin();


            wxPoint startP,endP;
            PaintBoardNode *orig,*dest;
            PaintBoardLink *newLink;
            PaintBoardEvent *newEvent;
			std::map<PaintBoardNode *,int> busyDock; // used only for storing the highest busy dock in the case that the SCXML doesn't have the required comments
			// fill the map
			std::vector<PaintBoardElement*>::iterator iterNodes;
			for (iterNodes = presentElements.begin(); iterNodes!=presentElements.end(); iterNodes ++){
				busyDock[static_cast<PaintBoardNode*>(*iterNodes)] = -1; // at this point all elements are nodes, so no check is done of the type of element

			}

            while (linkIter != links.end()){

                orig = findNodeByName((*linkIter).ID_Orig);
                dest = findNodeByName((*linkIter).ID_Dest);

				if ((*linkIter).Dock_Orig == DOCKS + 1){
					// if the dock is not established from the file (IE: the file doesn't have comments), then find the first free dock


					(*linkIter).Dock_Orig = busyDock[orig] + 1;
					busyDock[orig] = busyDock[orig] + 1;

					(*linkIter).Dock_Dest = busyDock[dest] + 1;
					busyDock[dest] = busyDock[dest] + 1;
				}

                startP = orig->getDock((*linkIter).Dock_Orig);
                endP = dest->getDock((*linkIter).Dock_Dest);
                newLink = drawALinkFromPosStart(startP,true);

                newLink->setConfigured();
                newEvent = new PaintBoardEvent();

                newEvent->SetEvent(wxString::FromAscii((*linkIter).event.c_str()));
                newEvent->SetCondition(wxString::FromAscii((*linkIter).condition.c_str()));
                newEvent->SetConditionType(wxString::FromAscii((*linkIter).conditionType.c_str()));
                newEvent->SetVariable(wxString::FromAscii((*linkIter).varName.c_str()));

                newLink->setEvent(newEvent);
                drawALinkFromPosStop(endP);
                //newLink->repaint();
                linkIter++;
            }

        }
        repaint();
    }
}

PaintBoardNode* PaintBoard::findNodeByName(std::string nodeID)
{
    PaintBoardNode *node;
    node = NULL;
    std::vector<PaintBoardElement*>::iterator elemIter = presentElements.begin();
    while(elemIter != presentElements.end()){
        node = dynamic_cast<PaintBoardNode*>(*elemIter);
        if (node)
        {
            if (std::string(node->getName().mb_str()) == nodeID){
                break;
            }
        }
        elemIter++;
    }
    return node;
}





bool PaintBoard::findClickedObject(PaintBoardElement*& clickedElement, wxPoint location)
{
    // find a dock near the current location. Used in painting the start and end of the links
    // output is done through parameters nearestNode and nearestDock. If a dock is found, the function will return true.

    bool found = false;     // true if a dock is found in the sensitivity range
   // int i,distance2;
   // int lowestDistance2 = DOCK_SENSITIVITY_RANGE*DOCK_SENSITIVITY_RANGE+1;  // initialize to maximum distance^2 + 1. Any nearby dock will be closer
    int minimumDistance2 = (RADIUS+CLICK_SENSITIVITY)*(RADIUS+CLICK_SENSITIVITY);
    int distance2;
    wxPoint *diff = new wxPoint();
    PaintBoardNode *node;
    PaintBoardLink *link;
    std::vector<PaintBoardElement*>::iterator elementsIter = this->presentElements.begin();  // Downcast the board from wxPanel to PaintBoard and get the list of present elements

    while( elementsIter != this->presentElements.end() ) {
        // do a dynamic downcast to ensure that the object taken is a node
        node = dynamic_cast<PaintBoardNode*>(*elementsIter);
        link = dynamic_cast<PaintBoardLink*>(*elementsIter);

        if (node){
            // if the element is a node
            *diff = node->getPosition() - location;        // get the difference vector
            distance2 = diff->x*diff->x + diff->y*diff->y;  // calculate distance^2
            if(distance2 <= minimumDistance2){
                found = true;
                clickedElement = (PaintBoardElement*)node;
            }

        } else if(link) {
            // if clicked inside an oval surrounding the line
            float linkLength2 = pow((double)(link->start->x - link->end->x),2) + pow((double)(link->start->y - link->end->y),2);
            float totalLength = 2*sqrt(CLICK_SENSITIVITY*CLICK_SENSITIVITY + linkLength2/4);
            float distanceToStart = sqrt(pow((double)(link->start->x - location.x),2) + pow((double)(link->start->y - location.y),2));
            float distanceToEnd = sqrt(pow((double)(link->end->x - location.x),2) + pow((double)(link->end->y - location.y),2));
            if (distanceToStart+distanceToEnd <= totalLength){
                //clicked near this line
                clickedElement = (PaintBoardElement*)link;
                found = true;
            }

        }
        ++elementsIter;
    }
	delete diff;
    return found;
}




