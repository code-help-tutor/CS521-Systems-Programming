WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include "util.h"
#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include<stdlib.h>

/* Forward declarations: */
void human_readable_size(char *buf, size_t buf_sz, double size, unsigned int decimals);
size_t simple_time_format(char *buf, size_t buf_sz, time_t time);

/**
 * Converts a size in bytes to human-readable format string (KiB, MiB, etc.).
 *
 * @param buf Buffer to print the human-readable format to as a string
 * @param buf_sz Size of the buffer to write to
 * @param size Size to be converted
 * @param decimals Number of decimals to display in the formatted string
 */
void human_readable_size(char *buf, size_t buf_sz, double size, unsigned int decimals)
{
    
    char *suffix[] = { "KiB", "MiB", "GiB", "TiB"};
	char length = sizeof(suffix) / sizeof(suffix[0]);

	int i = 0;
	double dblBytes = size;
	char unit[4];

	if(size < 1024){
		strcpy(unit, "B");

	}
	else{
		for (i = 0; dblBytes >=1024 && i<length; i++){
			dblBytes = dblBytes / 1024.0;
		 	LOG("%f\n", dblBytes);
			LOG ("%d\n", i);
		}
		strcpy(unit, suffix[--i]);
	}
    char d[5];
    if (decimals == 0) {
      strcpy(d, "%f");
    } else if (decimals == 1) {
      strcpy(d, "%.1f"); 
    } else if (decimals == 2) {
      strcpy(d, "%.2f"); 
    } else if (decimals == 3) {
      strcpy(d, "%.3f");
    } else {
      strcpy(d,"%.1f");
    }     
    
    snprintf(buf, buf_sz, d, dblBytes);
    strcat(buf, " "); 
    strcat(buf, unit);   
}

/**
 * Converts a time_t timespec to a simplified format, (Day Mon Year), or
 * "DD MMM YYYY".
 *
 * @param buf Buffer to print the formatted time string to
 * @param buf_sz Size of the buffer to write to
 * @param time Time to convert to a string
 */

size_t simple_time_format(char *buf, size_t buf_sz, time_t time)
{
    struct tm *tmptime = localtime(&time);
    return strftime(buf, buf_sz, "%b %d %G", tmptime);
}


//void main(){
//	char buff[15];
//	double size = 2.8* 1024*1024 ;
//	human_readable_size(buff, 15, size,1);
//	printf("%s\n", buff);
//}
