/*
 * $Id: gpsNMEA.c,v 1.4 2010/06/14 19:06:37 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: gpsNMEA.c,v $
 * Revision 1.4  2010/06/14 19:06:37  clivewebster
 * Add copyright license info
 *
 * Revision 1.3  2010/04/25 03:25:15  clivewebster
 * Reduce rounding errors when reading longitude latitude from GPS message
 *
 * Revision 1.2  2010/02/17 23:45:18  clivewebster
 * Add flags for each returned value
 *
 * Revision 1.1  2010/01/30 21:20:40  clivewebster
 * Moved to sensors foldere
 *
 * Revision 1.5  2010/01/17 02:40:52  clivewebster
 * *** empty log message ***
 *
 * Revision 1.4  2009/11/02 18:57:28  clivewebster
 * Added revision log
 *
 * ===========
 *
 * Copyright (C) 2010 Clive Webster (webbot@webbot.org.uk)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * gpsNMEA.c
 *
 *  Created on: 07-May-2009
 *      Author: Clive Webster
 */
#include "gpsNMEA.h"
#include <stdlib.h>
#include <math.h>

//#define DEBUG
#ifdef DEBUG
#include "../../../rprintf.h"
#endif



static void _gpsNMEA_init(GPS_NMEA* gps){
	// Allocate a buffer for the msg
	if(gps->msg==null){
		gps->msg = malloc(MAX_NMEA_MSG);
		_uartInit(gps->uart, gps->baudRate);
		__uartFlushReceiveBuffer(gps->uart);
		#ifdef DEBUG
		rprintf("Start monitoring GPS\n");
		#endif
	}
}



// Fwd refs
static void _gps_process_gprmc(GPS_NMEA* gps,int inx);
static void _gps_process_gpgsa(GPS_NMEA* gps,int inx);
static void _gps_process_gpgsv(GPS_NMEA* gps,int inx);
static void _gps_process_gpgga(GPS_NMEA* gps,int inx);

boolean gpsNMEAprocess(GPS_NMEA* gps){
	_gpsNMEA_init(gps);

	// Nothing has been changed
	gps->info.changes.all = 0;

	// Get working data
	int msgIndex = gps->msgIndex;
	boolean error = gps->error;
	char* msg = gps->msg;

	int i;
	while(i = __uartGetByte(gps->uart), i!=-1){
		char c = (char)i;

		// If its a $ then its the start of a msg
		if(c=='$'){
			msgIndex = 0;	// reset the msg buffer
			error=FALSE;	// clear error state
		}

		// Store the character
		if(msgIndex < MAX_NMEA_MSG-1){
			msg[msgIndex++] = c;	// store character
			msg[msgIndex] = 0;		// null terminate string
		}else{
			error=TRUE;				// the message is too long
		}

		// Test for end of line
		if(c=='\n' && error==FALSE){
			// Have got a line of length msgIndex

			// Test for minimum size of '$GP...<,>.....*xx<cr><lf>'
			if(msgIndex >= 8){
				if(msg[0]=='$' && msg[1]=='G' && msg[2]=='P' && msg[6]==',' && msg[msgIndex-1]=='\n' && msg[msgIndex-2]=='\r' && msg[msgIndex-5]=='*'){
					// Looks like a valid message
					msg[msgIndex-2]=0;	// chop off cr/lf
					msgIndex-=2;

					// Calculate the checksum
					uint8_t myChecksum=0;
					for(i=1; ;){
						c = msg[i++];
						if(c=='*'){
							break;
						}
						myChecksum ^= c;
					}

					// Remove checksum
					msg[i-1]=0;

					// Get received checksum
					uint8_t rxChecksum=0;
					c = msg[i++];
					if(c>='0' && c<='9'){
						rxChecksum += c-'0';
					}else{
						rxChecksum += c-'A'+10;
					}
					rxChecksum *= 16;
					c = msg[i++];
					if(c>='0' && c<='9'){
						rxChecksum += c-'0';
					}else{
						rxChecksum += c-'A'+10;
					}

					// If checksums agree then process the msg
					if(rxChecksum==myChecksum){

						// Process the msg
						error=TRUE;
						if(msg[3]=='R' && msg[4]=='M' && msg[5]=='C' ){
							// $GPRMC msg
							error=FALSE;
							_gps_process_gprmc(gps,7);
						}else if(msg[3]=='G'){
							if(msg[4]=='S'){
								if(msg[5]=='A' ){
									// $GPGSA msg
									error=FALSE;
									_gps_process_gpgsa(gps,7);
								}else if(msg[5]=='V' ){
									// $GPGSV msg
									error=FALSE;
									_gps_process_gpgsv(gps,7);
								}
							}else if(msg[4]=='G' && msg[5]=='A' ){
								// $GPGGA msg
								error=FALSE;
								_gps_process_gpgga(gps,7);
							}
						}

						#ifdef DEBUG
						if(error){
							rprintfStr("Unknown msg:");
						}
						#endif
					}else{
						#ifdef DEBUG
						rprintfStr("Bad checksum:");
						#endif
					}

					#ifdef DEBUG
					// Dump out the message
					rprintfStr(msg);
					rprintfCRLF();
					#endif

				}
			}
		}
	}
	// Set working data
	gps->msgIndex = msgIndex;
	gps->error = error;

	return (gps->info.changes.all == 0) ? FALSE : TRUE;
}

