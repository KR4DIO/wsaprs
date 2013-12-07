#define _XOPEN_SOURCE

#include <stdio.h>
#include <time.h>

#include "dfile.h"
#include "header.h"
#include "wrecord.h"
#include "wsaprs.h"

#define MS_TO_KNOTS(m) (m * 1.9438444924574)
#define C_TO_F(c) ((c * 9.0/5.0) + 32)

int aprs_wx(char* message, int length) {

	int rv=-1;

	struct weatherRecord record;
	weatherRecordPtr p = NULL;

	static struct tm wstime;
	time_t t;
	char APRSTime[7];
	char APRSWind[12];

	/* read the latest weather data */
	dopen(":usb:");
	p = rread(&record, 0);

	if (p!=NULL) {
		/* convert local time from ws data record to APRS UTC time stamp */
		strptime(getDateTime(), "%Y-%m-%d %H:%M", &wstime);
		wstime.tm_isdst = -1;
		t = mktime(&wstime);
		strftime(APRSTime, sizeof(APRSTime), "%d%H%M", gmtime(&t));

		/* wind can return direction 128 in error */
		if (p->windDir > 15) {
			/* error, set all wind fields to unavailable */
			sprintf(APRSWind, ".../...g...");
		}
		else
		{
			/* format the wind fields : convert direction from 16 cardinals to 1 to 360 degrees and wind speeds from m/s to knots */
			sprintf(APRSWind, "%03d/%03.0fg%03.0f", p->windDir == 0 ? 360 : p->windDir * 360 / 16, MS_TO_KNOTS(p->windSpeed), MS_TO_KNOTS(p->gustSpeed));
		}

		/* format the WX beacon message : convert temperature from celsius to fahrenheit, humidity to 2 digits and pressure to tenths of mb */
		snprintf(message, length, "/%6.6sz5231.73N/00013.24E_%st%03.0fh%02db%05.0f", APRSTime, APRSWind, C_TO_F(p->tempOut), p->humOut == 100 ? 0 : p->humOut, p->press * 10);

		rv = 0;
	}
	dclose();
	return rv;
}

