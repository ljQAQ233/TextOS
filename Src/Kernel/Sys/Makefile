SRC  := $(wildcard *.c)
OBJ  := $(patsubst %.c,%.o,${SRC})

OUTPUT := $(OUTPUT)/Sys

.PHONY: Sys
Sys: $(OUTPUT) $(OUTPUT)/Sys.o

$(OUTPUT):
	mkdir -p $(OUTPUT)

$(OUTPUT)/Sys.o : $(OBJ)
	$(LD) $(LDFLAGS) -r $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $(abspath $<) -o $(abspath $@)

.PHONY: clean
clean:
	rm -rf *.o

