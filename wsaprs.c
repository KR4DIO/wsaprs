#define _XOPEN_SOURCE

#include <stdio.h>
#include <time.h>

#include "dfile.h"
#include "header.h"
#include "wrecord.h"
#include "wsaprs.h"

int aprs_wx(char* message, int length) {

	int rv=-1;

	struct weatherRecord record;
	weatherRecordPtr p = NULL;

	static struct tm wstime;
	time_t t;
	char APRSTime[7];

	/* read the latest weather data */
	dopen(":usb:");
	p = rread(&record, 0);

	if (p!=NULL) {
		/* convert local time from ws data record to APRS UTC time stamp */
		strptime(getDateTime(), "%Y-%m-%d %H:%M", &wstime);
		wstime.tm_isdst = -1;
		t = mktime(&wstime);
		strftime(APRSTime, sizeof(APRSTime), "%d%H%M", gmtime(&t));

		/* format the WX beacon message : convert wind speeds from m/s to knots and temperature from celsius to fahrenheit */
		snprintf(message, length, "/%6.6sz5231.73N/00013.24E_%03d/%03.0fg%03.0ft%03.0fh%02db%05.0f", APRSTime, p->windDir==0?360:p->windDir * 360 / 16, p->windSpeed * 1.9438444924574, p->gustSpeed * 1.9438444924574, (p->tempOut * 1.8) + 32, p->humOut==100?0:p->humOut, p->press * 10);

		rv = 0;
	}
	dclose();
	return rv;
}

