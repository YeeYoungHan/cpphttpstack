#include "TestSipPlatform.h"
#include "StringUtility.h"

bool TestStringUtility()
{
	std::string strInput, strOutput;

	strInput = "%23%3B%2F%3F%3A%40%26%3D%2B%24%2C-_.!~*'()";

	DecodeUri( strInput, strOutput );

	if( strcmp( strOutput.c_str(), "#;/?:@&=+$,-_.!~*'()" ) )
	{
		printf( "output(%s) != \"#;/?:@&=+$,-_.!~*'()\"\n" );
		return false;
	}

	return true;
}
