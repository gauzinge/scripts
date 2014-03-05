INCLUDEFLAGS=-Iinclude/
COMPILERFLAGS+=-Wall
COMPILERFLAGS+=-Werror
COMPILERFLAGS+=-O0
COMPILERFLAGS+=-ggdb
ROOTFLAGS=`root-config --cflags`
ROOTLIBDIR=`root-config --libdir`
ROOTLIBFLAGS=`root-config --libs`


COMP=g++ $(COMPILERFLAGS) $(INCLUDEFLAGS)

all: cm_analysis

lib/read_histograms.o: src/read_histograms.cc src/read_histograms.h
	$(COMP) $(ROOTFLAGS) -c src/read_histograms.cc -o lib/read_histograms.o
	
lib/plotstyle.o: src/plotstyle.cc src/plotstyle.h
	$(COMP) $(ROOTFLAGS) -c src/plotstyle.cc -o lib/plotstyle.o
	
cm_analysis: src/cm_analysis.cc \
	lib/read_histograms.o \
	lib/plotstyle.o
	$(COMP) $(ROOTFLAGS) $(ROOTLIBFLAGS) \
	src/cm_analysis.cc \
	lib/plotstyle.o \
	lib/read_histograms.o -o cm_analyis
	
clean:
	rm -f cm_analysis lib/*.o