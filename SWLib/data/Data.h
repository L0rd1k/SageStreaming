#pragma once

#include <vector>
#include <memory>
#include "definitions/LocalDefinitions.h"

struct DataHeader {
    DataHeader(DataType type) : 
    type(type),
    size(0) {
    };
    DataType type = DataType::Undefined;
    uint32_t size = 0;
};

template <typename T>
struct DataCore {
public:
    DataCore() {
    }

    DataCore(std::vector<unsigned char> data) {
        _data = std::make_shared<std::vector<unsigned char> >(data);
    }

    unsigned char* dataRaw() const {
        return _data->data();
    }

    DataCore& operator=(DataCore obj) {
        _data = obj._data;
        return *this;
    }

    T* operator->() const {
        return (T*)_data->data();
    }

    unsigned char* data() const {
        return _data->data() + _headSize;
    }

    int dataSize() const {
        return header().size - _headSize;
    }

    const int dataRawSize() const {
        return header().size;
    }

    T& header() const {
        return *((T*)_data->data());
    }

    const int headerSize() const {
        return _headSize;
    }


    void memAllocate(int size) {
        int requiredSize = size + _headSize;
        if(requiredSize > (int)_data->size()) {
            T headerBackup;
            if((int)_data->size() >= headerSize()) {
                headerBackup = header();
            }
            _data->resize(_headSize + size);
            header() = headerBackup;
        }
        header().size = requiredSize;
    }

    void setBytes(unsigned char* bytes, uint32_t size) {
        memAllocate(size);
        memcpy(data(), bytes, size);
    }

private:
    const uint32_t _headSize = sizeof(T);
    std::shared_ptr<std::vector<unsigned char> > _data = 
        std::make_shared<std::vector<unsigned char> >();
};
