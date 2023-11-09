set term pdf enhanced color
set out 'rRocketEZ-1-firmware1_5_6.pdf'

t0=272.455
tf=24

set ylabel "h (m)"
set xlabel "t (s)"

set grid
set y2tics
set ytics nomirror

plot [0:tf] 'altimeter.txt' u 1:2 w lp t'h - rRocket (left)' axis x1y1, 'yat.dat' u ($1-t0):2 w lp t'h - Instrumentation (left)' axis x1y1, '' u ($1-t0):($4/1023*100) w lp t'Drogue signal (right)' axis x1y2, '' u ($1-t0):($5/1023*100) w lp t'Parachute signal (right)' axis x1y2