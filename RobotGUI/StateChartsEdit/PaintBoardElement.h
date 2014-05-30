/** Painting surface widget generic element
  * This object represents any generic element that may be painted on the board
  */

#ifndef PAINTBOARDELEMENT_H
#define PAINTBOARDELEMENT_H

#define LINE_WIDTH 3

#include<wx/wx.h>
//#include "PaintBoardElementConfigurator.h"

class PaintBoardElement
{
    /// The PaintBoardElement class represents any object that has a representation in the state chart. Any such object must inherit from this class
    /// \todo eliminate the close() function and change it to a normal destructor
    public:
        // Constructor
        PaintBoardElement(wxPanel *targetBoard);    // targetBoard is a pointer to the PaintBoard Object that the element is going to be painted on

        virtual ~PaintBoardElement() {}  // Dtor is required in polymorphic classes!!

        // painting methods, used for representing the object on screen
        virtual void drawForm(wxCoord position){};    // draws the element on screen (overwritten by the derived class)
        virtual void repaint(){};                     // repaints the element on screen (overwritten by the derived class)
		virtual void launchConfigurator(){};          // launches the configuration window of the element (overwritten by the derived class)
        virtual void close(){};                       // performs the actions prior to the closing of the object
        wxPanel* board;                             // pointer to the parent board
        void setConfigured();                         // sets the element as configured, so the configurator doesn't show up

        bool firstConfig;                               // true if the element has not been configured yet
};

#endif // PAINTBOARDELEMENT_H
