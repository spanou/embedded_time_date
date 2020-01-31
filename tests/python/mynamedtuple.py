import time

class StructTimeUnix:

	weekDay = {
		0: "Sun",
		1: "Mon",
		2: "Tue",
		3: "Wed",
		4: "Thu",
		5: "Fri",
		6: "Sat"
	}

	yearMon = {
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
		10:"Nov",
		11:"Dec"
	}

	inst = time.localtime(time.time())

	def __init__(self, tm=None):
		if(tm == None):
			return

	def __getattr__(self, name):

		print(name + "\n")

		if(name == "tm_hour"):
			return self.inst.tm_hour
		elif(name == "tm_min"):
			return self.inst.tm_min
		elif(name == "tm_sec"):
			return self.inst.tm_sec
		elif(name == "tm_mday"):
			return self.inst.tm_mday
		elif(name == "tm_wday"):
			return 0 if self.inst.tm_wday==6 else self.inst.tm_wday +1
		elif(name == "tm_yday"):
			return self.inst.tm_yday-1
		elif(name == "tm_year"):
			return self.inst.tm_year
		elif(name == "tm_mon"):
			return self.inst.tm_mon-1
		elif(name == "tm_zone"):
			return self.inst.tm_zone
		elif(name == "tm_isdst"):
			return self.inst.tm_isdst
		elif(name == "tm_gmtoff"):
			return self.inst.tm_gmtoff
		else:
			raise AttributeError

	def __str__(self):
		return("{wday}, {mday:0>2} {ymon} {year:0>4} {hour:0>2}:{min:0>2}:{sec:0>2}".format(
			wday=self.weekDay[self.tm_wday],
			mday=str(self.tm_mday),
			ymon=self.yearMon[self.tm_mon],
			year=str(self.tm_year),
			hour=str(self.tm_hour),
			min=str(self.tm_min),
			sec=str(self.tm_sec)))

def main():


	thisTime = StructTimeUnix()
	print(thisTime)


main()
