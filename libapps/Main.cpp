
/************************************************************************
||
|| DATE:                $Date: 2004/01/08 20:51:46 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/Main.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Main.cpp,v 1.1 2004/01/08 20:51:46 tomcat Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: Main.cpp,v $
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "Application.hpp"
#include <assert.h>

int main ( int argc, char **argv )
{
	// Make sure to 
	// instantiate an Application object
    
	assert ( theApplication ); 
    
	// Init Intrinsics, and enter event loop
	theApplication->initialize ( argc, argv );
    
	theApplication->handleEvents();
	return 0;
}
