INCLUDEFLAGS=-Iinclude/
COMPILERFLAGS+=-Wall
COMPILERFLAGS+=-Werror
COMPILERFLAGS+=-O0
COMPILERFLAGS+=-ggdb
ROOTFLAGS=`root-config --cflags`
ROOTLIBDIR=`root-config --libdir`
ROOTLIBFLAGS=`root-config --libs`


COMP=g++ $(COMPILERFLAGS) $(INCLUDEFLAGS)

all: fit_cmdata

lib/read_histograms.o: src/read_histograms.cc src/read_histograms.h
	$(COMP) $(ROOTFLAGS) -c src/read_histograms.cc -o lib/read_histograms.o
	
lib/cmn_fit.o: src/cmn_fit.cc src/cmn_fit.h
	$(COMP) $(ROOTFLAGS) -c src/cmn_fit.cc -o lib/cmn_fit.o
	
lib/plotstyle.o: src/plotstyle.cc src/plotstyle.h
	$(COMP) $(ROOTFLAGS) -c src/plotstyle.cc -o lib/plotstyle.o
	
lib/conditions_data.o: src/conditions_data.cc src/conditions_data.h
	$(COMP) $(ROOTFLAGS) -c src/conditions_data.cc -o lib/conditions_data.o
	
lib/bad_strips.o: src/bad_strips.cc src/bad_strips.h
	$(COMP) $(ROOTFLAGS) -c src/bad_strips.cc -o lib/bad_strips.o
	
lib/hit_prob.o: src/hit_prob.cc src/hit_prob.h
	$(COMP) $(ROOTFLAGS) -c src/hit_prob.cc -o lib/hit_prob.o
	
lib/cm_toy.o: src/cm_toy.cc src/cm_toy.h
	$(COMP) $(ROOTFLAGS) -c src/cm_toy.cc -o lib/cm_toy.o
	
fit_cmdata: src/fit_cmdata.cc \
	lib/read_histograms.o \
	lib/cmn_fit.o \
	lib/conditions_data.o \
	lib/bad_strips.o \
	lib/plotstyle.o \
	lib/hit_prob.o \
	lib/cm_toy.o
	$(COMP) $(ROOTFLAGS) $(ROOTLIBFLAGS) \
	src/fit_cmdata.cc \
	lib/cmn_fit.o \
	lib/conditions_data.o \
	lib/plotstyle.o \
	lib/bad_strips.o \
	lib/hit_prob.o \
	lib/cm_toy.o \
	lib/read_histograms.o -o fit_cm_data
	
	
	
clean:
	rm -f cm_analysis lib/*.o