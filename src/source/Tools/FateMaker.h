// // // // // // // // // // // // //
//
//	class FateMaker
//
//	created Oct. 6th, 1997
//	last mod Jan. 2 nd, 1998
//	Version 1.3.1
//	Author: Andreas Warnke: awarnke@cip.informatik.uni-wuerzburg.de
//	Freeware - just use & modify it!
//
//	description:
//	This class produces random numbers
//
//	needed libraries:
//	libroot.so
//



#ifndef FateMaker_h_
#define FateMaker_h_



class FateMaker
{
private:
	//	Register
	//	it's not static, because you might use this
	//	object to encode data instead of getting random
	//	numbers. In this case, no other FateMaker-object
	//	should change this value. 
	int32 theRegister;

	//	1st primitive polynom of degree 32:
	//	1 0000 0000 0000 0000 0000 0000 1010 1111
	//	#define FateMaker_Polynom_ 0x000000af
	
	//	another primitive polynom of degree 32:
	//	1 1010 0110 0100 1101 0110 1100 0001 0001
	#define FateMaker_Polynom_ 0xa64d6c11

public:
	//	Constructor, initialises theRegister
	FateMaker ( void );

	//	Destructor, does nothing
	virtual ~FateMaker ( void );
	
	//	GetLong, returns a random longint number
	//	in the Range [0,MaxValue-1]. Default is 0x100000000:
	unsigned long GetLong ( unsigned long MaxValue = 0 );	
	
	//	Register, to get the current Register status.
	//	This might be important, if you encode data.
	int32 Register ( void );
	
	//	SetRegister, to set the Register to a specific value
	//	This is important, if you encode data.
	//	NEVER SET theRegister TO ZERO!
	//	Otherwise, GetLong() always returns 0.
	void SetRegister ( int32 newValue );
};



#endif



//
//	The End :-)
//
// // // // // // // // // // // // //