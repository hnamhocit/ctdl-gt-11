# ==============================================================================
# Makefile - Đề Tài 11 (CTDL&GT): Quản Lý Biển Số Xe
# ==============================================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude
SRC_DIR  := src
INC_DIR  := include
BIN_DIR  := bin
DATA_DIR := data

TARGET   := $(BIN_DIR)/main

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

.PHONY: all clean run setup test help

all: setup $(TARGET)

setup:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(DATA_DIR)

$(TARGET): $(OBJS)
	@echo "==> Đang liên kết: $(TARGET)..."
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "==> Biên dịch thành công! Chạy bằng lệnh: ./$(TARGET)"

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "==> Biên dịch: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	@echo "==> Khởi chạy: ./$(TARGET)"
	./$(TARGET)

test: setup
	@echo "==> Biên dịch và chạy bộ kiểm thử Unit Tests..."
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/Plate.cpp $(SRC_DIR)/HashTable.cpp tests/test_hashtable.cpp -o $(BIN_DIR)/test_hashtable
	./$(BIN_DIR)/test_hashtable
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/Plate.cpp $(SRC_DIR)/HashTable.cpp $(SRC_DIR)/FileIO.cpp tests/test_fileio.cpp -o $(BIN_DIR)/test_fileio
	./$(BIN_DIR)/test_fileio
	@rm -f t1.txt t2.txt t3.txt t6.txt
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/Plate.cpp $(SRC_DIR)/HashTable.cpp $(SRC_DIR)/AdministrativeFilter.cpp tests/test_administrative_filter.cpp -o $(BIN_DIR)/test_administrative_filter
	./$(BIN_DIR)/test_administrative_filter
	@echo "==> Toàn bộ test cases đều đã PASS thành công!"

clean:
	@echo "==> Dọn dẹp tệp tin build..."
	rm -rf $(BIN_DIR)
	@rm -f t1.txt t2.txt t3.txt t6.txt

help:
	@echo "CÁC LỆNH MAKE:"
	@echo "  make        : Biên dịch chương trình vào bin/main"
	@echo "  make run    : Biên dịch và chạy ngay"
	@echo "  make test   : Biên dịch và thực thi toàn bộ test cases"
	@echo "  make clean  : Xóa thư mục bin/ và tệp tin tạm"
