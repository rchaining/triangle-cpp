# --- Variables ---
CXX := clang++
CXXFLAGS := -std=c++17 -I./metal-cpp -g
LDFLAGS := -framework Cocoa -framework Metal -framework QuartzCore -framework Foundation

# The name of your app
TARGET := HelloMetal
# The name of your shader library
METALLIB := default.metallib

# Source files
SRCS := main.mm Renderer.cpp
# Object files (automatically derived from sources)
OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.mm=.o)

# --- Rules ---

# 1. The Default Rule (runs when you type 'make')
all: $(TARGET) $(METALLIB)

# 2. Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 3. Compile C++ files (.cpp -> .o)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 4. Compile Objective-C++ files (.mm -> .o)
%.o: %.mm
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 5. Compile Shaders (.metal -> .metallib)
$(METALLIB): Shaders.metal
	xcrun -sdk macosx metal -c Shaders.metal -o Shaders.air
	xcrun -sdk macosx metallib Shaders.air -o $(METALLIB)
	rm Shaders.air

# 6. Clean up junk
clean:
	rm -f $(TARGET) $(OBJS) $(METALLIB) Shaders.air