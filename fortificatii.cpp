#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm>

#define NMAX 100005
#define INPUT_FILE "fortificatii.in"
#define OUTPUT_FILE "fortificatii.out"

struct NodeDist {
    long long dist;
    int vertex;
    NodeDist(const long long &dist, const int &vertex) {
        this->dist = dist;
        this->vertex = vertex;
    }

    bool operator < (const NodeDist &other) const {
        return this->dist > other.dist;
    }
};

// global variables
int n, m, k, b;
std::vector<NodeDist > graph[NMAX];
std::vector<long long> distances;
std::unordered_set<int> enemies;

void read_input() {
    std::ifstream fin(INPUT_FILE);
    // read first line
    fin >> n >> m >> k;

    int i, x, y;
    long long dist;
    // read second line
    fin >> b;
    for (i = 0; i< b; ++i) {
        fin >> x;
        enemies.insert(x);
    }

    for (i = 0; i < m; ++i) {
        // create the graph
        fin >> x >> y >> dist;
        graph[x].push_back({dist, y});
        graph[y].push_back({dist, x});
    }
    fin.close();
}

void get_relevant_distances() {
    // use Dijkstra to get routes to the enemy camps and their distance
    std::priority_queue<NodeDist> pq;
    std::vector<bool> visited(n+1, false);
    pq.push({0, 1});

    while (!pq.empty()) {
        long long vertex_distance = pq.top().dist;
        int vertex = pq.top().vertex;
        pq.pop();
        if (visited[vertex]) {
            continue;
        }
        visited[vertex] = true;

        for (auto &child : graph[vertex]) {
            if (enemies.count(child.vertex)) {  // found a route to an enemy
                distances.push_back(vertex_distance + child.dist);
            } else if (!visited[child.vertex]) {
                pq.push({vertex_distance + child.dist, child.vertex});
            }
        }
    }
}

long long get_minimum_time() {
    int size = distances.size();
    if (size == 1)  // a single route to the enemy camp
        return distances[0] * 1LL + k;
    // sort the distances in ascending order
    std::sort(distances.begin(), distances.end());
    // get the minimum time in a greedy fashion
    for (size_t i = 1; i < distances.size(); ++i) {
        long long difference = distances[i] - distances[i-1];
        if (difference == 0)
            continue;
        long long can_add = k / i;
        if (can_add < difference)
            return distances[i-1] * 1LL + can_add;
        k -= (difference * i);
        if (k <= 0)
            return distances[i];
    }
    // brought them all to equal level, add how much I can to all of them
    return distances[size-1] *1LL + (k / size);
}


void output_result(long long res) {
    std::ofstream fout(OUTPUT_FILE);
    fout << res;
    fout.close();
}

void solve() {
    read_input();
    get_relevant_distances();
    long long res = get_minimum_time();
    output_result(res);
}

int main() {
    solve();
    return 0;
}
