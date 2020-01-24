import minRtcWrapper as rtcLib

def main():
	if rtcLib.isLeapYear(2000) :
		print("Year 2000 is Leap")

	print("Year: 2000, Month: Jan, Day: 1 -> ( " +
		rtcLib.dayOfWeekFromDate(2000, "Jan", 1) + " )")

if __name__ == '__main__':
	main()