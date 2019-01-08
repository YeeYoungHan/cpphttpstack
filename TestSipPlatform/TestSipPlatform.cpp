#include "TestSipPlatform.h"

int main( int argc, char * argv[] )
{
	if( TestStringUtility() == false )
	{
		printf( "ERROR\n" );
	}
	else
	{
		printf( "OK\n" );
	}

	return 0;
}
