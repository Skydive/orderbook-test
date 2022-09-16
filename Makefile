Q = @
CC = g++
CXXVERSION = -std=c++1z
CFLAGS = -Wall -Werror -Wextra -O2 $(CXXVERSION)
LFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
OUT_DIR = bin

FILES_CPP = main.cpp OrderBook.cpp OrderBook_Print.cpp Order.cpp
FILES_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(FILES_CPP:.cpp=.o)))
FILE_OUT = $(OUT_DIR)/IcebergExecutable

TARGETS = $(FILES:%.cpp:%.o)

$(OBJ_DIR)/%.o: %.cpp
	$(Q)echo " CC $<"
	$(Q)mkdir -p $(OBJ_DIR)
	$(Q)$(CC) -c $< -o $@ $(CFLAGS)

$(FILE_OUT): $(FILES_OBJ)
	$(Q)echo "	LD $<"
	$(Q)mkdir -p $(OUT_DIR)
	$(Q)$(CC) $^ -o $@ $(LFLAGS)


build: $(FILE_OUT)

debug: CFLAGS += -DDEBUG -g -Og
debug: build

release: CFLAGS += -DNODEBUG -DRELEASE -O3
release: build

all: release

clean:
	$(Q)echo " RM -Rf $(OBJ_DIR)"
	$(Q)rm -Rf $(OBJ_DIR)