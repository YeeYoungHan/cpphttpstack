#include "XmlElement.h"

int main( int argc, char * argv[] )
{
	CXmlElement clsElement;
	std::string strXml;

	clsElement.SetName( "Class" );
	clsElement.InsertElementData( "Arg1", "Value1" );

	clsElement.ToString( strXml );

	return 0;
}
