#include "shmem.hh"
#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " (shmem|shmem_view)" << std::endl;
        return 1;
    }

    char str[] = "test";
    void* shared_address = (void*)0x3f0000;
    if (strcmp(argv[1], "shmem") == 0) {
        shmem s("aether_ro", 0x1000, shared_address);
        printf("writing into address %p\n", (void*)s);

        memcpy(s, str, sizeof(str));

        sleep(2);
    } else if (strcmp(argv[1], "shmem_view") == 0) {
        sleep(1);

        shmem_view s("aether_ro", 0x1000, shared_address);
        printf("reading from address %p\n", (void*)s);

        if (memcmp(s, str, sizeof(str)) == 0) {
            std::cout << "good" << std::endl;
        } else {
            std::cout << "bad" << std::endl;
        }
    } else {
        std::cerr << "usage: " << argv[0] << "(a|b)" << std::endl;
        return 1;
    }
    return 0;
}
