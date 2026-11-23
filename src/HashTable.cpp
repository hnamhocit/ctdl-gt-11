#include "HashTable.h"

LicensePlateHashTable::LicensePlateHashTable() : totalCount(0) {
  for (int i = 0; i < TABLE_SIZE; ++i) {
    buckets[i] = nullptr;
  }
}

LicensePlateHashTable::~LicensePlateHashTable() { clear(); }

int LicensePlateHashTable::hashFunction(const char *plate) const {
  if (!plate || plate[0] == '\0')
    return 0;

  unsigned long hash = 5381;
  int c;
  while ((c = static_cast<unsigned char>(*plate++))) {
    // (hash << 5) tương đương hash * 32, cộng thêm hash thành hash * 33
    hash = ((hash << 5) + hash) + static_cast<unsigned long>(c);
  }

  return static_cast<int>(hash % TABLE_SIZE);
}

bool LicensePlateHashTable::insertPlate(const PlateNode &newPlate) {
  if (newPlate.fullPlate[0] == '\0')
    return false;

  int index = hashFunction(newPlate.fullPlate);

  // Tìm biển trùng trong danh sách của bucket này.
  PlateNode *current = buckets[index];
  while (current != nullptr) {
    if (std::strcmp(current->fullPlate, newPlate.fullPlate) == 0) {
      return false; // Biển số đã tồn tại trong hệ thống
    }
    current = current->next;
  }

  // Nối node mới với đầu cũ rồi cập nhật đầu bucket.
  PlateNode *newNode = new PlateNode(newPlate);
  newNode->next = buckets[index];
  buckets[index] = newNode;
  totalCount++;

  return true;
}

PlateNode *LicensePlateHashTable::searchPlate(const char *plate) const {
  if (!plate || plate[0] == '\0')
    return nullptr;

  int index = hashFunction(plate);
  PlateNode *current = buckets[index];

  while (current != nullptr) {
    if (std::strcmp(current->fullPlate, plate) == 0) {
      return current;
    }
    current = current->next;
  }

  return nullptr;
}

bool LicensePlateHashTable::removePlate(const char *plate) {
  if (!plate || plate[0] == '\0')
    return false;

  int index = hashFunction(plate);
  PlateNode *current = buckets[index];
  PlateNode *previous = nullptr;

  while (current != nullptr) {
    if (std::strcmp(current->fullPlate, plate) == 0) {
      if (previous == nullptr) {
        // Dời đầu bucket sang node kế tiếp.
        buckets[index] = current->next;
      } else {
        // Nối node trước qua node cần xóa.
        previous->next = current->next;
      }

      delete current;
      totalCount--;
      return true;
    }
    previous = current;
    current = current->next;
  }

  return false;
}

void LicensePlateHashTable::clear() {
  for (int i = 0; i < TABLE_SIZE; ++i) {
    PlateNode *current = buckets[i];
    while (current != nullptr) {
      PlateNode *temp = current;
      current = current->next; // Giữ node kế tiếp trước khi xóa.
      delete temp;
    }
    buckets[i] = nullptr;
  }
  totalCount = 0;
}

int LicensePlateHashTable::getTotalCount() const { return totalCount; }

double LicensePlateHashTable::getLoadFactor() const {
  return static_cast<double>(totalCount) / static_cast<double>(TABLE_SIZE);
}

int LicensePlateHashTable::getBucketSize() const { return TABLE_SIZE; }

PlateNode *LicensePlateHashTable::getBucket(int index) const {
  if (index < 0 || index >= TABLE_SIZE)
    return nullptr;
  return buckets[index];
}
