SRC  := $(wildcard *.c)
OBJ  := $(patsubst %.c,%.o,${SRC})

OUTPUT := $(OUTPUT)/VSPrintf

.PHONY: VSPrintf
VSPrintf : $(OUTPUT) $(OUTPUT)/VSPrintf.o

$(OUTPUT):
	mkdir -p $(OUTPUT)

$(OUTPUT)/VSPrintf.o : $(OBJ)
	$(LD) $(LDFLAGS) -r $^ -o $@
	echo "$(OUTPUT)"

%.o : %.c
	$(CC) $(CFLAGS) -c $(abspath $<) -o $(abspath $@)

.PHONY: clean
clean:
	rm -rf *.o

