#include <fstream>
#include <iostream>
#include <new>

// numarul maxim de noduri
#define NMAX 105

class Task {
public:
    void solve() {
        read_input();
    }

private:
    int n;

    void read_input() {
        std::ifstream fin("in");
        fin >> n;


        fin.close();
    }

};

int main() {
    auto* task = new (std::nothrow) Task(); // hint: cppreference/nothrow
    if (!task) {
        std::cerr << "new failed: WTF are you doing? Throw your PC!\n";
        return -1;
    }
    task->solve();
    delete task;
    return 0;
}
