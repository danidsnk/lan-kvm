#include "filedescriptor.h"
#include <fcntl.h>
#include <unistd.h>

namespace device::utils {

file_descriptor::file_descriptor(const std::string &path, int flags) {
    if (fd_ = open(path.c_str(), flags); fd_ == -1) {
        throw opening_error();
    }
}

file_descriptor::file_descriptor(file_descriptor &&other) noexcept
    : fd_(other.fd_) {
    other.fd_ = -1;
}

file_descriptor &file_descriptor::operator=(file_descriptor &&other) noexcept {
    if (this != &other) {
        close();
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

file_descriptor::~file_descriptor() { close(); }

file_descriptor::operator int() const { return fd_; }

void file_descriptor::close() const {
    if (fd_ != -1) {
        ::close(fd_);
    }
}

} // namespace device::utils
