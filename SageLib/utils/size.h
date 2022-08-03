#pragma once

namespace sage {

template <typename T>
class Size {
public:
    Size() = default;

    Size(T width, T height) {
        _width = width;
        _height = height;
    }

    Size(const Size& obj) {
        _width = obj.width();
        _height = obj.height();
    }

    Size& operator=(const Size& obj) {
        _width = obj.width();
        _height = obj.height();
        return *this;
    }

    void setWidth(T width) {
        _width = width;
    }

    void setHeight(T height) {
        _height = height;
    }

    const T& width() const {
        return _width;
    }

    const T& height() const {
        return _height;
    }

    bool isValid() {
        return (_height > 0) || (_width > 0);
    }

    std::string toStr() {
        return std::to_string(_width) + "x" + std::to_string(_height);
    }

private:
    T _width;
    T _height;
};

}  // namespace sage