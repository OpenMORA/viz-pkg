
#ifndef PAINTBOARDNODE_H
#define PAINTBOARDNODE_H


#include "PaintBoardElement.h"
#include "PaintBoardElementConfigurator.h"
#include "PaintBoardAction.h"
#include <vector>
#include <string>
#include <sstream>
#include "tinyxml.h"


/// dock_element: contains the information of a dock
struct dock_element
{
    wxPoint* location;              // location of the dock in the board.
    bool IsFree;                    // true if the dock is not engaged
    bool incoming;                  // true if the  linked node is previous to the current node (i.e: the arrow points towards the current state)
    PaintBoardElement* node;        // pointer to the linked node
    PaintBoardElement* link;        // pointer to the link that uses the dock
    wxString event;                 // event that launches the transition
    wxString condition;             // condition of the transition
    wxString conditionType;         // condition type
    wxString variable;              // variable name
};

/// Constants for the Node object
#define DOCKS 36                    // Defines the number of docks that the nodes will have. The docks are the points that link several states.
#define RADIUS 50                   // Radius of the circle representing a state
#define RADIUS_DOCK 2               // Radius of the circle representing a dock when highlighted
#define CLICK_SENSITIVITY 10        // Distance to the border of the circle that is still considered as part of the node (in terms of mouse clicks)


class PaintBoardNode : public PaintBoardElement
{
    /// The PaintBoardNode class represents a state in the State Chart. It is derived from the PaintBoardElement class, that
    /// describes generally any object painted on the widget. This class includes methods for painting the node element
    /// and for linking one state to others.

    public:
        // Constructor
        PaintBoardNode(wxPanel* targetBoard);
        // Destructor
        ~PaintBoardNode();

        // painting methods, used for representing the object on screen
        void drawForm(wxPoint position);    // draws the element on screen
        void showDock(int dockID);  // shows (highlights) the dock given by dockID
        void showDocks();          // shows (highlights) all the docks
        void hideDocks();          // hides the docks
        void repaint();                     // repaints the node with the current configuration of hidden/shown elements

        // linking methods, used for linking elements through their docking points
        wxPoint getDock(int dockID);        // returns the coordinates of the dock given by dockID
        void engageDock(int localDock, int remoteDock, PaintBoardNode* target, PaintBoardElement* interlink, bool incoming, PaintBoardEvent *event);      // links the dock given by localDock with the remoteDock of the targer node
        void disengageDock(int dockID);     // frees a dock
        bool dockIsFree(int dockID);        // see if the dock is engaged or free

        // Getter methods, used for obtaining information from the element
        std::vector<dock_element> obtainConnections(){ return docks;};          // returns the list of linked nodes
        wxString getName(){return name;};                                       // returns the name of the state
        bool isFinal(){return final;};                                           // returns true if the element is final
        bool isInitial(){return initial;};                                      // returns true if the element is initial
        std::vector<PaintBoardAction*> getActions(){return actionList;};        // returns the list of actions associated with the state
       // std::vector<PaintBoardEvent*> getEvents();          // returns the list of events associated with the state
       // std::vector<wxString> getEventConditions();
        wxPoint getPosition(){return actualPosition;};                          // return the position of the node on the PaintBoard


        // property setting methods, used to change the information of the element
        void setName(wxString newName){name=newName;};                                           // sets the name of the state
        void setFinal(bool status){final = status;};                                            // sets the status of the state as a final state
        void setInitial(bool status){initial = status;};                                        // sets the status of the state as an initial state
        void setActionList(std::vector<PaintBoardAction*> actions){actionList = actions;};      // set the list of actions associated to the state
        //void setEvents(std::vector<PaintBoardEvent*> evtList);
        void addAction(wxString actionT, wxString ID, wxString actionP, int rep);               // add an action to the list of associated actions of the state (unused, kept for possible future needs)
        //void addEvent(PaintBoardEvent *evt);

        // other methods
        void launchConfigurator();      // launch the configurator dialog
        void close();                   // perform actions prior to deleting the node (todo: use a standard destructor)
        TiXmlElement* SCXMLEntry();     // generate the SCXML entry for the state

        // conections
        std::vector<dock_element> docks;                // list of docks

    private:
        // positioning
        wxPoint actualPosition;                         // absolute position of the state in the widget


        // state characteristics
        wxString name;                                  // name
        bool final;	                                    // status as final state
        bool showDocksBool;
        bool initial;                                   // status as initial state
        //bool firstConfig;                               // true if the element has not been configured yet
        std::vector<PaintBoardAction*> actionList;      // list of actions associated to the state
//        int repetitions;

//        std::vector<PaintBoardEvent*> watchedEvents;    //events represented as wxString. Might be upgraded to objects at some point in the development
        //std::vector<wxString> watchedEventConditions;
        // onEntry and onExit actions temporarily disabled
      //  std::vector<PaintBoardAction*> onEntry;	//!< Actions on entry.
      //  std::vector<PaintBoardAction*> onExit;	//!< Actions on exit.

      PaintBoardElementConfigurator *configurator;      // link to the configurator dialog object
};


#endif // PAINTBOARDNODE_H
