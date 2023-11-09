t0 = 58.12
duration=25
plot 'yat.dat' u ($1-t0):3 w l t'Capacitor', '' u ($1-t0):4 w l t'Drogue', '' u ($1-t0):5 w l t'Paraquedas'\
	, 'eeprom.txt ' u 1:2 w lp t'EEPROM'