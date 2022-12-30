SRC  := $(wildcard *.c)
OBJ  := $(patsubst %.c,%.o,${SRC})

OUTPUT := $(OUTPUT)/String

.PHONY: String
String : $(OUTPUT) $(OUTPUT)/String.o

$(OUTPUT):
	mkdir -p $(OUTPUT)

$(OUTPUT)/String.o : $(OBJ)
	$(LD) $(LDFLAGS) -r $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $(abspath $<) -o $(abspath $@)

.PHONY: clean
clean:
	rm -rf *.o

