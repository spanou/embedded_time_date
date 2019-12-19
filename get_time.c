// Compile as g++ -Wall -o get_time get_time.c
#include<iostream>
#include<time.h>
#include<string.h>

using namespace std;


unsigned long timeInSeconds(struct tm time, bool adjustForUTC);
bool isYearLeap(unsigned long year);
bool leapYearChecker(unsigned int start, unsigned int end);

void printTable(const unsigned int* table, 
	const unsigned int tableSize, 
	const unsigned char rowLength);

const static unsigned long epochYear = 1970;
const static unsigned long secondsInYear = 31536000;
const static unsigned long secondsInDay = 86400;
const static unsigned long secondsInHour = 3600;
const static unsigned long secondsInMin = 60;
const static unsigned long yearInStructTm = 1900;

// Leap Years from 1800-2400 
// https://kalender-365.de/leap-years.php
// https://miniwebtool.com/leap-years-list/?start_year=1800&end_year=2400
unsigned int leapYearCheckTable[] = {
	1804, 1808, 1812, 1816, 1820, 1824, 1828, 1832, 1836, 1840,
	1844, 1848, 1852, 1856, 1860, 1864, 1868, 1872, 1876, 1880,
	1884, 1888, 1892, 1896, 1904, 1908, 1912, 1916, 1920, 1924,
	1928, 1932, 1936, 1940, 1944, 1948, 1952, 1956, 1960, 1964,
	1968, 1972, 1976, 1980, 1984, 1988, 1992, 1996, 2000, 2004,
	2008, 2012, 2016, 2020, 2024, 2028, 2032, 2036, 2040, 2044,
	2048, 2052, 2056, 2060, 2064, 2068, 2072, 2076, 2080, 2084,
	2088, 2092, 2096, 2104, 2108, 2112, 2116, 2120, 2124, 2128,
	2132, 2136, 2140, 2144, 2148, 2152, 2156, 2160, 2164, 2168,
	2172, 2176, 2180, 2184, 2188, 2192, 2196, 2204, 2208, 2212,
	2216, 2220, 2224, 2228, 2232, 2236, 2240, 2244, 2248, 2252,
	2256, 2260, 2264, 2268, 2272, 2276, 2280, 2284, 2288, 2292,
	2296, 2304, 2308, 2312, 2316, 2320, 2324, 2328, 2332, 2336,
	2340, 2344, 2348, 2352, 2356, 2360, 2364, 2368, 2372, 2376,
	2380, 2384, 2388, 2392, 2396, 2400
};

int main(int argc, char* argv[]){

	time_t theTimeInSeconds; 

	theTimeInSeconds = time(0);
	struct tm *theTime = localtime(&theTimeInSeconds);

	leapYearChecker(1800, 2400);

	cout << "The Current Time Since 1970-1-1, 00:00:00 +0000UTC" << endl; 
	cout << "In Seconds : " << theTimeInSeconds << endl;

	if(0 == theTime)
		return(-1);

	cout << "In YYYY-MM-DD, hh:mm:ss : " << (yearInStructTm+theTime->tm_year) 
		<< "-" << (theTime->tm_mon +1) << "-" 
		<< theTime->tm_mday << ", " 
		<< theTime->tm_hour << ":" 
		<< theTime->tm_min << ":" 
		<< theTime->tm_sec << endl;

	unsigned long curTimeInSeconds = timeInSeconds(*theTime, true);
	cout << "curTimeInSeconds = " << curTimeInSeconds << endl;

	return(0);
}


