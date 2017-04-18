// // // // // // // // // // // // //
//
//	class FateMaker
//
//	created Oct. 6th, 1997
//	last mod Jan. 2nd, 1998
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


#include <be/kernel/OS.h>
#include "FateMaker.h"



// // // // // // // // // // // // //
//
//	Constructor, initialises theRegister
//

FateMaker :: FateMaker ( void )
{
	//	set theRegister to the lower 32 bit of the real_time_clock:
	theRegister = real_time_clock_usecs ();
	
	//	theRegister must not be zero!
	if ( theRegister == 0x00000000 )
		theRegister = 0x00000001;
		
	//	Avoid similar Start-Sequences
	//	when multiple FateMaker-Objects are constructed
	//	at nearly the same Time:
	GetLong();
};



// // // // // // // // // // // // //
//
//	Destructor, does nothing
//

FateMaker :: ~FateMaker ( void )
{
};
	

	
// // // // // // // // // // // // //
//	
//	GetLong, returns a random longint number
//	in the Range [0,MaxValue-1]. Default ist 0x100000000:
//

unsigned long FateMaker :: GetLong ( unsigned long MaxValue )
{
	//	define result:
	unsigned long result;
	
	//	MaxValue --; New Range: [0,MaxValue]
	MaxValue--;

	//	repeat until ( result <= MaxValue ):
	do
	{
		//	Set Result to zero:
		result = 0;
		
		//	copy MaxValue
		unsigned long GetMoreBits = MaxValue;
		
		//	get the bits from the register
		while ( GetMoreBits != 0 )
		{
			//	Shift left of the result:
			result = result <<	1;
			
			//	Add the least significant bit of the Register:
			result = result | ( theRegister & 0x00000001 );
			
			//	Is the most significant bit of theRegister set?
			if ( theRegister < 0 )
				//	Shift theRegiser and XOR the primitive polynom:
				theRegister = ( theRegister << 1 ) ^ FateMaker_Polynom_;
			else
				//	Shift the Register:
				theRegister = theRegister << 1;
				
			//	next Bit:
			GetMoreBits = GetMoreBits >> 1;
		};
	}
	while ( result > MaxValue ); 
	
	//	ready:
	return result;
};

	
	
// // // // // // // // // // // // //
//
//	Register, to get the current Register status.
//	This might be important, if you encode data.
//

int32 FateMaker :: Register ( void )
{
	return theRegister;
};



// // // // // // // // // // // // //
//
//	SetRegister, to set the Register to a specific value
//	This is important, if you encode data.
//	NEVER SET theRegister TO ZERO!
//	Otherwise, GetLong() always returns 0.

void FateMaker :: SetRegister ( int32 newValue )
{
	theRegister = newValue;
};



//
//	The End :-)
//
// // // // // // // // // // // // //