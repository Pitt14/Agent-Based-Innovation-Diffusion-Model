#include "../CppProgressBar.hpp"
#include <thread>

int main() {
    using namespace std::chrono_literals;
    int steps = 1000;
    auto bar = ProgressBar(steps);
    for (int i = 0; i < steps; i++) {
        bar.tick();
        std::this_thread:: sleep_for(2ms);
    }

    return 0;
}