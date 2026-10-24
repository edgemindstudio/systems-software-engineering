#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <system_error>

class UniqueFd {
public:
    explicit UniqueFd(int fd = -1) noexcept : fd_(fd) {}
    ~UniqueFd() { if (fd_ >= 0) ::close(fd_); }

    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;

    UniqueFd(UniqueFd&& other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
    UniqueFd& operator=(UniqueFd&& other) noexcept {
        if (this != &other) {
            if (fd_ >= 0) ::close(fd_);
            fd_ = other.fd_;
            other.fd_ = -1;
        }
        return *this;
    }

    [[nodiscard]] int get() const noexcept { return fd_; }

private:
    int fd_;
};

static UniqueFd open_readonly(const char* path) {
    int fd = ::open(path, O_RDONLY);
    if (fd < 0) {
        throw std::system_error(errno, std::generic_category(), "open");
    }
    return UniqueFd(fd);
}

int main() {
    const char* good = "/etc/hosts";
    const char* bad = "/definitely/not/a/real/path/cpp-systems";

    try {
        UniqueFd fd = open_readonly(good);
        std::cout << "good_open_fd_nonnegative=" << (fd.get() >= 0 ? "yes" : "no") << '\n';
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }

    try {
        [[maybe_unused]] UniqueFd fd = open_readonly(bad);
        std::cout << "unexpected_success=yes\n";
        return 1;
    } catch (const std::system_error& ex) {
        std::cout << "bad_open_caught_system_error=yes\n";
        std::cout << "error_value=" << ex.code().value() << '\n';
    }

    std::cout << "exception_path_survived=yes\n";
    return 0;
}
