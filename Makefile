CC = g++

CompileParms = -c -Wall -std=c++11 -O2

OBJS = standaard.o vieropeenrij.o main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o VierOpEenRij

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

vieropeenrij.o: vieropeenrij.cc standaard.h constantes.h vieropeenrij.h
	$(CC) $(CompileParms)  vieropeenrij.cc

main.o: main.cc constantes.h vieropeenrij.h
	$(CC) $(CompileParms)  main.cc

