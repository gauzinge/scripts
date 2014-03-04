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

lib/read_histograms.o: src/read_histograms.cc
	$(COMP) $(ROOTFLAGS) -c src/read_histograms.cc -o lib/read_histograms.o
	
cm_analysis: src/cm_analysis.cc \
	lib/read_histograms.o 
	$(COMP) $(ROOTFLAGS) $(ROOTLIBFLAGS) \
	src/cm_analysis.cc \
	lib/read_histograms.o -o cm_analyis
	
clean:
	rm -f cm_analysis lib/*.o