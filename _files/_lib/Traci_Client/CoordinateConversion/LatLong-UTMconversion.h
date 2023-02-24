/*
 * LatLong-UTMconversion.h
 *
 *  Created on: Dec 7, 2016
 *      Author: WA00434454
 */

#ifndef LATLONG_UTMCONVERSION_H_
#define LATLONG_UTMCONVERSION_H_

//LatLong- UTM conversion..h
//definitions for lat/long to UTM and UTM to lat/lng conversions
#include <string.h>

#ifndef LATLONGCONV
#define LATLONGCONV

void LLtoUTM(int ReferenceEllipsoid, const double Lat, const double Long,
			 double &UTMNorthing, double &UTMEasting, char* UTMZone);
void UTMtoLL(int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting, const char* UTMZone,
			  double& Lat,  double& Long );
char UTMLetterDesignator(double Lat);
void LLtoSwissGrid(const double Lat, const double Long,
			 double &SwissNorthing, double &SwissEasting);
void SwissGridtoLL(const double SwissNorthing, const double SwissEasting,
					double& Lat, double& Long);

void Convert_XY_to_LatLong(double,double,double&,double&);


class Ellipsoid
{
public:
	Ellipsoid(){};
	Ellipsoid(int Id, char* name, double radius, double ecc)
	{
		id = Id; ellipsoidName = name;
		EquatorialRadius = radius; eccentricitySquared = ecc;
	}

	int id;
	char* ellipsoidName;
	double EquatorialRadius;
	double eccentricitySquared;

};



#endif




#endif /* LATLONG_UTMCONVERSION_H_ */
