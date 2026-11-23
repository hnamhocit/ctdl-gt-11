# ==============================================================================
# Makefile - Đề Tài 11 (CTDL&GT): Quản Lý Biển Số Xe
# ==============================================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude
SRC_DIR  := src
INC_DIR  := include
BIN_DIR  := bin
DATA_DIR := $(CURDIR)/data
DATA_CPPFLAGS := -DLICENSE_PLATE_DATA_DIR='"$(DATA_DIR)"'

TARGET   := $(BIN_DIR)/main

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

.PHONY: all clean run setup test benchmark stress valgrind help

all: setup $(TARGET)

setup:
	@mkdir -p $(BIN_DIR)
	@mkdir -p "$(DATA_DIR)"

$(TARGET): $(OBJS)
	@echo "==> Đang liên kết: $(TARGET)..."
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "==> Biên dịch thành công! Chạy bằng lệnh: ./$(TARGET)"

$(BIN_DIR)/main.o: CPPFLAGS += $(DATA_CPPFLAGS)
$(BIN_DIR)/main.o: Makefile

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | setup
	@echo "==> Biên dịch: $<"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: all
	@echo "==> Khởi chạy: ./$(TARGET)"
	./$(TARGET)

test: setup
	@echo "==> Biên dịch và chạy bộ kiểm thử Unit Tests..."
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/Plate.cpp $(SRC_DIR)/HashTable.cpp tests/test_hashtable.cpp -o $(BIN_DIR)/test_hashtable
	./$(BIN_DIR)/test_hashtable
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/Plate.cpp $(SRC_DIR)/HashTable.cpp tests/test_proj7.cpp -o $(BIN_DIR)/test_proj7
	./$(BIN_DIR)/test_proj7
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/Plate.cpp $(SRC_DIR)/HashTable.cpp $(SRC_DIR)/Validator.cpp tests/test_proj11.cpp -o $(BIN_DIR)/test_proj11
	./$(BIN_DIR)/test_proj11
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

benchmark: setup
	@echo "==> Biên dịch và chạy Benchmark hiệu năng (PROJ-17)..."
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/Plate.cpp $(SRC_DIR)/HashTable.cpp $(SRC_DIR)/Benchmark.cpp tests/benchmark_hash_table.cpp -o $(BIN_DIR)/benchmark_hash_table
	./$(BIN_DIR)/benchmark_hash_table

stress: setup
	@echo "==> Biên dịch và chạy Stress Test FileIO (PROJ-14)..."
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/Plate.cpp $(SRC_DIR)/HashTable.cpp $(SRC_DIR)/FileIO.cpp tests/test_fileio_stress.cpp -o $(BIN_DIR)/test_fileio_stress
	./$(BIN_DIR)/test_fileio_stress

valgrind: setup
	@echo "==> Kiểm tra an toàn bộ nhớ Valgrind cho Unit Tests (PROJ-17)..."
	$(CXX) -std=c++17 -g -O0 -Iinclude $(SRC_DIR)/Plate.cpp $(SRC_DIR)/HashTable.cpp $(SRC_DIR)/Validator.cpp tests/test_proj11.cpp -o $(BIN_DIR)/test_proj11
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN_DIR)/test_proj11

help:
	@echo "CÁC LỆNH MAKE:"
	@echo "  make        : Biên dịch chương trình vào bin/main"
	@echo "  make run    : Biên dịch và chạy ngay"
	@echo "  make test   : Biên dịch và thực thi toàn bộ test cases"
	@echo "  make benchmark : Biên dịch và đo đạc hiệu năng bảng băm (100 - 10.000 mẫu)"
	@echo "  make stress : Chạy kiểm thử tải trọng lớn cho FileIO (10.000 mẫu)"
	@echo "  make clean  : Xóa thư mục bin/ và tệp tin tạm"
