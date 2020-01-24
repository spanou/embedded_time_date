from _min_rtc_cffi import ffi,lib


dayCodeInString = {
	0: "Sunday",
	1: "Monday",
	2: "Tuesday",
	3: "Wednesday",
	4: "Thursday",
	5: "Friday",
	6: "Saturday"
}

dayStringInCode = {
	"Sunday" : 0,
	"Monday" : 1,
	"Tuesday" :2,
	"Wednesday": 3,
	"Thursday": 4,
	"Friday": 5,
	"Saturday": 6
}

monthCodeInString = {
  	 0: "Jan",
  	 1: "Feb",
  	 2: "Mar",
  	 3: "Apr",
  	 4: "May",
  	 5: "Jun",
  	 6: "Jul",
  	 7: "Aug",
  	 8: "Sep",
  	 9: "Oct",
  	 10: "Nov",
  	 11: "Dec"
}

monthStringInCode = {
  	"Jan": 0,
  	"Feb": 1,
  	"Mar": 2,
  	"Apr": 3,
  	"May": 4,
  	"Jun": 5,
  	"Jul": 6,
  	"Aug": 7,
  	"Sep": 8,
  	"Oct": 9,
  	"Nov": 10,
  	"Dec": 11
}

def isLeapYear(year) :
  return lib.isYearLeap(year)

def dayOfWeekFromDate(year, month, day):
  dayCode = ffi.new("uint8_t*", 0)
  dayString = ""

  if 0 == lib.dayOfWeek(year, monthStringInCode[month], day, dayCode) :
    dayString = dayCodeInString[dayCode[0]]
  else:
    dayString = "Error"

  ffi.release(dayCode)

  return(dayString)