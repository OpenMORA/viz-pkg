/** Painting surface widget
  * This widget implements a surface where the state chart elements are painted
  * and edited.
  * The objects that this widget admits are derived from the class PaintBoardElement.
  *
  * The widget contains a vector with references to all the objects painted on its surface.
  * It is implemented as a state machine, whose states change both from outside and automatically from the actions taken.
  * The states are:
  *      STATE: Used when a click on the surface of the widget will paint a simple state. Started externally.
  *      LINK: A click on the surface of the widget will start a transition from a state to another state. Started externally.
  *      LINKSTOP: This state indicates that a link has an origin point and now its destination is being decided. It is automatically called when a click is done in the LINK state and the link has begun being painted.
  *      NONE: Neutral state. A click with the left button doesn't trigger any action if it happens in an empty area
  * \todo Set policy when a line is painted and matches no dock either at the beginning, the end or both
  */

#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/sizer.h>
#include "tinyxml.h"
#include "PaintBoardElement.h"
#include "PaintBoardNode.h"
#include "PaintBoardLink.h"
//class PaintBoardLink;

/// machine_states: states of the widget
enum machine_states {LINK, LINKSTOP, STATE, MOVE, NONE};

class PaintBoard : public wxPanel
{

public:

    // Constructor
    PaintBoard(wxPanel* _parent);
    //PaintBoard(wxPanel* parent, std::vector<wxString> actions);

    // Destructor
    ~PaintBoard();

    // Event handlers
    void paintEvent(wxPaintEvent & evt);                // repaint event
     void mouseMoved(wxMouseEvent& event);              // mouse moved
     void mouseDown(wxMouseEvent& event);               // left click
     void mouseWheelMoved(wxMouseEvent& event);         // mouse wheel moved
     void mouseReleased(wxMouseEvent& event);           // left button released
     void rightClick(wxMouseEvent& event);              // right click
     void mouseLeftWindow(wxMouseEvent& event);         // mouse got out of window
     void keyPressed(wxKeyEvent& event);                // key pressed
     void keyReleased(wxKeyEvent& event);               // key released
	 void onPopupMenuClick(wxMouseEvent& event);

     // state machine functions
     void setMState(machine_states _state);
     machine_states getMState(){return currentState;};

     // Painting functions
    void drawAState(wxMouseEvent& event);               // paints a state
    PaintBoardNode* drawAStateFromPos(wxPoint *location, bool configured);
    void drawALinkStart(wxMouseEvent& event);           // starts painting a link (if clicked near a free dock) and changes the state to LINKSTOP (in this state the end of the link will follow the mouse cursor until it is clicked again on the destination node)
    PaintBoardLink* drawALinkFromPosStart(wxPoint location, bool configured);
    void drawALinkStop(wxMouseEvent& event);            // finish painting a link (if clicked near a dock) and engage the states
    void drawALinkFromPosStop(wxPoint location);
    void updateActualLink(wxMouseEvent& event);         // updates the image of the link (following the mouse cursor)
    void updateActualLink(wxPoint endPoint);
    void repaint();                                     // repaint

    void moveNodeStop(PaintBoardNode *node, wxPoint position);
    void moveNode();

	void launchCtxMenu();

    // Other functions
    bool findClickedObject(PaintBoardElement*& clickedElement, wxPoint location);   // find whick object was clicked
    PaintBoardElement *clickedElement;                                              // last clicked element
    void configureElement();                                                        // configure the last clicked element
    void deleteElement();                                                           // delete the last clicked element
    std::string SaveXML();                                                // save the information drawn on the board to an XML file
    void setActions(std::vector<wxString> val) {actions = val;};
    void setSaved(bool val) {saved = val;};
    void openStateChart();
    PaintBoardNode* findNodeByName(std::string nodeID);
    void addVariable(wxString var);

    // vector of present elements
    std::vector<PaintBoardElement*> presentElements;

    // available actions and events
    std::vector<wxString> actions;

    DECLARE_EVENT_TABLE()


    private:
    // current state
    machine_states currentState;
    bool saved;
	int linkRefreshCount;
    wxPanel *parent;
    wxString *fileName;
    std::vector<std::string> variables;
};

#endif
