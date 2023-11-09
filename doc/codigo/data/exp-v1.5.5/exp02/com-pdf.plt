t0=-0.6+0.155
h0=3.55
set term pdf enhanced color font "Arial,10"

set out 'fig.pdf'

set xrange [28:60]
set grid
set key right center
set xlabel 't(s)'

set y2tics
set ytics nomirror

plot 'yat.dat' u ($1-t0):($2-h0) w lp ps 0.2 t'h (m) - Instrumentacao' axis x1y1\
	, '' u ($1-t0):($3*100.0/1023) w l lw 2 t'Capacitor' axis x1y2\
	, '' u ($1-t0):($4*100.0/1023) w l lc 7 lw 2 t'Drogue' axis x1y2\
	, '' u ($1-t0):($5*100.0/1023) w l lc 6 lw 2 t'Paraquedas' axis x1y2\
	, 'eeprom.txt ' u 1:2 w lp ps 0.2 t'h (m) - EEPROM' axis x1y1\
