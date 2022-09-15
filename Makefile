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
	echo " CC $<"
	$(Q)mkdir -p $(OBJ_DIR)
	$(Q)$(CC) -c $< -o $@ $(CFLAGS)

$(FILE_OUT): $(FILES_OBJ)
	echo "	LD $<"
	$(Q)mkdir -p $(OUT_DIR)
	$(Q)$(CC) $^ -o $@ $(LFLAGS)


debug: CCFLAGS += -DDEBUG -g

all: $(FILE_OUT)

clean:
	rm -R $(OBJ_DIR)