static int _gps_skip(char* msg,int inx){
	while(msg[inx++] != ',');
	return inx;
}

static double _gps_getRadians(char* msg,int inx){
	/*
	 * All this multiplication and division is causing rounding errors
	double rtn = strtod(&msg[inx], null);
	// convert to pure degrees [dd.dddd] format
	double degrees;
	double minutesfrac = modf(rtn/100.0, &degrees);
	rtn = degrees + (minutesfrac*100.0)/60.0;
	*/

	int16_t degrees=atoi(&msg[inx])/100;
	while(msg[inx]!='.') inx++;
	double rtn = degrees + strtod(&msg[inx-2],null)/ 60.0;

	// convert to radians
	rtn *= (M_PI/180);
	return rtn;
}

static int _gps_getLongitude(char* msg, int inx, double * rtn){
	double longitude = _gps_getRadians(msg,inx);
	inx = _gps_skip(msg,inx);

	// correct latitute for E/W
	if(msg[inx++] == 'W'){
		longitude = -longitude;
	}
	inx = _gps_skip(msg,inx);
	*rtn = longitude;
	return inx;
}

static int _gps_getLatitude(char* msg,int inx, double * rtn){
	double latitude = _gps_getRadians(msg,inx);
	inx = _gps_skip(msg,inx);

	// get N/S indicator
	// correct latitute for N/S
	if(msg[inx++] == 'S'){
		latitude = -latitude;
	}
	inx = _gps_skip(msg,inx);
	*rtn = latitude;
	return inx;
}

/*
 RMC - NMEA has its own version of essential gps pvt (position, velocity, time) data. It is called RMC, The Recommended Minimum, which will look similar to:

$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A

Where:
     RMC          Recommended Minimum sentence C
     123519       Fix taken at 12:35:19 UTC
     A            Status A=active or V=Void.
     4807.038,N   Latitude 48 deg 07.038' N
     01131.000,E  Longitude 11 deg 31.000' E
     022.4        Speed over the ground in knots
     084.4        Track angle in degrees True
     230394       Date - 23rd of March 1994
     003.1,W      Magnetic Variation
 */
static void _gps_process_gprmc(GPS_NMEA* gps,int inx){
	double fixTime;
	double latitude;
	double longitude;
	double speed;
	double track;
	char* msg = gps->msg;

	// get UTC time [hhmmss.sss]
	fixTime = strtod(&msg[inx], null);
	inx = _gps_skip(msg,inx);

	// ignore if not active
	if(msg[inx++]!='A')
		return;
	inx = _gps_skip(msg,inx);

	// get latitude  [ddmm.mmmmm]
	inx = _gps_getLatitude(msg,inx,&latitude);

	// get longitude [ddmm.mmmmm]
	inx = _gps_getLongitude(msg,inx,&longitude);

	//Get speed in knots
	speed = strtod(&msg[inx], null);
	inx = _gps_skip(msg,inx);

	//Get track angle in degrees
	track = strtod(&msg[inx], null);

	gps->info.valid = TRUE;
	gps->info.fixTime = fixTime;
	gps->info.longitude = longitude;
	gps->info.latitude = latitude;
	gps->info.speed = speed;
	gps->info.track = track;

	gps->info.changes.values.fixTime =
	gps->info.changes.values.latitude =
	gps->info.changes.values.longitude =
	gps->info.changes.values.speed =
	gps->info.changes.values.track = TRUE;

}

/*
GSA - GPS DOP and active satellites. This sentence provides details on the nature of the fix. It includes the numbers of the satellites being used in the current solution and the DOP. DOP (dilution of precision) is an indication of the effect of satellite geometry on the accuracy of the fix. It is a unitless number where smaller is better. For 3D fixes using 4 satellites a 1.0 would be considered to be a perfect number, however for overdetermined solutions it is possible to see numbers below 1.0.

There are differences in the way the PRN's are presented which can effect the ability of some programs
to display this data. For example, in the example shown below there are 5 satellites in the solution
and the null fields are scattered indicating that the almanac would show satellites in the null
positions that are not being used as part of this solution. Other receivers might output all of
the satellites used at the beginning of the sentence with the null field all stacked up at the end.
This difference accounts for some satellite display programs not always being able to display the
satellites being tracked. Some units may show all satellites that have ephemeris data without regard
to their use as part of the solution but this is non-standard.

  $GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39

Where:
     GSA      Satellite status
     A        Auto selection of 2D or 3D fix (M = manual)
     3        3D fix - values include: 1 = no fix
                                       2 = 2D fix
                                       3 = 3D fix
     04,05... PRNs of satellites used for fix (space for 12)
     2.5      PDOP (dilution of precision)
     1.3      Horizontal dilution of precision (HDOP)
     2.1      Vertical dilution of precision (VDOP)

 */
