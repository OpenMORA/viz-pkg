#include "PaintBoardNode.h"
#include "PaintBoardLink.h"
#include "PaintBoard.h"
#include <wx/dc.h>
#include <iostream>
#include <algorithm>

// Constructor (calls the constructor of the superclass)
PaintBoardNode::PaintBoardNode(wxPanel* targetBoard)
: PaintBoardElement(targetBoard)
{
    // set variables
    final = false;
    initial = false;
    firstConfig = true;
    showDocksBool = false;
    name = *(new wxString(_("Unnamed")));
    //repetitions = 1;

    // create docks
    int i = 0;
    dock_element dockPoint;
    for (i=0;i<DOCKS;i++){
        // initialize the fields of the dock
        dockPoint.location = new wxPoint();
        dockPoint.IsFree = true;
        docks.push_back(dockPoint);
    }

    // initialize the configurator
    configurator = new PaintBoardElementConfigurator(name,NODET,this);

}

PaintBoardNode::~PaintBoardNode()
{
    // Destructor

    // destroy the configurator
    configurator->Destroy();
    delete configurator;
}



// painting method, used for painting the object on screen
void PaintBoardNode::drawForm(wxPoint position)
{
    // set position
    actualPosition = position;

    // obtain a wxClientDC for painting on the board
    wxClientDC *dc = new wxClientDC(board);

    // set the background color and line for the circle
    wxBrush *brush = new wxBrush(*wxCYAN_BRUSH);
    dc->SetBrush(*brush);
    wxPen *pen = new wxPen(*wxBLACK, LINE_WIDTH, wxSOLID);
    dc->SetPen(*pen);

    // draw the circle
    dc->DrawCircle(actualPosition, RADIUS);

    wxCoord *w,*h;
    w = new wxCoord();
    h = new wxCoord();

    int xpos, ypos;
    dc->GetTextExtent(name, w, h);
    xpos = actualPosition.x - (int)(*w)/2;
    ypos = actualPosition.y - (int)(*h)/2;
    dc->DrawText(name,xpos,ypos);
	delete dc; delete brush; delete pen; delete w; delete h;
    // locate the dock points
    /* TEMPORARILY DISABLED AND USING ONLY ONE DOCK SITUATED AT THE TOP OF THE NODE */
    int i = 0;
    float angle;
    for (i=0;i<DOCKS;i++){
        // locate the dock

        // calculate the angle on the border of the figure
        angle = (6.28/DOCKS)*i;
        // obtain coordinates (absolute)
        docks[i].location->x = RADIUS * sin(angle) + actualPosition.x;
        docks[i].location->y = RADIUS * cos(angle) + actualPosition.y;
        if (showDocksBool)
            showDock(i);
    }
    if (firstConfig){
        // if it is the first paint, launch the configurator
        launchConfigurator();
        firstConfig = false;
    }

}

// shows (highlights) the dock given by dockID
void PaintBoardNode::showDock(int dockID)
{
    // obtain a wxClientDC
    wxClientDC dc(board);

    // obtain the position of the dock
    wxPoint *pos = new wxPoint(docks[dockID].location->x,docks[dockID].location->y);

    // draw the circle
    dc.DrawCircle(*pos, RADIUS_DOCK);
	delete pos;
}

// shows (highlights) the dock given by dockID
void PaintBoardNode::showDocks()
{
    // obtain a wxClientDC
    wxClientDC dc(board);
    showDocksBool = true;
    int i;
    for (i=0;i<DOCKS;i++)
    {
        showDock(i);
    }
}

void PaintBoardNode::hideDocks()
{
    // hides the docks

    showDocksBool = false;
    drawForm(actualPosition);
}


// linking methods, used for linking elements through their docking points
wxPoint PaintBoardNode::getDock(int dockID)
{
    // returns the coordinates of the dock given by dockID

    // get the dock from the list
    dock_element *dockPoint;
    dockPoint = &(docks[dockID]);

    // get the position and return it
    wxPoint *pos = new wxPoint(dockPoint->location->x, dockPoint->location->y);
    return *pos;
}

