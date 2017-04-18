
//  project cocoview
//  version 2
//
//  created 9.11.98 by Andreas Warnke

#include "View.h"
#include "Bitmap.h"
#include "cocoview.h"

#include <be/interface/Screen.h>
#include <be/interface/Rect.h>
#include <be/storage/File.h>
#include <be/storage/Directory.h>
#include <be/translation/TranslatorRoster.h>
#include <be/translation/BitmapStream.h>

//  Konstruktor
CocoView::CocoView( BRect inRect )
    :BView ( inRect, "cocoview", B_FOLLOW_ALL_SIDES,
        B_PULSE_NEEDED + B_WILL_DRAW + B_FULL_UPDATE_ON_RESIZE ) 
{
    theFiles = new BList ( 256 );
    theFate = new FateMaker ();
    currentImage = NULL;
    NextUpdate = 0;
    //  vorschglag für das nächste bild:
    theProposal = -1;
    //  semaphoren:
    ImageLock = create_sem ( 1, "ImageLock" );
    ListLock = create_sem ( 1, "ListLock" );
    //  color:
    SetViewColor ( BScreen().DesktopColor() );
}

//  Destruktor:
CocoView::~CocoView()
{
    //  memory_:
    delete theFate;
    acquire_sem ( ListLock );
    while ( theFiles -> CountItems() > 0 )
        delete ( theFiles -> RemoveItem ( (long) 0 ) );
    delete ( theFiles );
    release_sem ( ListLock );
    //  semaphoren:
    delete_sem ( ImageLock );
    delete_sem ( ListLock );
}

//  loadImage
BBitmap* CocoView::loadImage ( char* inFile )
{
    //  Erstelle ei8n BFile-Objekt:
    BFile theFile ( inFile, B_READ_ONLY );

    //  loadImage:    
    return loadImage ( &theFile );
}

//  loadImage
BBitmap* CocoView::loadImage ( entry_ref* inFile )
{
    //  Erstelle ei8n BFile-Objekt:
    BFile theFile ( inFile, B_READ_ONLY );

    //  loadImage:    
    return loadImage ( &theFile );
    
    //  theFile - destruktor
}
    
//  loadImage
BBitmap* CocoView::loadImage ( BEntry* inFile )
{
    //  Erstelle ei8n BFile-Objekt:
    BFile theFile ( inFile, B_READ_ONLY );

    //  loadImage:    
    return loadImage ( &theFile );
}
    
//  loadImage
BBitmap* CocoView::loadImage ( BFile* inFile )
{    
    //  Frage nach dem Default-TranslationRoster:
    BTranslatorRoster* theRoster = BTranslatorRoster::Default ();
    
    //  Erstelle einen BitmapStream:
    BBitmapStream* theStream = new BBitmapStream;
    
    //  speichere die neue bitmap lokal:
    BBitmap* theResult = NULL;
    
    //  lies das Bitmap aus dem file, uebersetze es, und gib den Stream zurück:
    if ( theRoster -> Translate( inFile, NULL, NULL, theStream,
            B_TRANSLATOR_BITMAP ) >= B_OK )
        theStream -> DetachBitmap ( &theResult );
        
    //  lösche den Bitmap-Stream:
    delete theStream;
    
    //  fertig:
    return theResult;
}

//  draw:
void CocoView::Draw ( BRect inUpdate )
{
    acquire_sem ( ImageLock );
    if ( currentImage != NULL )
    {
        BRect theSource = calcSourceRect ( currentImage -> Bounds() );
        BRect theDest = calcDestRect ( currentImage -> Bounds(), Bounds() );
        //DrawBitmap ( currentImage, theSource, Bounds() );
        DrawBitmap ( currentImage, theSource, theDest );
    }
    release_sem ( ImageLock );
}

//  calcSourceRect
BRect CocoView::calcSourceRect ( BRect inSource )
{
    return inSource;
}

//  calcDestRect
BRect CocoView::calcDestRect ( BRect inSource, BRect inView )
{
    //  kopien der variablen:
    int OrigWidth = inSource.Width();
    int OrigHeight = inSource.Height();
    int OutWidth = inView.Width();
    int OutHeight = inView.Height();
    
    //  Ausgabe-groesse:
    int ShowWidth = OutWidth;
    int ShowHeight = OutHeight;

    //  konstante x/y ratio:
    if ( OutWidth * OrigHeight > OrigWidth * OutHeight )
        ShowWidth = ( OrigWidth * OutHeight ) / OrigHeight;
    else
        ShowHeight = ( OutWidth * OrigHeight ) / OrigWidth;
        
    //  Verzerrung:
    if ( ShowWidth < OutWidth )
        ShowWidth = ( ShowWidth * MaxRatioChange ) / 100;
    else
        ShowHeight = ( ShowHeight * MaxRatioChange ) / 100;
    if ( ShowWidth > OutWidth )
        ShowWidth = OutWidth;
    if ( ShowHeight > OutHeight )
        ShowHeight = OutHeight;

    //  fertig:    
    int LeftBorder = ( OutWidth - ShowWidth ) / 2;
    int TopBorder = ( OutHeight - ShowHeight ) / 2;
    return BRect ( LeftBorder, TopBorder,
        ShowWidth + LeftBorder, ShowHeight + TopBorder );
}

