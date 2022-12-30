SRC  := $(wildcard *.c)
OBJ  := $(patsubst %.c,%.o,${SRC})

OUTPUT := $(OUTPUT)/Console

.PHONY: Console
Console: $(OUTPUT) $(OUTPUT)/Console.o

$(OUTPUT):
	mkdir -p $(OUTPUT)

$(OUTPUT)/Console.o : $(OBJ)
	$(LD) $(LDFLAGS) -r $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $(abspath $<) -o $(abspath $@)

.PHONY: clean
clean:
	rm -rf *.o