void PaintBoardNode::engageDock(int localDock, int remoteDock, PaintBoardNode* target, PaintBoardElement* interlink, bool incoming, PaintBoardEvent *event)
{
    // links the dock given by localDock with the remoteDock of the target node

    // get the dock
    dock_element *dockPoint;
    dockPoint = &(docks[localDock]);

    // fill the information of the dock
    dockPoint->IsFree=false;                                    // mark the dock as busy
    dockPoint->link=interlink;                                  // pointer to the link element
    dockPoint->node = target;                                   // pointer to the target element
    dockPoint->incoming = incoming;                             // direction of the link
    dockPoint->event = *(event->GetEvent());                    // set the event
    dockPoint->condition = *(event->GetCondition());            // set the condition
    dockPoint->conditionType = *(event->GetConditionType());    // set the condition type (by the moment only random variables are available)
    //std::cout << (event->GetConditionType())->mb_str() << "\n";
    dockPoint->variable = *(event->GetVariable());              // set the variable name
}

void PaintBoardNode::disengageDock(int dockID)
{
    // disengage dock only disengages the node of the current node. The linked node's dock will have to be disengaged manually
    dock_element *dockPoint;
    dockPoint = &(docks[dockID]);
    dockPoint->IsFree = true;
}

bool PaintBoardNode::dockIsFree(int dockID)
{
    // see if the dock is engaged or free
    dock_element *dockPoint;
    dockPoint = &(docks[dockID]);
    return dockPoint->IsFree;
}

void PaintBoardNode::close()
{
    // Function to close the node
    // delete all links before exiting (delete links *before* deleting node)
    std::vector<dock_element>::iterator dockiter = docks.begin();
    PaintBoardLink *link;

    // iterate through docks and close them
    while(dockiter != docks.end())
    {
        if (!(*dockiter).IsFree)
        {
            // only process the busy docks
            link = dynamic_cast<PaintBoardLink*>((*dockiter).link);
            link->close();
        }
        dockiter++;
    }

	delete configurator;

    // Delete the entry of the current node from the element list of the board
   // std::vector<PaintBoardElement*>::iterator elementIter = (static_cast<PaintBoard*>(board))->presentElements.begin();


    // iterate through the list looking for itself
/*    while (elementIter != (static_cast<PaintBoard*>(board))->presentElements.end()){
        if (*elementIter == this){
            (static_cast<PaintBoard*>(board))->presentElements.erase(elementIter);
            break;
        }
        elementIter++;
    }
*/
}


void PaintBoardNode::repaint()
{
    // repaint the node
    drawForm(actualPosition);
}


