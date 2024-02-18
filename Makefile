CPPFLAGS = -Wall -std=c++14 -Wno-multichar
LINK.o = $(LINK.cc)

mkcsv: mkcsv.o

proper:
	rm -f *.log *.aux

clean:
	rm -f mkcsv mkcsv.o
	rm -f 1*.csv 2*.csv 3*.csv 4*.csv
	rm -f 1*.tex 2*.tex 3*.tex 4*.tex
	rm -f *.html *.log *.aux *.pdf
