TARGET_DIR=bin

ifeq ($(.DEFAULT_GOAL), )
	.DEFAULT_GOAL := debug
endif

#phony targets won't check modification dates and will always build
.phony: all clean debug release

ROOT_SRC_DIR:=$(abspath .)
CUR_MAKE_DIR:=$(abspath .)
ROOT_HTS_DIR:=$(abspath .)
CUR_SRC_DIR:=$(abspath .)

# included from a different makefile
LDFLAGS += -l pthread -l rt -l stdc++ -lcrypto -lssl -lcurl -ljson-c

ifeq ($(mode), release)
	tmp_flags := -O2
	CFLAGS := $(CFLAGS) $(tmp_flags)
	CXXFLAGS := $(CXXFLAGS) $(tmp_flags)
else
	tmp_flags := -g -D DEBUG
	CFLAGS := $(CFLAGS) $(tmp_flags)
	CXXFLAGS := $(CXXFLAGS) $(tmp_flags)
endif

tmp_flags := -I $(CUR_SRC_DIR) -Wall
CFLAGS := $(CFLAGS) $(tmp_flags) -std=gnu11
CXXFLAGS := $(CXXFLAGS) $(tmp_flags) -std=gnu++11

TOOL_NAME:=$(notdir $(CUR_SRC_DIR))
TARGET_FOLDER:=$(ROOT_HTS_DIR)/$(TARGET_DIR)

TARGET_NAME:=$(TOOL_NAME)
TARGET_PATH:=$(TARGET_FOLDER)/$(TARGET_NAME)
DEBUGFILE:=$(TARGET_FOLDER)/$(TARGET_NAME).dbg

SRC_LIST := $(shell find $(CUR_SRC_DIR)/src -maxdepth 1 -name *.c*)

TMP_OBJ_FILES:=$(patsubst %.cc, %.o,$(patsubst %.c, %.o,$(subst $(CUR_SRC_DIR)/,,$(SRC_LIST))))

vpath %.cc $(CUR_SRC_DIR)
vpath %.c $(CUR_SRC_DIR)

debug: $(TARGET_FOLDER) $(TARGET_PATH)

release: $(TARGET_FOLDER) $(TARGET_PATH)

$(TARGET_PATH): $(TMP_OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(EXTRA_OBJ_FILES)
	objcopy --only-keep-debug "$@" "${DEBUGFILE}"
	strip --strip-debug --strip-unneeded "$@"
	objcopy --add-gnu-debuglink="${DEBUGFILE}" "$@"
	chmod -x "${DEBUGFILE}"

$(TARGET_FOLDER):
	mkdir -p $@

clean:
	@rm -Rf $(TARGET_PATH)
	@rm -Rf $(DEBUGFILE)
	@rm -f *.o
