PREFIX = /usr/local

batteryInfo: batteryInfo.c
	cc -o batteryInfo -O2 batteryInfo.c

install: batteryInfo
	install -m 755 batteryInfo $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/batteryInfo
