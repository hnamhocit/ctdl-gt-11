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

.PHONY: all clean run setup help

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

clean:
	@echo "==> Dọn dẹp tệp tin build..."
	rm -rf $(BIN_DIR)

help:
	@echo "CÁC LỆNH MAKE:"
	@echo "  make        : Biên dịch chương trình vào bin/main"
	@echo "  make run    : Biên dịch và chạy ngay"
	@echo "  make clean  : Xóa thư mục bin/"
