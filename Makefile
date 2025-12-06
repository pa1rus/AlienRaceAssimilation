SRC        = $(shell find src -name "*.c")
INCDIR_SRC = $(shell find src -type d -exec echo -I{} \;)
INCDIR_LIB = -Ilib

BUILD_DIR  = build
OUT_NATIVE = $(BUILD_DIR)/linux/game

CC_NATIVE     = zig cc
CFLAGS_NATIVE = -Wall -std=c99 $(INCDIR_SRC) $(INCDIR_LIB)
LIBS_NATIVE   = -Llib -lraylib -lcjson -lhermes -lenet

.PHONY: all clean native web

all: native

native:
	@mkdir -p $(BUILD_DIR)/linux
	$(CC_NATIVE) $(CFLAGS_NATIVE) $(SRC) -o $(OUT_NATIVE) $(LIBS_NATIVE)
	./$(OUT_NATIVE)

clean:
	rm -rf $(BUILD_DIR)
