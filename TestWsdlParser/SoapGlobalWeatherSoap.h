#ifndef _C_SOAP_GLOBAL_WEATHER_SOAP_H_
#define _C_SOAP_GLOBAL_WEATHER_SOAP_H_

#include <string>

class CSoapGlobalWeatherSoap
{
public:
	CSoapGlobalWeatherSoap();
	~CSoapGlobalWeatherSoap();

	bool GetCitiesByCountry( std::string & strCountryName, std::string & strGetCitiesByCountryResultOut );
	bool GetWeather( std::string & strCityName, std::string & strCountryName, std::string & strGetWeatherResultOut );
};

#endif
