#include <fstream>
#include <vector>
#include <algorithm>

#define NMAX 25
#define MMAX 10005
#define INPUT_FILE "curse.in"
#define OUTPUT_FILE "curse.out"

int n, m, a;
std::vector<int> adj[MMAX];
std::vector<int> topsort;
bool visited[MMAX];

void read_and_construct() {
    std::ifstream fin(INPUT_FILE);
    fin >> n >> m >> a;
    std::vector<int> first(n + 1, 0),
                     second(n + 1, 0);
    // read the first training sessions
    for (int i = 0; i < n; ++i) {
        fin >> first[i];
    }
    // Go through trainings
    for (int i = 1; i < a; ++i) {
        bool found = false;
        for (int j = 0; j < n; ++j) {
            fin >> second[j];
            if (first[j] != second[j] && found == false) {
                // create edge
                adj[first[j]].push_back(second[j]);
                found = true;
            }
        }
        first = second;
    }
    fin.close();
}

void dfs(int node) {
    visited[node] = true;
    for (auto &child : adj[node]) {
        if (!visited[child]) {
            dfs(child);
        }
    }
    topsort.push_back(node);
}

void topo_sort() {
    for (int i = 1; i <= m; ++i) {
        if (visited[i] == false)
            dfs(i);
    }
    std::reverse(topsort.begin(), topsort.end());
}

void print_result() {
    std::ofstream fout(OUTPUT_FILE);
    for (int &it : topsort)
        fout << it << " ";
    fout.close();
}

void solve() {
    read_and_construct();
    topo_sort();
    print_result();
}

int main() {
    solve();
    return 0;
}
