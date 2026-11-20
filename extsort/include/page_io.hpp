#pragma once
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <fcntl.h>
#include <type_traits>
#include <unistd.h>
#include <system_error>

inline constexpr std::size_t PAGE_SIZE = 8192;

using Value = std::int64_t;

inline constexpr std::size_t MAX_ELEMENTS = PAGE_SIZE / sizeof(Value);

struct Page {
    Value values[MAX_ELEMENTS];
};

static_assert(std::is_trivially_copyable_v<Page>); // helps read bytes

class FileHandle {
    int fd_ = -1;

public:
   FileHandle(const char* path, int flags, mode_t mode = 0644): fd_(::open(path, flags, mode))
   {
        if (fd_ < 0)
            throw std::system_error(errno, std::generic_category(), path);
   }

   ~FileHandle() noexcept {
        try {
            close();
        } catch (...) {
        }
    }

    int release() noexcept {
        int fd = fd_;
        fd_ = -1;
        return fd;
    }

    int get() const noexcept {
        return fd_;
    }

   FileHandle(const FileHandle&) = delete;
   FileHandle& operator=(const FileHandle&) = delete;

   FileHandle(FileHandle&& other) noexcept : fd_(other.release()){}

   FileHandle& operator=(FileHandle&& other) noexcept{
        if(this != &other){
            close();
            fd_ = other.release();
        }
        return *this;
   }

    void close() {
        if (fd_ >= 0) {
            if (::close(fd_) < 0)
                throw std::system_error(errno, std::generic_category(), "close");
            else
                fd_ = -1;
        }
    }
};


