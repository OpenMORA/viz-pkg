#include "PaintBoardLink.h"
#include "PaintBoard.h"

#include <stdexcept>

PaintBoardLink::PaintBoardLink(wxPanel* targetBoard)
: PaintBoardElement(targetBoard)
{
    /** Constructor
      * starts the points
      */

    start = new wxPoint(0,0);
    end = new wxPoint(0,0);
	prevend = new wxPoint(0,0);

    //event = new wxString(_("EVENT1"));/// TEMPORARY SOLUTION: AFTER THAT THE EVENTS WILL HAVE TO BE INITIALIZED *BEFORE* ANY STATE CHART IS PAINTED, SO A DEFAULT ACTION CAN BE SELECTED. OR MAKE THE PROGRAM MANAGE THE POSSIBILITY OF NO ACTION (AT THE MOMENT IT DIES)
    //condition = new wxString();
    event_ = new PaintBoardEvent();

    configurator = new PaintBoardElementConfigurator(_("Unnamed link"),LINKT,this);

}

PaintBoardLink::~PaintBoardLink()
{
    configurator->Destroy();
    delete configurator;
}

void PaintBoardLink::drawForm(wxPoint position)
{
    // draws the element on screen (overwritten by the derived class). "position" is the starting point of the line
    wxClientDC *dc = new wxClientDC(board);
	
	wxPen *deletePen = new wxPen(board->GetBackgroundColour(), LINE_WIDTH, wxSOLID);
	dc->SetPen(*deletePen);
	dc->DrawLine(start->x,start->y,prevend->x,prevend->y);
        // set line width
    wxPen *pen = new wxPen(*wxBLACK, LINE_WIDTH, wxSOLID);
    dc->SetPen(*pen);
        // paint line
    dc->DrawLine(start->x,start->y,end->x,end->y);
    // draw the tip of the arrow
    //drawTriangle(*end,angle);
    drawTriangle(*start,*end, true);
	delete dc; delete pen; delete deletePen;
}

void PaintBoardLink::setAng(float newAngle)
{
    // sets the angle and the end point acording to the given angle
    angle = newAngle;
    wxPoint *endpoint = new wxPoint(length*sin(angle),length*cos(angle));
	*prevend = *end;
    end = endpoint;
}

bool PaintBoardLink::setEnd(wxPoint endPoint)
{
    // paints the initial point of the link

    // search for nearest dock in a limited distance
    // first, search for nearby nodes which may have nearby docks
    int nearestNode, nearestDock ; // nearestNode is the index of the nearest node in the elements vector of the widget. nearestDock is the index of the dock in the vector of docks of the node
    bool found = findNearDock(nearestNode,nearestDock,endPoint);

    if (found){
        // there is a dock that is near enough so as to attach the line to it

        // set the starting node and dock
        endNode = static_cast<PaintBoardNode*>((static_cast<PaintBoard*>(board))->presentElements[nearestNode]); //first, board (type wxPanel) is casted to PaintBoard type, and after that, presentElements[nearestNode] (which is of type PaintBoardElement) is converted to PaintBoardNode
        endDock = nearestDock;
		*prevend = *end;
        // set the initial point on the widget
        *end = endNode->getDock(nearestDock);

    } else {
        // no nearby dock found
        /** \todo Better policy when a dock is not found. Ideally destroy the object*/
        endNode = NULL;
        endDock = 0; // NULL
		*prevend = *end;
        *end = endPoint; // set en point under where the mouse clicked
    }

    // get the angle
    wxPoint difference = *end - *start;
    if (difference.y != 0){
        // prevent a divide by zero situation
        angle = atan((double)(difference.x/difference.y));
    } else {
        angle = 0;
    }

    // redraw the link
    drawForm(*start);
    return found;
}

bool PaintBoardLink::setStart(wxPoint startPoint)
{
    // paints the initial point of the link

    // search for nearest dock in a limited distance
    // first, search for nearby nodes which may have nearby docks
    int nearestNode, nearestDock ; // nearestNode is the index of the nearest node in the elements vector of the widget. nearestDock is the index of the dock in the vector of docks of the node
    bool found = findNearDock(nearestNode,nearestDock,startPoint);

    if (found){
        // there is a dock that is near enough so as to attach the line to it

        // set the starting node and dock
        startNode = static_cast<PaintBoardNode*>((static_cast<PaintBoard*>(board))->presentElements[nearestNode]); //first, board (type wxPanel) is casted to PaintBoard type, and after that, presentElements[nearestNode] (which is of type PaintBoardElement) is converted to PaintBoardNode

        // check if the dock is free
        if (startNode->dockIsFree(nearestDock)){
            startDock = nearestDock;

            // set the initial point on the widget
            *start = startNode->getDock(nearestDock);
        } else {
            found = false;
        }


    } else {
        // no nearby dock found
        /** \todo Better policy when a dock is not found. Ideally destroy the object*/
        startNode = NULL;
        startDock = 0; // NULL

        *start = startPoint;
    }

    // redraw the link
    drawForm(*start);

    return found;
}

void PaintBoardLink::engage()
{
        // engage the docks in inital and final nodes
        /** \todo Act when there is no node at either end of the line
          * \todo Act when a dock is not free
          */


        if (startNode && endNode){
            if (startNode->dockIsFree(startDock) && endNode->dockIsFree(endDock)){
         //       launchConfigurator(); // For this to work, the program should stop here, which I'm not very sure it will (actually almost sure it will not)
                //wxString *temporal = new wxString(_("Not implemented"));

                startNode->engageDock(startDock,endDock,endNode,this,false,event_); /// \todo In this point, the event and condition are passed as striungs. Implement them as event objects
                endNode->engageDock(endDock,startDock,startNode,this,true,event_);

               // startNode->engageDock(startDock,endDock,endNode,false);
               // endNode->engageDock(endDock,startDock,startNode,true);

            }
        }
}

