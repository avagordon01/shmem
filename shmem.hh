#include <cassert>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>

class shmem {
    std::string path;
    size_t size;
    void* address;

public:
    shmem(std::string path_, size_t size_, void* address_ = nullptr):
        path(path_), size(size_) {
        int fd = shm_open(path.c_str(), O_CREAT | O_EXCL | O_RDWR, S_IRWXU);
        assert(fd && "failed to shm_open");
        int ret = ftruncate(fd, size);
        assert(!ret && "failed to ftruncate");
        address = mmap(address_, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    }
    void* addr() {
        return address;
    }
    ~shmem() {
        int ret = munmap(address, size);
        assert(!ret);
        ret = shm_unlink(path.c_str());
        assert(!ret);
    }
};

class shmem_view {
    std::string path;
    size_t size;
    void* address;

public:
    shmem_view(std::string path_, size_t size_, void* address_ = nullptr):
        path(path_), size(size_) {
        int fd = shm_open(path.c_str(), O_RDONLY, S_IRWXU);
        assert(fd && "failed to shm_open");
        address = mmap(address_, size, PROT_READ, MAP_SHARED, fd, 0);
    }
    void* addr() {
        return address;
    }
    ~shmem_view() {
        int ret = munmap(address, size);
        assert(!ret);
    }
};
