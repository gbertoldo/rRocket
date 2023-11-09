t0 = 30-2.2
t1 = 20.5
duration=25
plot 'yat.dat' u ($1-t0):3 w l t'Capacitor', '' u ($1-t0):4 w l t'Drogue', '' u ($1-t0):5 w l t'Paraquedas'\
	, 'eeprom.txt ' u 1:2 w lp t'EEPROM'\
	, 'events.txt' u ($1-t1):2:4 w labels point  pt 9 ps 1 offset char 5,1 notitle\
	, 'serial.txt' u ($1-t1):2 w lp t'Trajetoria'