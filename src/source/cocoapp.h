
//  project cocoview
//  version 2
//
//  created 10.11.98 by Andreas Warnke


#pragma once

#include "Application.h"
#include "cocowin.h"


//  cocoapp:
class CocoApp : public BApplication
{
private:
    //  zeiger auf das einzige cocowin:
    CocoWin* theWindow;
    
    //  supoer:
    typedef BApplication super;
    
public:    
    //  Konstruktor
    CocoApp ();

    //  RefsReceived
    void RefsReceived ( BMessage* inMessage );
    
    //  MessageReceived 
    void MessageReceived ( BMessage* inMessage );
};

