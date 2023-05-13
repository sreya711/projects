CC = gcc

SDIR = ./Func
ODIR = ./Func/Obj
IDIR = ./Headers

DEPS_1 = $(IDIR)/structs.h $(IDIR)/functions.h
DEPS_2 = $(IDIR)/stack.h
DEPS = $(IDIR)/*.h

a.out: $(ODIR)/main.o $(ODIR)/generate.o $(ODIR)/init.o $(ODIR)/insert.o $(ODIR)/free.o $(ODIR)/stack.o $(ODIR)/parse.o
	$(CC) -o $@ $^

$(ODIR)/main.o: main.c
	$(CC) -c -o $@ $<

$(ODIR)/generate.o: $(SDIR)/generate.c $(DEPS_1)
	$(CC) -c -o $@ $<

$(ODIR)/init.o: $(SDIR)/init.c $(DEPS_1)
	$(CC) -c -o $@ $<

$(ODIR)/insert.o: $(SDIR)/insert.c $(DEPS_1)
	$(CC) -c -o $@ $<

$(ODIR)/free.o: $(SDIR)/free.c $(DEPS_1)
	$(CC) -c -o $@ $<

$(ODIR)/stack.o: $(SDIR)/stack.c $(DEPS_2)
	$(CC) -c -o $@ $<

$(ODIR)/parse.o: $(SDIR)/parse.c $(DEPS)
	$(CC) -c -o $@ $<

.PHONY = clean

clean:
	rm -f ./Func/Obj/*.o ./a.out *.txt