static void _gps_process_gpgsa(GPS_NMEA* gps, int inx){
}


/*
GSV - Satellites in View shows data about the satellites that the unit might be able to find based on
its viewing mask and almanac data. It also shows current ability to track this data.
Note that one GSV sentence only can provide data for up to 4 satellites and thus there may need to be
3 sentences for the full information. It is reasonable for the GSV sentence to contain more satellites
than GGA might indicate since GSV may include satellites that are not used as part of the solution.
It is not a requirment that the GSV sentences all appear in sequence. To avoid overloading the data
bandwidth some receivers may place the various sentences in totally different samples since each
sentence identifies which one it is.

The field called SNR (Signal to Noise Ratio) in the NMEA standard is often referred to as signal strength.
SNR is an indirect but more useful value that raw signal strength. It can range from 0 to 99 and has units
of dB according to the NMEA standard, but the various manufacturers send different ranges of numbers with
different starting numbers so the values themselves cannot necessarily be used to evaluate different units.
The range of working values in a given gps will usually show a difference of about 25 to 35 between the
lowest and highest values, however 0 is a special case and may be shown on satellites that are in view but
not being tracked.

  $GPGSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75

Where:
      GSV          Satellites in view
      2            Number of sentences for full data
      1            sentence 1 of 2
      08           Number of satellites in view

      01           Satellite PRN number
      40           Elevation, degrees
      083          Azimuth, degrees
      46           SNR - higher is better
           for up to 4 satellites per sentence

 */
static void _gps_process_gpgsv(GPS_NMEA* gps, int inx){
}


/*
GGA - essential fix data which provide 3D location and accuracy data.

 $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47

Where:
     GGA          Global Positioning System Fix Data
     123519       Fix taken at 12:35:19 UTC
     4807.038,N   Latitude 48 deg 07.038' N
     01131.000,E  Longitude 11 deg 31.000' E
     1            Fix quality: 0 = invalid
                               1 = GPS fix (SPS)
                               2 = DGPS fix
                               3 = PPS fix
			       4 = Real Time Kinematic
			       5 = Float RTK
                               6 = estimated (dead reckoning) (2.3 feature)
			       7 = Manual input mode
			       8 = Simulation mode
     08           Number of satellites being tracked
     0.9          Horizontal dilution of position
     545.4,M      Altitude, Meters, above mean sea level
     46.9,M       Height of geoid (mean sea level) above WGS84
                      ellipsoid
     (empty field) time in seconds since last DGPS update
     (empty field) DGPS station ID number
     *47          the checksum data, always begins with *

If the height of geoid is missing then the altitude should be suspect. Some non-standard implementations report altitude with respect to the ellipsoid rather than geoid altitude. Some units do not report negative altitudes at all. This is the only sentence that reports altitude.
 */

static void _gps_process_gpgga(GPS_NMEA* gps, int inx){

	double fixTime;
	double latitude;
	double longitude;
	int satellites;
	double altitude;
	char* msg = gps->msg;

	// get UTC time [hhmmss.sss]
	fixTime = strtod(&msg[inx], null);
	inx = _gps_skip(msg,inx);

	// If no longitude or latitude then reject it
	if(msg[inx]==',' && msg[inx+1]==',')
		return;

	// get latitude  [ddmm.mmmmm]
	inx = _gps_getLatitude(msg,inx,&latitude);

	// get longitude [ddmm.mmmmm]
	inx = _gps_getLongitude(msg,inx,&longitude);

	// Test if it is valid
	if( (msg[inx] == '0') || (msg[inx] == ',') )
		return;
	inx = _gps_skip(msg,inx);

	// get number of satellites used in GPS solution
	satellites = atoi(&msg[inx]);
	inx = _gps_skip(msg,inx);

	// skip dilution
	inx = _gps_skip(msg,inx);

	// get altitude (in meters)
	altitude = strtod(&msg[inx], null);

	gps->info.valid = TRUE;
	gps->info.fixTime = fixTime;
	gps->info.latitude = latitude;
	gps->info.longitude = longitude;
	gps->info.numSatellites = satellites;
	gps->info.altitude = altitude;

	gps->info.changes.values.fixTime =
	gps->info.changes.values.latitude =
	gps->info.changes.values.longitude =
	gps->info.changes.values.numSatellites =
	gps->info.changes.values.altitude = TRUE;
}
