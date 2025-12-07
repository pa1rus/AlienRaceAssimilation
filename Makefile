SRC = $(shell find src -name "*.c") lib/cjson/cJSON.c
INCDIR_SRC = $(shell find src -type d -exec echo -I{} \;)
INCDIR_LIB = -Ilib -Ilib/cjson

BUILD_DIR  = build
OUT_NATIVE = $(BUILD_DIR)/linux/game
OUT_WEB    = $(BUILD_DIR)/web/index.html

CC_NATIVE     = zig cc
CFLAGS_NATIVE = -Wall -std=c99 $(INCDIR_SRC) $(INCDIR_LIB)
LIBS_NATIVE   = -Llib -lraylib -lcjson -lhermes -lenet 

CC_WEB       = emcc
INCDIR_WEB   = $(INCDIR_SRC) -Ilib -Ilib/cjson
CFLAGS_WEB   = -O2 -Wall -DPLATFORM_WEB -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 $(INCDIR_WEB)
LIBS_WEB     = ./lib/libraylib.web.a -s USE_GLFW=3 -s ASSERTIONS=0 -s GL_DEBUG=0 -s NO_EXIT_RUNTIME=1

ifeq ($(OS),Linux)
	LIBS_NATIVE +=  -luuid
endif

.PHONY: all clean native web

all: native

native:
	@mkdir -p $(BUILD_DIR)/linux
	$(CC_NATIVE) $(CFLAGS_NATIVE) $(SRC) -o $(OUT_NATIVE) $(LIBS_NATIVE)
	./$(OUT_NATIVE)

web:
	@mkdir -p $(BUILD_DIR)/web
	$(CC_WEB) $(CFLAGS_WEB) $(SRC) $(LIBS_WEB) \
	--preload-file assets/audio@/assets/audio \
	--preload-file assets/background@/assets/background \
	--preload-file assets/animations@/assets/animations \
	--preload-file assets/cutscene@/assets/cutscene \
	--preload-file assets/maps@/assets/maps \
	--shell-file src/minshell.html \
	-o $(OUT_WEB)


clean:
	rm -rf $(BUILD_DIR)
