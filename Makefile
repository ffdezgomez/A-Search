# Compilador y banderas
CXX := g++
CXXFLAGS := -g -std=c++17  -Iinclude

# Directorios
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin

# Archivos
TARGET := $(BIN_DIR)/search
SOURCES := $(wildcard $(SRC_DIR)/*.cc)
OBJECTS := $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SOURCES))

# Regla por defecto
all: $(TARGET)

# Enlazar ejecutable (depende de bin/ y objetos)
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Compilar cada .cc en .o (depende de build/)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear directorios si no existen
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Limpiar compilados
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Ejecutar
# run: all
# 	./$(TARGET) input/prueba