unsigned long timeInSeconds(struct tm time, bool adjustForUTC){
	//
	// Current Year - 1970
	//
	const unsigned long yearsInTotal = (time.tm_year + yearInStructTm) - epochYear;
	unsigned long timeInSeconds = 0;

	// cout << "Epoch Year: " << epochYear << endl;
	// cout << "Years In Total: " << yearsInTotal << endl;
	// cout << "TimeZone : " << time.tm_gmtoff << endl;

	// Count all the leap years since the epoch
	unsigned int leapYearCount = 0;
	for(unsigned int i = 0; i < yearsInTotal; i++){	
		unsigned int year = epochYear + i;	
		// Is this a leap year ?
		if(isYearLeap(year)){
			++leapYearCount;	
			//cout << year << " is a leap year." << endl;
		}
	}

	// The time in seconds for all the years since 1970 + the additional leap days.
	timeInSeconds = ((secondsInYear * yearsInTotal) + (leapYearCount * secondsInDay));
	// The time in seconds for all the days for the current year. 
	timeInSeconds += (time.tm_yday * secondsInDay);	
	// The time in seconds for all the hours in the current day.
	timeInSeconds += (time.tm_hour * secondsInHour);
	// The time in seconds for the minutes since the current hour 
	timeInSeconds += (time.tm_min * secondsInMin);
	// The time in seconds from the current minute.
	timeInSeconds += time.tm_sec;
	// Adjust the timezone for UTC.
	if(true == adjustForUTC) {
		timeInSeconds -= time.tm_gmtoff;	
	}
	

	// The value should match: https://www.unixtimestamp.com/index.php  
	return(timeInSeconds);
}


//
// Formulae for determining Leap Years 
// 
// POSIX.1 defines seconds since the Epoch using a formula that approximates 
// the number of seconds between a specified time and the Epoch. 
// This formula takes account of the facts that all years that are evenly 
// divisible by 4 are leap years, but years that are evenly divisible by 100 
// are not leap years unless they are also evenly divisible by 400, in which 
// case they are leap years. This value is not the same as the actual number 
// of seconds between the time and the Epoch, because of leap seconds and 
// because system clocks are not required to be synchronized to a standard 
// reference. The intention is that the interpretation of seconds since the 
// Epoch values be consistent; see POSIX.1-2008 Rationale A.4.15 for further 
// rationale.
//
bool isYearLeap(unsigned long year) {
	bool ret = false;

	// Is this year divisible by 4 ?
	if( (0 == (year % 4)) ) {		
		ret = true;		
		if(0 == (year % 100)){
			ret = false;
			if(0 == (year % 400)){
				ret = true;
			}
		}
	}

	return(ret);
}

bool leapYearChecker(unsigned int start, unsigned int end){

	bool ret = false;

	// End has to be higher than start
	if(end < start){
		return(ret);
	}

	const unsigned int leapYearTableSize = sizeof(leapYearCheckTable)/sizeof(unsigned int);
	const unsigned int leapYearCalcTableSize = ((end-start) + 1);
	unsigned int calculatedLeapYearTable[leapYearCalcTableSize];
	unsigned int calculatedLeapYearTableIndex = 0;

	cout << endl;
	cout << "========== Printing Golden Table ==========" << endl;
	cout << "Table Size = "  << leapYearTableSize << endl;
	printTable(leapYearCheckTable, leapYearTableSize, 10);
	cout << endl;

	for(unsigned int i = start; i <= end; i++){
		if(true == isYearLeap(i)){
			calculatedLeapYearTable[calculatedLeapYearTableIndex++] = i;			
		}
	}

	cout << endl;
	cout << "========== Printing Calculated Table =========="  << endl;
	cout << "Table Size = "  << calculatedLeapYearTableIndex << endl;
	printTable(calculatedLeapYearTable, calculatedLeapYearTableIndex, 10);
	cout << endl;

	if(leapYearTableSize != calculatedLeapYearTableIndex){
		cout << "Error: Golden Leap Table Size = " << leapYearTableSize 
			 << " Calculated Leap Table Size = "  << calculatedLeapYearTableIndex << endl;
 	} 

 	if(0 != memcmp((const void*)leapYearCheckTable, (const void*)calculatedLeapYearTable, leapYearTableSize)) {
 		cout << "Error: Golden Leap Table doesn't match Calculated Leap Table" << endl;
 	}


	return(ret);
}

//=============================================================================
//  
// printTable: Prints a table of data in tabular format
//
// Prints a table of data in tabular format taking in as parameters 
// the table itself, the size of the table, and the row length.  
// 
// 
// Notes: N/A 
//=============================================================================
void printTable(const unsigned int* table, const unsigned int size, 
	const unsigned char blockSize){

	if((0==table) || (0==size) || (0==blockSize))
		return;

	for(unsigned int i = 0; i < size; i++) {
		if(0 == ((i+1) % (blockSize))) {
			cout << table[i]  << "," << endl;
		} else {
			cout << table[i]  << ", " ; 
		}
	}
	cout << endl;
}