t0=0
plot 'yat.dat' u ($1-t0):2 w lp t'Instrumentacao', 'yat.dat' u ($1-t0):3 w lp t'Capacitor', '' u ($1-t0):4 w lp t'Drogue', '' u ($1-t0):5 w lp t'Paraquedas', 'eeprom.txt' u 1:2 w lp t'Altimetro'