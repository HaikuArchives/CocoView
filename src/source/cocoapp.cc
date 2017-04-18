
//  project cocoview
//  version 2
//
//  created 10.11.98 by Andreas Warnke


#include "Application.h"
#include "cocoapp.h"
#include "cocowin.h"



//  Konstruktor
CocoApp::CocoApp()
    : BApplication ("application/x-vnd.awarnke.cocoview")
{
    theWindow = new CocoWin ();
};

//  RefsReceived
void CocoApp::RefsReceived ( BMessage* inMessage )
{
    //  sende Message an Fenster:
    theWindow -> PostMessage ( inMessage, NULL );
}

//  MessageReceived
void CocoApp::MessageReceived ( BMessage* inMessage )
{
    //  close-event:
    if ( inMessage -> what == CocoWin::CloseMsg )
        Quit ();
    else
        super::MessageReceived ( inMessage );
}

