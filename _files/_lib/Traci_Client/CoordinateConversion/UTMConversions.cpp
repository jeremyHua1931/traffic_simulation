/*
 * UTMConversions.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: WA00434454
 */
//UTM Conversion.cpp- test program for lat/long to UTM and UTM to lat/long conversions
#include <iostream>
#include <iomanip>
#include "LatLong-UTMconversion.h"
using namespace std;




//int main()
//{
//	double Lat =49.576346;// 47.37816667;
//	double Long=11.012396;//8.23250000;
//	double UTMNorthing;
//	double UTMEasting;
//	double SwissNorthing;
//	double SwissEasting;
//	char UTMZone[4];
//	int RefEllipsoid = 23;//WGS-84. See list with file "LatLong- UTM conversion.cpp" for id numbers
//
//	cout << "Starting position(Lat, Long):  " << Lat << "   " << Long <<endl;
//
//	LLtoUTM(RefEllipsoid, Lat, Long, UTMNorthing, UTMEasting, UTMZone);
//	cout << setiosflags(ios::showpoint | ios::fixed) << setprecision(5);
//	cout << "Calculated UTM position(Northing, Easting, Zone):  ";
//	cout << UTMNorthing << "   " << UTMEasting;
//	cout << "   " << UTMZone <<endl;
//
//	UTMtoLL(RefEllipsoid, UTMNorthing, UTMEasting, UTMZone, Lat, Long);
//	cout << "Calculated Lat, Long position(Lat, Long):  " << Lat << "   " << Long << endl <<endl;
//
//	LLtoSwissGrid(Lat, Long, SwissNorthing, SwissEasting);
//	cout << setiosflags(ios::showpoint | ios::fixed) << setprecision(5);
//	cout << "Calculated Swiss Grid position(Northing, Easting):  ";
//	cout << SwissNorthing << "   " << SwissEasting << endl;
//
//	return 0;
//}

	//wasim::test

void Convert_XY_to_LatLong(double X,double Y,double &Lat,double &Long)
{
	//5493473.95059   645481.17968   32U
//		double UTMNorthing = 5493473.95059;
//		double UTMEasting = 645481.17968;

	double UTMEasting = X;
	double UTMNorthing = Y;


			char UTMZone[4] = {'3','2','U'};

			int RefEllipsoid = 23;

		UTMtoLL(RefEllipsoid, UTMNorthing, UTMEasting, UTMZone, Lat, Long);
		//cout << "Calculated Lat, Long position(Lat, Long):  " << Lat << "   " << Long << endl <<endl;


}



/* N 47.38195° E 8.54879°  (Swiss Grid: 683.748 248.342)
  N 47°12.625' / E 7° 27.103'= N 47.21041667 E 7.45171667(Swiss Grid = 600920/228685)
  N 47°22.690' / E 8° 13.950'= N 47.37816667 E 8.23250000 (Swiss Grid = 659879/247637)
*/



