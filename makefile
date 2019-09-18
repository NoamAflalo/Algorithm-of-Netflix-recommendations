CFlAGS=-Wall -g -03
LDFLAGS=-lm

all : MyFiltering 

MyFiltering : tri.o eval.o  reco.o  projet.o main.o 
		gcc-o MyFiltering projet.o eval.o tri.o reco.o projet.o main.o $(CFLAGS)
		gcc-o tri.o -c eval.c $(CFLAGS)

projet.o : projet.c projet.h
	gcc-o projet.o -c projet.c $(CFLAGS)

eval.o : projet.c projet.h
	gcc-o projet.o -c projet.c $(CFLAGS)


reco.o: reco.c projet.h
	gcc-o tri.o -c eval.c $(CFLAGS)



main.o : main.c projet.h
	gcc-o main.o -c main.c $(CFLAGS)

clean: rm -rf *.o

