SRC  := $(wildcard *.c)
OBJ  := $(patsubst %.c,%.o,${SRC})

OUTPUT := $(OUTPUT)/Graphics

.PHONY: Graphics
Graphics: $(OUTPUT) $(OUTPUT)/Graphics.o

$(OUTPUT):
	mkdir -p $(OUTPUT)

$(OUTPUT)/Graphics.o : $(OBJ)
	$(LD) $(LDFLAGS) -r $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $(abspath $<) -o $(abspath $@)

.PHONY: clean
clean:
	rm -rf *.o

