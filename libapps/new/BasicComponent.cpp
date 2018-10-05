#include "BasicComponent.hpp"
#include <assert.h>
#include <stdio.h>

BasicComponent::BasicComponent ( const char *name )
{
	assert ( name != NULL );  
	_name = strdup ( name );
}

BasicComponent::~BasicComponent()
{
	delete [] _name;
}

void BasicComponent::manage()
{
}

void BasicComponent::unmanage()
{
}

void
BasicComponent::installDestroyHandler()
{
		
}
