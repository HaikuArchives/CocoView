
//  project cocoview
//  version 2
//
//  created 9.11.98 by Andreas Warnke

#include "Window.h"
#include "cocowin.h"
#include "cocoview.h"
#include <be/app/Application.h>

//  Konstruktor
CocoWin::CocoWin()
:BWindow ( 
      BRect ( 10, 30, 210, 330 ),
      "cocoview",
      B_TITLED_WINDOW,
      B_PULSE_NEEDED )
{
    //  pulserate: 1 per second
    SetPulseRate ( 1000000 );
    
    //  cocoview:
    theView = new CocoView ( BRect ( 0, 0, 200, 300 ) );
    AddChild ( theView );
    
    //  User-Zoom:
    /*
    UserLeftTop.SetTo ( 0, 0 );
    UserWidth = 0;
    UserHeight = 0;
    */
    
    //  fertig:
    Show();
}

//  CloseBox
bool CocoWin::QuitRequested()
{
    be_app -> PostMessage ( CloseMsg );
    return true;
}

//  DispatchMessage
void CocoWin::DispatchMessage ( BMessage* inMessage, BHandler* inHandler )
{
    if ( inMessage -> what == B_REFS_RECEIVED )
        super::DispatchMessage ( inMessage, theView );
    else if ( inMessage -> what == CocoView::NewImage )
        SetTitle ( inMessage -> FindString ( "filename" ) );
    else
        super::DispatchMessage ( inMessage, inHandler );
}

//  Zoom:
void CocoWin::Zoom ( BPoint inLeftTop, float inWidth, float inHeight )
{
    /*
    BRect Win = BScreen::Frame();
    if ( ( BRect.LeftTop() == inLeftTop ) && ( BRect.Widht() == inWidht )
    MoveTo ( inLeftTop );
    ResizeTo ( inWidth, inHeight );
    */
    super::Zoom ( inLeftTop, inWidth, inHeight );
}

