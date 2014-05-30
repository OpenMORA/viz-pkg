/** Painting surface widget link element
  * This object represents a transition between two states (each represented by a node). It links the two nodes by two of their docks
  * The link will try to find the nearest docks to its beginning and end, and attach itself to them. If it fails in doing so, the line
  * \todo Decide policy when a nearby dock is not found at either end
  *
  */
#ifndef PAINTBOARDLINK_H
#define PAINTBOARDLINK_H

#include <wx/wx.h>
#include <wx/dcclient.h>
#include "PaintBoardElement.h"
#include "PaintBoardNode.h"


/// Constants for the Link object
#define DOCK_SENSITIVITY_RANGE 20   // Range inside which the node will see a dock and try to attach to it
#define ARROW_SIZE  10              // Size of the arrow

class PaintBoardLink : public PaintBoardElement
{
    /// The PaintBoardLink class represents a transition between two states (each represented by a node). It links the two nodes by one of their docks
    /// The link will try to find the nearest docks to its beginning and end, and attach itself to them. If it fails in doing so, it will inform the user and not create the link
    public:
        // Constructor
        PaintBoardLink(wxPanel* targetBoard);
        // Destructor
        ~PaintBoardLink();

        // Drawing functions
        void drawForm(wxPoint position);                                            // draws the element on screen (overwritten by the derived class). "position" is the starting point of the line
        void setAng(float newAngle);                                                // sets the inclination of the line (angles start at 12 o'clock, positive angles clockwise)
        bool setStart(wxPoint startPoint);                                          // sets the origin of the link
        bool setEnd(wxPoint endPoint);                                              // sets the destination of the link. Returns true if a destination dock is found
        void drawTriangle(wxPoint start, wxPoint end, bool ext_call);                              // draw the tip of the arrow that represents the link
        
        void repaint();                                                             // repaint the link
        bool findNearDock(int& nearestNode, int& nearestDock, wxPoint location);    // find a dock near the given location and return a reference to it

        // Target fixing functions
        void engage();          // engage the nodes at both ends of the link
        bool fixate();          // see if the link is attached correctly to a destination state and launch configurator
        void close();           // perform actions prior to deleting the link (todo: use a standard destructor)

        // Event management functions
        PaintBoardEvent* getEvent(){return event_;};                // Get the event that fires the transition
        void setEvent(PaintBoardEvent* value){event_ = value;};      // Set the event that fires the transition

        void addVariable(wxString var);

        // Other methods
        void launchConfigurator();

        // Properties
        wxPoint *start,*end,*prevend;                            // start and end points
        PaintBoardNode *startNode,*endNode;             // start and end nodes (origin and destination states)
        int startDock,endDock;                          // docks engaged in the start and end nodes
        float angle,length;                             // angle of inclination and length of the line
        PaintBoardEvent *event_;                         // associated event
        //wxString *event, *condition;
        PaintBoardElementConfigurator *configurator;    // configurator dialog

};

#endif
