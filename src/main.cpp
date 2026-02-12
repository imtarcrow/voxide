#include <print>

int main(int argc, char** argv) {
    std::println("Argument Count: {}", argc);

    for(int i = 1; i <= argc; i++) {
        std::println("{}. {}", i, argv[i - 1]);
    }

    return 0;
}
