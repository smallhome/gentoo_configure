#!/bin/gnuplot

set terminal png size 800,600 crop
set output outputfile
set xtics 100
plot filename1 using 2:3 with points lw 3 title "1", filename2 using 2:3 with points lw 3 title "2"
print 'gnuplot finish'
