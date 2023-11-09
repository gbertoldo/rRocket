set term pdf enhanced color
set out 'hRocket-firmware1_5_6.pdf'

t0=2.76
tf=14

set ylabel "h (m)"
set xlabel "t (s)"

set grid

plot [0:tf] 'altimeter.txt' u 1:2 w lp t'h - hRocket', 'yat.dat' u ($1-t0):2 w lp t'h - Instrumentation'