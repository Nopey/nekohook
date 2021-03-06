
/*
 * logging.cpp
 *
 *  Created on: Oct 3, 2016
 *      Author: nullifiedcat
 */

#include <string.h> // String stuff
#include <time.h> 	// Time

#include "logging.hpp"

// Cathooks main logging util
CatLogger
#ifdef __GNUC__
 	__attribute__ ((init_priority (101)))// We want this to be init fast so we can log other inits!
#endif
g_CatLogging(P_tmpdir "/nekohook.log", true);

CatLogger::CatLogger(const char* _file_path, bool _ptime) : log_handle(fopen(_file_path, "w")), ptime(_ptime) {}
CatLogger::~CatLogger() {	fclose(log_handle); }

void CatLogger::log(const char* fmt, ...) {

	// Print our time if needed
	if (ptime) {
		// Get our time
		time_t current_time = time(0);
		struct tm* time_info = localtime(&current_time);

		// print it to a string
		char timeString[10];
		strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);

		// Print the time into the log
		fprintf(log_handle, "[Neko] [%s] ", timeString);
	}

	// Get the string we want to log
	char buffer[1024];
	va_list list;
	va_start(list, fmt);
	vsprintf(buffer, fmt, list);
	va_end(list);
	strcat(buffer, "\n");

	// Write our log to the file
	fputs(buffer, log_handle);
	fflush(log_handle);

	// Push result var to a console here
	console_tap(buffer);
}
