#ifndef BASICCOMPONENT_H
#define BASICCOMPONENT_H
#include <iostream.h>
//#include <X11/X.h>
#include <X11/Intrinsic.h>

class BasicComponent {
    
	protected:
		char    *_name;
    
		// Protected constructor to prevent instantiation
    
		BasicComponent ( const char * );   

		void installDestroyHandler(); // Easy hook for derived classes
    
	public:
    
		virtual ~BasicComponent();
		virtual void manage();  
		virtual void unmanage();
		virtual const char *const className() { return "BasicComponent";}
};
#endif

