
//  project cocoview
//  version 2
//
//  created 10.11.98 by Andreas Warnke

#pragma once

#include "View.h"
#include "Bitmap.h"
#include "FateMaker.h"

class CocoView : public BView
{
private:
    //  Zeiger auf das aktuelle Bild:
    BBitmap* currentImage;

    //  Liste der Dateien (entry_refs):
    BList* theFiles;
    
    //  synchronize-lock für currentImage und currentImagePos
    int ImageLock;
    
    //  synchronize-lock für File-liste:
    int ListLock;
    
    //  loadImage
    BBitmap* loadImage ( char* inFile );
    BBitmap* loadImage ( entry_ref* inFile );
    BBitmap* loadImage ( BFile* inFile );
    BBitmap* loadImage ( BEntry* inFile );
    
    //  changeImage
    void changeImage ();
    
    //  calcSourceRect
    BRect calcSourceRect ( BRect inSource );
    
    //  calcDestRect
    BRect calcDestRect ( BRect inSource, BRect inView );
    
    //  super:
    typedef BView super;
    
    //  Zufallsgenerator:
    FateMaker* theFate;
    
    //  Maximale Verzerrung in Prozenz
    enum { MaxRatioChange = 115 };
    
    //  Pulse-Signale bis zum nächsten Update:
    enum { PulsesTillUpdate = 15 };
    //enum { PulsesTillUpdate = 2 };
    int NextUpdate;    
    
    //  add/remove Entry für die File-Liste:
    void addEntry ( entry_ref* inEntry );
    void removeEntry ( entry_ref* inEntry );
    
    //  Directory-check - prüft, ob das entry_ref ein directory ist
    bool checkDirectory ( entry_ref* inDirectory );
    
    //  Vorschlag für das nächstze Bild:
    int theProposal;
    
public:
    //  Konstruktor
    CocoView ( BRect inRect );
    
    //  destruktor:
    ~CocoView ();
    
    //  Draw-Function
    void Draw ( BRect inUpdate );
    
    //  MessageReceived
    void MessageReceived ( BMessage* inMessage );
    
    //  Pulse
    void Pulse();
    
    //  Message an das Fenster, daß sich das Bild geändert hat:
    enum { NewImage = 0x00C0FFEE };

};