//  messageReceived:
void CocoView::MessageReceived ( BMessage* inMessage )
{
    if ( ( inMessage -> what == 0x44415441 )
        || ( inMessage -> what == B_REFS_RECEIVED ) )
    {
        //  file referenes sind eingetroffen.
        entry_ref theFileName;
        int Index = 0;
        while ( inMessage -> FindRef ( "refs", Index, &theFileName ) >= B_OK )
        {
            if ( ! checkDirectory ( &theFileName ) )
                addEntry ( new entry_ref ( theFileName ) ); 
            Index++;
        }
        
        //  neues bild laden:
        NextUpdate = 0;
    }
    else super::MessageReceived ( inMessage );
    
}

//  addEntry
void CocoView::addEntry ( entry_ref* inEntry )
{
    acquire_sem ( ListLock );
    //  check, if the entry is already in the list:
    bool Found = false;
    for ( int i = 0; i < theFiles -> CountItems(); i++ )
    {
        entry_ref* ListPos = (entry_ref*) theFiles -> ItemAt ( i );
        if ( (*ListPos) == (*inEntry) )
        {
            // Dieses Bild ist der neue Vorschlag:
            theProposal = i; 
            Found = true; 
        }
    }
    
    if ( ! Found )
    {
        //  einfügen:
        theFiles -> AddItem ( inEntry ); 
        //  Dieses Bild ist der neue Vorschlag:
        theProposal = theFiles -> CountItems() - 1;
    }
    release_sem ( ListLock );
}

//  removeEntry
void CocoView::removeEntry ( entry_ref* inEntry )
{
    //  kopiere inEntry:
    entry_ref* inRefCopy = new entry_ref ( *inEntry ); 

    acquire_sem ( ListLock );
    //  check, if the entry is already in the list:
    for ( int i = theFiles -> CountItems() - 1; i >= 0; i-- )
    {
        entry_ref* ListPos = (entry_ref*) theFiles -> ItemAt ( i );
        if ( (*ListPos) == (*inRefCopy) )
        {
            //  Lösche das item an Position i:
            entry_ref* Found = (entry_ref*) theFiles -> RemoveItem ( i ); 
            if ( Found != NULL )
                delete Found;
            //  korrigiere den Zeiger auf den aktuellen Vorschlag:
            if ( theProposal == i )
                theProposal = -1;
            else if ( theProposal > i )
                theProposal --;
        }
    }
    release_sem ( ListLock );
    
    //  memory:
    delete inRefCopy;
}

//  checkDirectory
bool CocoView::checkDirectory ( entry_ref* inDirectory )
{
    //  directory-test:
    BNode testNode ( inDirectory );
    struct stat testStat;
    if ( testNode.GetStat( &testStat ) < B_NO_ERROR )
        //  error
        return false;
    if ( ! S_ISDIR ( testStat.st_mode ) )
        //  kein dir
        return false;
        
    //  create the directory
    BDirectory testDir ( inDirectory );
    if ( testDir.InitCheck() < B_NO_ERROR )
        //  error
        return false;
        
    //  this is a directory
    BMessage FileReferences ( B_REFS_RECEIVED );
    entry_ref oneFile;
    while ( testDir.GetNextRef ( &oneFile ) >= B_OK )
        FileReferences.AddRef ( "refs", &oneFile );
    BLooper* SendTo = Looper();
    if ( SendTo != NULL )
        SendTo -> PostMessage ( &FileReferences );
        
    //  ready
    return true;
}

//  Pulse:
void CocoView::Pulse()
{
    if ( NextUpdate == 0 )
    {
        NextUpdate = PulsesTillUpdate;
        changeImage();
    }
    else
        NextUpdate --;
}

//  changeImage
void CocoView::changeImage()
{
    //  BEntry des zu ladenden Bildes:
    entry_ref* newImageFile = NULL;

    acquire_sem ( ListLock );
    //  listenlänge == 0?
    if ( theFiles -> CountItems() > 0 )
    {
        //  id des zu ladenden bildes:
        int newImagePos;
        if ( theProposal >= 0 )
        {
            newImagePos = theProposal;
            theProposal = -1;
        }
        else
            newImagePos = theFate -> GetLong ( theFiles -> CountItems() );
        
        //  lies die BEntry aus der liste:
        newImageFile = new entry_ref (
            * (entry_ref*) theFiles -> ItemAt ( newImagePos ) ); 
    }
    release_sem ( ListLock );
    
    //  kein bild?
    if ( newImageFile == NULL )
        return;
        
    //  lade das bild:
    bool Error = false;
    acquire_sem ( ImageLock );
    //  lösche das alte bild:
    if ( currentImage != NULL )
        delete currentImage;
    //  lade das bild:
    currentImage = loadImage ( newImageFile );
    if ( currentImage == NULL )
        //  das war eine ungültige file referenz.
        Error = true;
    release_sem ( ImageLock );
    
    //  fehler bein Laden des bildes:
    if ( Error )
    {
        NextUpdate = 0;
        removeEntry ( newImageFile );
    }
    
    //  Anzeige:
    Invalidate();
    
    //  Message an des Fenster:
    BMessage* theMessage = new BMessage ( NewImage );
    theMessage -> AddString ( "filename", newImageFile -> name );
    BLooper* theWin = Looper();
    if ( theWin != NULL )
        theWin -> PostMessage ( theMessage ); 
    delete theMessage;
    delete newImageFile;
}


