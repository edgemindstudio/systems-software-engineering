#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

class FileDescriptor {
public:
    explicit FileDescriptor(int fd = -1) noexcept : fd_(fd) {}

    ~FileDescriptor() {
        if (fd_ >= 0) {
            ::close(fd_);
        }
    }

    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;

    FileDescriptor(FileDescriptor&& other) noexcept : fd_(other.release()) {}

    FileDescriptor& operator=(FileDescriptor&& other) noexcept {
        if (this != &other) {
            reset(other.release());
        }
        return *this;
    }

    [[nodiscard]] int get() const noexcept { return fd_; }
    [[nodiscard]] bool valid() const noexcept { return fd_ >= 0; }

    int release() noexcept {
        int old = fd_;
        fd_ = -1;
        return old;
    }

    void reset(int replacement = -1) noexcept {
        if (fd_ >= 0) {
            ::close(fd_);
        }
        fd_ = replacement;
    }

private:
    int fd_;
};

int main() {
    const std::string path = "/tmp/cpp-raii-fd-demo.txt";
    int raw = ::open(path.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0600);
    if (raw < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    {
        FileDescriptor fd(raw);
        const char message[] = "raii-resource-ok\n";
        if (::write(fd.get(), message, sizeof(message) - 1U) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }
        std::cout << "fd_valid_inside_scope=" << (fd.valid() ? "yes" : "no") << '\n';
    }

    errno = 0;
    const int rc = ::fcntl(raw, F_GETFD);
    std::cout << "raw_fd_closed_after_scope="
              << (rc == -1 && errno == EBADF ? "yes" : "no") << '\n';

    ::unlink(path.c_str());
    return 0;
}
