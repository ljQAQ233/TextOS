SRC  := $(wildcard *.c)
OBJ  := $(patsubst %.c,%.o,${SRC})

OUTPUT := $(OUTPUT)/Tools

.PHONY: Tools
Tools: $(OUTPUT) $(OUTPUT)/Tools.o

$(OUTPUT):
	mkdir -p $(OUTPUT)

$(OUTPUT)/Tools.o : $(OBJ)
	$(LD) $(LDFLAGS) -r $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $(abspath $<) -o $(abspath $@)

.PHONY: clean
clean:
	rm -rf *.o

