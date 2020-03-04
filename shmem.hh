#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>

class shmem {
    int fd;
    std::string path;
    void* address;

public:
    size_t size;
    shmem(std::string path_, size_t size_, void* address_ = nullptr):
        path(path_) {
        fd = shm_open(path.c_str(), O_CREAT | O_EXCL | O_RDWR, S_IRWXU);
        if (fd < 0) {
            perror("shm_open");
            abort();
        }
        resize(size_);
        address = mmap(address_, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (address == MAP_FAILED) {
            perror("mmap");
            abort();
        }
        fprintf(stderr, "shmem fd %i address %p\n", fd, address);
    }
    void resize(size_t size_) {
        int ret = ftruncate(fd, size_);
        if (ret) {
            perror("ftruncate");
            abort();
        }
        fprintf(stderr, "ftruncate fd %i size %zu\n", fd, size_);
        size = size_;
    }
    operator void*() {
        return address;
    }
    ~shmem() {
        int ret = munmap(address, size);
        if (ret == -1) {
            perror("munmap");
            abort();
        }
        ret = shm_unlink(path.c_str());
        if (ret == -1) {
            perror("shm_unlink");
            abort();
        }
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
        if (fd < 0) {
            perror("shm_open");
            abort();
        }
        address = mmap(address_, size, PROT_READ, MAP_SHARED, fd, 0);
    }
    operator void*() {
        return address;
    }
    ~shmem_view() {
        int ret = munmap(address, size);
        if (ret == -1) {
            perror("munmap");
            abort();
        }
    }
};

template <class T>
class shmem_allocator {
    shmem s = {"alloc", 0, (void*)0x5f0000};

public:
    using value_type    = T;

//     using pointer       = value_type*;
//     using const_pointer = typename std::pointer_traits<pointer>::template
//                                                     rebind<value_type const>;
//     using void_pointer       = typename std::pointer_traits<pointer>::template
//                                                           rebind<void>;
//     using const_void_pointer = typename std::pointer_traits<pointer>::template
//                                                           rebind<const void>;

//     using difference_type = typename std::pointer_traits<pointer>::difference_type;
//     using size_type       = std::make_unsigned_t<difference_type>;

//     template <class U> struct rebind {typedef shmem_allocator<U> other;};

    shmem_allocator() noexcept {
        fprintf(stderr, "shmem_allocator ctor\n");
    }  // not required, unless used
    template <class U>
    shmem_allocator(shmem_allocator<U> const&) noexcept {
        fprintf(stderr, "shmem_allocator ctor\n");
    }

    value_type* // Use pointer if pointer is not a value_type*
    allocate(std::size_t n) {
        fprintf(stderr, "shmem_allocator allocate\n");
        return static_cast<value_type*>(::operator new (n*sizeof(value_type)));
        size_t offset = s.size;
        s.resize(s.size + sizeof(value_type) * n);
        return static_cast<value_type*>(((void*)s) + offset);
    }

    void deallocate(value_type* p, std::size_t) noexcept {// Use pointer if pointer is not a value_type*
        //::operator delete(p);
    }

//     value_type*
//     allocate(std::size_t n, const_void_pointer)
//     {
//         return allocate(n);
//     }

//     template <class U, class ...Args>
//     void
//     construct(U* p, Args&& ...args)
//     {
//         ::new(p) U(std::forward<Args>(args)...);
//     }

//     template <class U>
//     void
//     destroy(U* p) noexcept
//     {
//         p->~U();
//     }

//     std::size_t
//     max_size() const noexcept
//     {
//         return std::numeric_limits<size_type>::max();
//     }

//     shmem_allocator
//     select_on_container_copy_construction() const
//     {
//         return *this;
//     }

//     using propagate_on_container_copy_assignment = std::false_type;
//     using propagate_on_container_move_assignment = std::false_type;
//     using propagate_on_container_swap            = std::false_type;
//     using is_always_equal                        = std::is_empty<shmem_allocator>;
};

template <class T, class U>
bool operator==(shmem_allocator<T> const&, shmem_allocator<U> const&) noexcept {
    return false;
}

template <class T, class U>
bool operator!=(shmem_allocator<T> const& x, shmem_allocator<U> const& y) noexcept {
    return !(x == y);
}
