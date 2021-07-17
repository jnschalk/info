
ifeq ($(.DEFAULT_GOAL), )
	.DEFAULT_GOAL := release
endif

.phony: all clean debug release

CUR_SRC_DIR:=$(abspath .)

tmp_flags := -I $(CUR_SRC_DIR)
CFLAGS := $(CFLAGS) $(tmp_flags)
CXXFLAGS := $(CXXFLAGS) $(tmp_flags)

TOOL_NAME:=$(notdir $(CUR_SRC_DIR))
TARGET_FOLDER:=$(CUR_SRC_DIR)/bin

TARGET_NAME:=$(TOOL_NAME)
TARGET_PATH:=$(TARGET_FOLDER)/$(TARGET_NAME)
DEBUGFILE:=$(TARGET_FOLDER)/$(TARGET_NAME).dbg

SRC_LIST := $(shell find $(CUR_SRC_DIR)/src -maxdepth 1 -name *.c*)

TMP_OBJ_FILES:=$(patsubst %.cc, %.o,$(patsubst %.c, %.o,$(subst $(CUR_SRC_DIR)/,,$(SRC_LIST))))

vpath %.cc $(CUR_SRC_DIR)/src
vpath %.c $(CUR_SRC_DIR)/src

all: $(TARGET_PATH)
debug: CXXFLAGS += -g -D DEBUG
debug: $(TARGET_FOLDER) $(TARGET_PATH)
release: $(TARGET_FOLDER) $(TARGET_PATH)
release: CXXFLAGS += -O2

src/main.o: src/GetBlockSubsidy.snip src/dummy_params.h

$(TARGET_PATH): $(TMP_OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(TMP_OBJ_FILES)
	objcopy --only-keep-debug "$@" "${DEBUGFILE}"
	strip --strip-debug --strip-unneeded "$@"
	objcopy --add-gnu-debuglink="${DEBUGFILE}" "$@"
	chmod -x "${DEBUGFILE}"

$(TARGET_FOLDER):
	mkdir -p $@

clean:
	@rm -Rf $(TARGET_PATH)
	@rm -Rf $(DEBUGFILE)
	@rm -f $(TMP_OBJ_FILES)
