#ifndef FILEDESCRIPTOR_H
#define FILEDESCRIPTOR_H

#include <stdexcept>
#include <string>

namespace device::utils {

class file_descriptor {
public:
    struct opening_error : std::runtime_error {
        opening_error() : std::runtime_error("Failed to open file descriptor") {}
    };

    file_descriptor() = default;
    file_descriptor(const std::string &path, int flags);
    file_descriptor(const file_descriptor &) = delete;
    file_descriptor &operator=(const file_descriptor &) = delete;
    file_descriptor(file_descriptor &&other) noexcept;
    file_descriptor &operator=(file_descriptor &&other) noexcept;
    ~file_descriptor();
    operator int() const;

private:
    void close() const;
    int fd_ = -1;
};

} // namespace device::utils

#endif
