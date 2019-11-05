# Nom de votre projet
# (utilisez un seul mot, exemple: 'monprojet')
PROJECTNAME=read
# Fichiers sources
PRJSRC= read.cpp

##### A VERIFIER #####
# LIBRAIRIE= $(libRobot).a
# AR= avr-ar
# LIBSRC= $(wildcard ../lib_dir/* .cpp)
# LIBOBJS= $(wildcard ../lib_dir/* .o) #../lib_dir/timer.o #../lib_dir/del.o ../lib_dir/memoire.o

# libdir: $(LIBRAIRIE)

# $(LIBRAIRIE): $(LIBOBJS)
# 	$(AR) -crs $(LIBSRC) $^ 

# Inclusions additionnels (ex: -I/path/to/mydir)
INC= -I ../lib_dir/

# Libraires a lier (ex: -lmylib)
LIBS= ../lib_dir/libRobot.a

#Nom des cibles par defaut
TRG=$(PROJECTNAME).out
HEXROMTRG=$(PROJECTNAME).hex
HEXTRG=$(HEXROMTRG) $(PROJECTNAME).ee.hex

# Make all permet simplement de compiler le projet
all: $(TRG)

include ../Makefile_Common

#$(MAKE) execute make pour la lib_dir, comme ca nous n'avons pas besoin de changer de repertoire pour make.
# Implementation de la cible
$(TRG): $(OBJDEPS)
	$(MAKE) -C ../lib_dir	
	$(CC) $(LDFLAGS) -o $(TRG) $(OBJDEPS) \
	-lm $(LIBS)

clean:
	$(REMOVE) $(TRG) $(TRG).map $(OBJDEPS) $(HEXTRG) *.d ../lib_dir/*.o ../lib_dir/*.a ../lib_dir/*.d write2/*.d write2/*.a write2/*.o write2/*.out write2/*.hex write/*.d write/*.a write/*.o write/*.out write/*.hex write/*.out.map write2/*.out.map

