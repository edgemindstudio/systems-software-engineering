#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

class SmallVector {
public:
    SmallVector() = default;

    void push_back(int value) {
        if (size_ == capacity_) {
            grow();
        }
        data_[size_] = value;
        ++size_;
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] std::size_t capacity() const noexcept { return capacity_; }

    int& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("SmallVector::at");
        }
        return data_[index];
    }

    const int& at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("SmallVector::at");
        }
        return data_[index];
    }

private:
    void grow() {
        const std::size_t new_capacity = capacity_ == 0U ? 4U : capacity_ * 2U;
        auto replacement = std::make_unique<int[]>(new_capacity);
        for (std::size_t i = 0; i < size_; ++i) {
            replacement[i] = data_[i];
        }
        data_ = std::move(replacement);
        capacity_ = new_capacity;
    }

    std::unique_ptr<int[]> data_;
    std::size_t size_ = 0U;
    std::size_t capacity_ = 0U;
};

int main() {
    SmallVector values;
    for (int i = 1; i <= 10; ++i) {
        values.push_back(i * 10);
    }

    std::cout << "size=" << values.size() << '\n';
    std::cout << "capacity=" << values.capacity() << '\n';
    std::cout << "first=" << values.at(0U) << '\n';
    std::cout << "last=" << values.at(9U) << '\n';

    try {
        (void)values.at(10U);
        std::cout << "bounds_check_failed=yes\n";
        return 1;
    } catch (const std::out_of_range&) {
        std::cout << "bounds_exception_caught=yes\n";
    }

    std::cout << "container_invariants_verified=yes\n";
    return 0;
}
