#include "PaintBoardElement.h"

PaintBoardElement::PaintBoardElement(wxPanel *targetBoard)
{
    // Constructor
    board = targetBoard;    // sets the parent board
    firstConfig = true;
}

void PaintBoardElement::setConfigured()
{
    firstConfig = false;
}
