#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>

class Buffer {
public:
    explicit Buffer(std::size_t size)
        : size_(size), data_(std::make_unique<int[]>(size)) {}

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept
        : size_(std::exchange(other.size_, 0U)),
          data_(std::move(other.data_)) {}

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            size_ = std::exchange(other.size_, 0U);
            data_ = std::move(other.data_);
        }
        return *this;
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0U; }

    int& operator[](std::size_t index) { return data_[index]; }
    const int& operator[](std::size_t index) const { return data_[index]; }

private:
    std::size_t size_;
    std::unique_ptr<int[]> data_;
};

int main() {
    Buffer source(4U);
    source[0] = 11;
    source[1] = 22;
    source[2] = 33;
    source[3] = 44;

    Buffer destination(std::move(source));

    std::cout << "source_empty_after_move=" << (source.empty() ? "yes" : "no") << '\n';
    std::cout << "destination_size=" << destination.size() << '\n';
    std::cout << "destination_values="
              << destination[0] << ','
              << destination[1] << ','
              << destination[2] << ','
              << destination[3] << '\n';

    return 0;
}
