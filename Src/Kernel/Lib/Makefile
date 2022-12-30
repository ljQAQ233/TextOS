OUTPUT := $(OUTPUT)/Lib

.PHONY: Lib
Lib: $(OUTPUT) $(OUTPUT)/Lib.o

$(OUTPUT):
	mkdir -p $(OUTPUT)

$(OUTPUT)/Lib.o: $(OUTPUT)/String/String.o \
	$(OUTPUT)/VSPrintf/VSPrintf.o
	$(LD) $(LDFLAGS) -r $^ -o $@

.PHONY: $(OUTPUT)/VSPrintf/VSPrintf.o
$(OUTPUT)/VSPrintf/VSPrintf.o:
	make -C VSPrintf

.PHONY: $(OUTPUT)/String/String.o
$(OUTPUT)/String/String.o:
	make -C String

.PHONY: clean
clean:
	make -C String clean
	make -C VSPrintf clean
