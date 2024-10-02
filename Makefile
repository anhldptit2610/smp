TARGET = sol
CC = g++
CXXFLAGS = -g -Wall -std=c++20
BUILD_DIR = build
SRC_DIR = Src
INC_DIR = Inc
LIBS = -lncurses -ltag -lSDL2 -lSDL2_mixer
INC = -I$(INC_DIR)


# CXX_SRC = $(wildcard $(SRC_DIR)/*.cc)
# CXX_SRC += $(wildcard $(SRC_DIR)/$(MODULE)/%.cc)
CXX_SRC = $(shell find $(SRC_DIR) -name '*.cc')

# OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(BUILD_DIR)/%.o, $(filter $(SRC_DIR)/%.cc, $(CXX_SRC)))
# OBJECTS += $(patsubst $(SRC_DIR)/$(MODULE)/%.cc, $(BUILD_DIR)/%.o, $(filter $(SRC_DIR)/$(MODULE)/%.cc, $(CXX_SRC)))

# OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(BUILD_DIR)/%.o, $(filter $(SRC_DIR)/%.cc,$(CXX_SRC)))
# OBJECTS += $(patsubst $(SRC_DIR)/$(MODULE)/%.cc, $(BUILD_DIR)/%.o, $(filter $(SRC_DIR)/$(MODULE)/%.cc, $(CXX_SRC)))
OBJECTS = $(patsubst %.cc, $(BUILD_DIR)/%.o, $(notdir $(filter %.cc,$(CXX_SRC))))

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CXXFLAGS) $^ $(LIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -c $< $(INC) -o $@

$(BUILD_DIR)/%.o: %.cc | $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -c $< $(INC) -o $@

$(BUILD_DIR):
	mkdir build

clean:
	rm -rf build