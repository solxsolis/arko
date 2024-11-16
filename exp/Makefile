EXEFILE = julia
OBJECTS = julia.o drawJulia.o
CCFMT = -m64
LIBS = -lallegro -lallegro_primitives -lallegro_font -no-pie
NASMFMT = -f elf64
CCOPT = 
NASMOPT = -w+all

.c.o:
	gcc $(CCFMT) $(CCOPT) -c $<

.s.o:
	nasm $(NASMFMT) $(NASMOPT) -l $*.lst $<

$(EXEFILE): $(OBJECTS)
	gcc $(CCFMT) -o $@ $^ $(LIBS)
	
clean:
	rm *.o *.lst $(EXEFILE)
