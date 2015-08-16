reset

set style histogram rowstacked gap 0
set style fill solid 0.6 border lt -1
set xlabel "Process number" font "Times-Italic,13"
set ylabel "Time(microseconds)" font "Times-Italic,13"
set title "BeagleBoard Black Context Switch Latency Measurement" font ",18"
plot "../LAT_CTX_results/result_32k.txt" using 1:2 smooth freq with boxes title "Process size=32kb",\
"../LAT_CTX_results/result_32k.txt" using 1:($2 + 0.35):($2) with labels font ",7" notitle

set terminal postscript color
set output "../LAT_CTX_results/graph/32k_h.ps"
replot
