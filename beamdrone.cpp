#include <fstream>
#include <iostream>
#include <queue>
#include <cmath>
#include <set>
#include <map>

#define NMAX 1005
#define INPUT_FILE "beamdrone.in"
#define OUTPUT_FILE "beamdrone.out"

int grid[NMAX][NMAX];
int delta_x[4] = {-1, 0, 1, 0};  // offsets to help get the neighbours
int delta_y[4] = {0, 1, 0, -1};  // offsets to help get the neighbours

int n, m, startX, startY, stopX, stopY;

void read_input() {
    std::ifstream fin(INPUT_FILE);
    fin >> n >> m;
    fin >> startX >> startY >> stopX >> stopY;

    char c;
    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            fin >> c;
            if(c == '.')
                grid[i][j] = -1;
            else
                grid[i][j] = -2;
        }
    }
    grid[startX][startY] = 0;
    grid[stopX][stopY] = -3;

    fin.close();
}

bool check_boundaries(int x, int y) {
    if(x < 0 || y < 0 || x >= n || y >= m)
        return false;
    return true;
}

void print_result(int result) {
    std::ofstream fout(OUTPUT_FILE);
    fout << result;
    fout.close();
}

struct Node{
    int x, y, dist, dir;

    Node(const int &x, const int &y, const int &dist,
        const int &dir) {
        this->x = x;
        this->y = y;
        this->dist = dist;
        this->dir = dir;
    }
};

int modified_lee() {
    std::queue<Node> q;
    q.push({startX, startY, 0, -1});

    while (!q.empty()) {
        int current_x = q.front().x;
        int current_y = q.front().y;
        int current_dist = q.front().dist;
        int dir = q.front().dir;

        q.pop();
        for (int k = 0; k < 4; ++k) {
            if (dir != -1) {
                if (dir == k || dir == ((k + 2) % 4))
                    continue;
            }
            // go as long as you can in one direction
            int next_x = current_x + delta_x[k];
            int next_y = current_y + delta_y[k];

            // mark the direction as visited

            while (check_boundaries(next_x, next_y) 
                    && grid[next_x][next_y] != -2) {
                    
                // check if end
                if(next_x == stopX && next_y == stopY) {
                    return current_dist;
                }                

                // set the distance
                if (grid[next_x][next_y] == -1) {
                    grid[next_x][next_y] = current_dist;
                    q.push({next_x, next_y, current_dist + 1, k});
                }
                else {
                    if (current_dist < grid[next_x][next_y]) {
                        grid[next_x][next_y] = current_dist;
                        q.push({next_x, next_y, current_dist + 1, k});
                    }
                    
                }
                
                // get the next element
                next_x = next_x + delta_x[k];
                next_y = next_y + delta_y[k];
            }
        }
    }
    return -1;
}

void solve() {
    read_input();
    int result = modified_lee();
    print_result(result);
}

int main() {
    solve();
    return 0;
}
