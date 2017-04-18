
//  project cocoview
//  version 2
//
//  created 9.11.98 by Andreas Warnke

#pragma once

#include "Window.h"
#include "cocoview.h"

class CocoWin : public BWindow
{
private:
    //  Zeiger auf die cocoview:
    CocoView* theView;
    
    //  super:
    typedef BWindow super;
    
    //  Window-Position:
    /*
    BPoint UserLeftTop;
    float UserWidth;
    float UserHeight;
    bool Zoomed;
    */
    
public:
    //  Konstruktor
    CocoWin();
    
	//	CloseBox:
	virtual bool QuitRequested ();
	
	//  DispatchMessage
	void DispatchMessage ( BMessage* inMessage, BHandler* inHandler );
	
	//  close-message:
	enum { CloseMsg = 0x01C0FFEE };
	
	//  Zoom:
	void Zoom ( BPoint inLeftTop, float inWidth, float inHeight );
};