TiXmlElement* PaintBoardNode::SCXMLEntry()
{
    // generate the SCXML entry for the node

    TiXmlElement *stateEntry;

    // if the state is final, the entry needs to reflect it
    if (final)
    {
        stateEntry = new TiXmlElement("final");
    } else {
        stateEntry = new TiXmlElement("state");
    }

    // set the id of the node
    stateEntry->SetAttribute("id",name.mb_str());

    // onEntry actions
    TiXmlElement *onEntry = new TiXmlElement("onentry");
    TiXmlElement *actionEntry;
    wxString *actionExpr = new wxString();
    // iterate through the action list
    std::vector<PaintBoardAction*>::iterator actioniter = actionList.begin();
    while(actioniter != actionList.end())
    {
        actionEntry = new TiXmlElement("log");
        actionEntry->SetAttribute("label",((*actioniter)->GetID()).mb_str());
        *actionExpr = (*actioniter)->GetType() + _(" ") + (*actioniter)->GetParameters();   // create entry for the action summing up the action and the parameter
        printf("%s",  std::string(actionExpr->mbc_str()).c_str() );
        printf("\n");
        std::stringstream expr;
        expr << "\'" << actionExpr->mbc_str() << "\'";
        actionEntry->SetAttribute("expr",expr.str().c_str());
        onEntry->LinkEndChild(actionEntry);

        actioniter++;
    }
    // attach the "onentry" tag to the document
    //stateEntry->InsertEndChild(*onEntry);

    /* NEW SECTION */

    // Set the graphic information (position and incoming links)
    // This information is given as a XML comment so it is ignored by the State Charts module
    // The entries are given as:
    //  For a state:        <!--;NODE;x_coord;y_coord;-->
    //  For a transition:   <!--;LINK;id_start;id_end;dock_start;dock_end;-->
    // Where the ; are used as separators
    std::stringstream commStream;
    int x,y;
    x=actualPosition.x;
    y=actualPosition.y;
    commStream << ";NODE;" << x << ";" << y << ";";
   // sprintf(str,";NODE;%i;%i;",x,y);
    //std::string nodeComment(str);
    TiXmlComment *nodeComment = new TiXmlComment(commStream.str().c_str());
    stateEntry->LinkEndChild(nodeComment);
    /* NEW SECTION */

    /** \todo onExit actions */

    // create entries for the transitions
    std::vector<dock_element>::iterator dockiter = docks.begin();
    PaintBoardNode *destination;
    wxString *targetName = new wxString();
    wxString *eventTrans = new wxString();
    wxString *conditionTrans = new wxString();
    wxString *conditionTypeTrans = new wxString();
    wxString *variableTrans = new wxString();
    std::vector<wxString> variableList;

    PaintBoardLink *dockLink;
    TiXmlElement *transition,*assign;
    // iterate through the docks

    while(dockiter != docks.end())
    {

        if (!(*dockiter).incoming && !(*dockiter).IsFree)
        {

            // only create entry for engaged docks
            destination = dynamic_cast<PaintBoardNode*>((*dockiter).node);  // get a pointer to the destination node
            *targetName = destination->getName();                           // get the name of the destination node
            *conditionTrans = (*dockiter).condition;                        // get the condition
            *eventTrans = (*dockiter).event;                                // get the event
            *conditionTypeTrans = (*dockiter).conditionType;                // get the condition type
            *variableTrans = (*dockiter).variable;                          // get the variable name

            dockLink = dynamic_cast<PaintBoardLink*>((*dockiter).link);     // get the pointer to the link element (to take graphic information)
            transition = new TiXmlElement("transition");
            transition->SetAttribute("target",targetName->mb_str());        // set the target name
            transition->SetAttribute("event",eventTrans->mb_str());         // set the event

            //printf("LINKMSG: \n");
            if (strcmp(conditionTrans->mb_str(),"")){

//                transition->SetAttribute("cond", variableTrans->mb_str() + wxString(".val",wxConvUTF8) + conditionTrans->mb_str());      // set the condition if it is not empty
                transition->SetAttribute("cond", (*variableTrans + wxString(".val",wxConvUTF8) + *conditionTrans).mb_str());      // set the condition if it is not empty

                // check if the variable has been initilizad at the entry of this state
                std::vector<wxString>::iterator it;
                it = std::find(variableList.begin(),variableList.end(),*variableTrans);

                if(it == variableList.end()){
                    // then make an entry for initializing the variable
                    variableList.push_back(*variableTrans);

                    assign = new TiXmlElement("assign");
                    if (!strcmp(conditionTypeTrans->mb_str(),"RANDOM")){
                        assign->SetAttribute("location",(*variableTrans+wxString(".val",wxConvUTF8)).mb_str());
                        assign->SetAttribute("expr","Math.Random()");
                        onEntry->InsertBeforeChild(onEntry->FirstChild(),*assign); // insert in the beginning of <onentry>
                    }

                }

            }
            stateEntry->LinkEndChild(transition);                           // add the entry to the document

            /* NEW SECTION */

            //int origDock, destDock;

            commStream.str("");
            commStream << ";LINK;" << name.mb_str() << ";" << targetName->mb_str() << ";" << dockLink->startDock << ";" << dockLink->endDock << ";";
            //sprintf(strg,";LINK;%s;%s;%i;%i;",name,*targetName,dockLink->startDock,dockLink->endDock);
            //std::string nodeComment(str);
            TiXmlComment *nodeComment = new TiXmlComment(commStream.str().c_str());
            stateEntry->LinkEndChild(nodeComment);
            /* NEW SECTION */

        }
        dockiter++;
    }
    stateEntry->InsertBeforeChild(stateEntry->FirstChild(),*onEntry);
	//delete onEntry;
	//delete actionExpr;
	//delete nodeComment;
	//delete targetName;
	//delete eventTrans;
	//delete conditionTrans;
	//delete conditionTypeTrans;
	//delete variableTrans;
	//delete transition;
	//delete actionEntry;

	//delete nodeComment;

    return stateEntry;

}


void PaintBoardNode::addAction(wxString actionT, wxString ID, wxString actionP, int rep)
{
    // add an action to the action list (unused by the moment, because the list is updated as a whole, but kept for possible future usage)

    // create action and fill the data
    PaintBoardAction *newAction = new PaintBoardAction();
    newAction->SetID(ID);
    newAction->SetType(actionT);
    newAction->SetParameters(actionP);
    newAction->SetRepetitions(rep);

    // add the action to the list
    actionList.push_back(newAction);
}

void PaintBoardNode::launchConfigurator()
{
    // launch the configurator
    configurator->updateInfo();     // first update the information of the configurator
    configurator->Show(true);       // show the configurator

}

