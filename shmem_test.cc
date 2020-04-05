#include "shmem.hh"
#include <iostream>
#include <cstring>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " (shmem|shmem_view)" << std::endl;
        return 1;
    }

    int fd = open("lock", O_CREAT | O_EXCL, S_IRWXU);
    if (fd == -1 && errno == EEXIST) {
    } else if (fd == -1) {
    } else {
    }

    if (strcmp(argv[1], "shmem") == 0) {
        std::vector<char, shmem_allocator<char>> v;
        v.push_back('t');
        v.push_back('e');
        v.push_back('s');
        v.push_back('t');

        sleep(2);
    } else if (strcmp(argv[1], "shmem_view") == 0) {
        sleep(1);

        char str[] = "test";
        shmem_view sv("aether_alloc", (void*)0x5f0000);
        fprintf(stderr, "%s\n", (char*)(void*)sv);

        if (memcmp((void*)sv, str, sizeof(str)) == 0) {
            std::cout << "good" << std::endl;
        } else {
            std::cout << "bad" << std::endl;
        }
        std::cout << "contents: \"" << (char*)(void*)sv << "\"" << std::endl;
    } else {
        std::cerr << "usage: " << argv[0] << "(a|b)" << std::endl;
        return 1;
    }
    return 0;
}
