# Compiler and flags
CC            = g++
CFLAGS        = -std=c++17 -Wall -Wextra -Weffc++ -pedantic -fPIE -Iinclude
C_DEBUG_FLAGS = -g
RM            = /bin/rm -f
LIB_DIR       = ./lib
SRC_DIR       = ./src
OBJ_DIR       = ./obj
LIBS          = -L$(LIB_DIR) -lISCAS -lDIFF_HUFF
SOURCES       = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS       = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET        = robdd
LINKS         = robdd-sizes robdd-opt uncompress compare

# Default target
all: $(TARGET) links

# Compile the target
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(C_DEBUG_FLAGS) $(CFLAGS) -MMD -c $< -o $@

-include $(OBJECTS:.o=.d)

# Create symbolic links
links: $(TARGET)
	@for link in $(LINKS); do \
		ln -sf $(TARGET) $$link; \
	done

# Clean up
clean:
	$(RM) $(TARGET) $(OBJ_DIR)/*.d $(OBJ_DIR)/*.o $(LINKS)

# Phony targets
.PHONY: all links clean
