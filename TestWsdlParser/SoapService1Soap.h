#ifndef _C_SOAP_SERVICE1_SOAP_H_
#define _C_SOAP_SERVICE1_SOAP_H_

#include <string>

class CSoapService1Soap
{
public:
	CSoapService1Soap();
	~CSoapService1Soap();

	bool HelloWorld( std::string & strHelloWorldResult );
};

#endif
