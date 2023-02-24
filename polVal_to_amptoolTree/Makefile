ROOT_CFLAGS   := $(shell root-config --cflags)
ROOT_LIBFLAGS := $(shell root-config --libs) -lMinuit -lMathCore -lMathMore -lTreePlayer -lRooFitCore


###############################
# should be able to link to the library and not the individual
# object files -- but can't for some reason on stanley
# shouldn't need any of this:
###############################

EXE_SRCFILES := $(wildcard *.cc)
EXE_EXEFILES := $(foreach file,$(EXE_SRCFILES),$(file:.cc=))

EXTRA_SRCFILES := $(wildcard *.C)
EXTRA_INCFILES := $(wildcard *.h)

all: $(EXE_EXEFILES)

%: %.cc $(EXTRA_SRCFILES) $(EXTRA_INCFILES) $(FSROOT_LIBFILE)
	# DO THIS FOR NOW:
	g++ $(ROOT_CFLAGS) $(ROOT_LIBFLAGS) $(FSROOT_OBJFILES) $(FSROOT_INCFLAGS) -I. -o $@ $@.cc $(EXTRA_SRCFILES)
	# THIS WOULD BE BETTER:
	#g++ $(ROOT_CFLAGS) $(ROOT_LIBFLAGS) $(FSROOT_LIBFLAGS) $(FSROOT_INCFLAGS) -I. -o $@ $@.cc $(EXTRA_SRCFILES)

clean:
	rm -rf $(EXE_EXEFILES)

