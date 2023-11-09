reset

set term pdf enhanced color font "Arial,10"

set out 'trajectory.pdf'

set key right
set yrange [:60]
set grid

set xlabel 't (s)'

plot 'trajectory.txt' u 1:2 w lp ps 0.5 t'Altitude (m)', '' u 1:3 w lp pt 6 ps 0.2 t'Velocidade (m/s)'\
, 'events.txt' u 1:2:4 w labels point  pt 9 ps 1 offset char 5,1 notitle\
#, 'events.txt' u 1:3:4 w labels point  pt 9 ps 1 offset char 5,1 notitle