bool PaintBoardLink::fixate()
{
    bool engaged = false;
    if (startNode && endNode){
            if (startNode->dockIsFree(startDock) && endNode->dockIsFree(endDock)){
                    if (firstConfig){
                        // if it is the first paint, launch the configurator
                        launchConfigurator();
                        firstConfig = false;
                    }
                engaged = true;
            }
    }
    return engaged;
}

void PaintBoardLink::close()
{
    startNode->disengageDock(startDock);
    endNode->disengageDock(endDock);

//    endNode = static_cast<PaintBoardNode*>((static_cast<PaintBoard*>(board))->presentElements[nearestNode]); //first, board (type wxPanel) is casted to PaintBoard type, and after that, presentElements[nearestNode] (which is of type PaintBoardElement) is converted to PaintBoardNode
	delete configurator;
//    std::vector<PaintBoardElement*>::iterator elementIter = (static_cast<PaintBoard*>(board))->presentElements.begin();
  //  elementIter = find(presentElements.begin(),presentElements.end(),clickedElement);
 /*   while (elementIter != (static_cast<PaintBoard*>(board))->presentElements.end()){
        if (*elementIter == this){
           // clickedElement->close();
          (static_cast<PaintBoard*>(board))->presentElements.erase(elementIter);
           // repaint();
            break;
        }
        elementIter++;
    }*/

//    (static_cast<PaintBoard*>(board)->presentElements).erase((static_cast<PaintBoardElement*>(this)));
}


void PaintBoardLink::repaint()
{
    // Repaint the link starting at the beginning point
    drawForm(*start);
}

void PaintBoardLink::addVariable(wxString var)
{
    static_cast<PaintBoard*>(board)->addVariable(var);
}

bool PaintBoardLink::findNearDock(int& nearestNode, int& nearestDock, wxPoint location)
{
    // find a dock near the current location. Used in painting the start and end of the links
    // output is done through parameters nearestNode and nearestDock. If a dock is found, the function will return true.

    bool found = false;     // true if a dock is found in the sensitivity range
    int i,distance2;
    int lowestDistance2 = DOCK_SENSITIVITY_RANGE*DOCK_SENSITIVITY_RANGE+1;  // initialize to maximum distance^2 + 1. Any nearby dock will be closer

    wxPoint *diff = new wxPoint();
    PaintBoardNode *node;
    std::vector<PaintBoardElement*>::iterator elementsIter = static_cast<PaintBoard*>(board)->presentElements.begin();  // Downcast the board from wxPanel to PaintBoard and get the list of present elements

    while( elementsIter != (static_cast<PaintBoard*>(board))->presentElements.end() ) {
        // do a dynamic downcast to ensure that the object taken is a node
        node = dynamic_cast<PaintBoardNode*>(*elementsIter);

        if (node){
            // find the nearest dock
            for(i=0;i<DOCKS;i++){
                *diff = node->getDock(i) - location;        // get the difference vector
                distance2 = diff->x*diff->x + diff->y*diff->y;  // calculate distance^2

                if ((distance2 < lowestDistance2) && node->dockIsFree(i)){
                    // the actual dock is nearer than the nearest dock found so far and it is free
                    lowestDistance2 = distance2;
                    nearestNode = elementsIter - static_cast<PaintBoard*>(board)->presentElements.begin();  // get the index of the node in the elements list
                    nearestDock = i;    // get the index of the nearest dock in the node
                    found = true;
                }
            }
        }
        ++elementsIter;
    }
    return found;
}

void PaintBoardLink::launchConfigurator()
{

    configurator->updateInfo();
    configurator->Show(true);
}

void PaintBoardLink::drawTriangle(wxPoint start, wxPoint end, bool deleting){
	// Delete previous triangle
	if (!deleting)
		drawTriangle(start, *prevend, false);
    // Drawing the triangle using vectorial math
    float distanceSE;
    wxPoint arrowDir,perp1,perp2, arrowBottom, SE;

    distanceSE = sqrt((float)((start.x-end.x)*(start.x-end.x)+(start.y-end.y)*(start.y-end.y)));
    SE.x = end.x - start.x;
    SE.y = end.y - start.y;
    arrowDir.x = SE.x / distanceSE * ARROW_SIZE;
    arrowDir.y = SE.y / distanceSE * ARROW_SIZE;

    // calculate the perpendicular
    perp1.x = arrowDir.y / 2;
    perp1.y = -arrowDir.x / 2;

    perp2.x = -arrowDir.y / 2;
    perp2.y = arrowDir.x / 2;

    arrowBottom.x = end.x - arrowDir.x;
    arrowBottom.y = end.y - arrowDir.y;

    wxPoint p[3];
    p[0] = end;
    p[1] = perp1 + arrowBottom;
    p[2] = perp2 + arrowBottom;

    // draw arrow end
    wxClientDC *dc;
    dc = new wxClientDC(board);
        // set color fill
	wxBrush *brush;
	if (deleting)
		brush = new wxBrush(board->GetBackgroundColour());
	else
		brush = new wxBrush(*wxBLACK_BRUSH);

    dc->SetBrush(*brush);

        // Draw the triangle
    dc->DrawPolygon(3,p);
	delete dc;
	delete brush;
}
