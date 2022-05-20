#include <fstream>
#include <iostream>
#include <new>
#include <vector>
#include <queue>
#include <stdio.h>
#include <string.h>
#include <map>
#include <set>

// numarul maxim de noduri
#define NMAX 1005
#define MAXP 9
#define INPUT_FILE "curatare.in"
#define OUTPUT_FILE "curatare.out"

int n, m;
int grid[NMAX][NMAX], auxGrid[NMAX][NMAX];
int delta_x[4] = {-1,0,1,0};  // offsets to help get the neighbours
int delta_y[4] = {0,1,0,-1};  // offsets to help get the neighbours

std::vector<std::pair<int,int> > robots, to_clean, all;
std::map<std::pair<int,int>, int> point_to_index;
int min_distances[MAXP][MAXP];

std::vector<int> robot_index;
std::vector<int> zones_index;


void read_input() {
    std::ifstream fin(INPUT_FILE);
    fin >> n >> m;
    int i, j, k = 0;
    char current_character;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < m; ++j) {
            fin >> current_character;  // get the current char
            if (current_character == '.') {
                grid[i][j] = -1;
            }
            else if (current_character == 'R') {
                robots.push_back({i,j});
                all.push_back({i,j});
                grid[i][j] = 0;
                point_to_index[{i,j}] = k++;
            }
            else if (current_character == 'S') {
                to_clean.push_back({i,j});
                all.push_back({i,j});
                grid[i][j] = 0;
                point_to_index[{i,j}] = k++;
            }
            else {
                grid[i][j] = -2;
            }
        }
    }

    fin.close();
}

bool check_bounds(int i, int j, int x, int y) {
    if(i == x && j == y)
        return false;
    if(i < 0 || j < 0 || i >= n || j >= m)
        return false;
    return true;
}

void lee_algorithm(std::pair<int,int> &point) {
    std::queue<std::pair<int,int> > q;
    int next_i, next_j, i, j, k, x = point.first, y = point.second;
    q.push(point);

    while (!q.empty()) {
        i = q.front().first;
        j = q.front().second;
        q.pop();
        for (k = 0; k < 4; ++k) {
            next_i = i + delta_x[k];
            next_j = j + delta_y[k];
            if (check_bounds(next_i, next_j, x, y) && 
                        (auxGrid[next_i][next_j] == 0 || auxGrid[next_i][next_j] == -1)) {
                if(auxGrid[next_i][next_j] == 0) { // if it is a point of interest
                    min_distances[point_to_index[{x,y}]][point_to_index[{next_i,next_j}]] =
                        1 + auxGrid[i][j];
                }
                auxGrid[next_i][next_j] = 1 + auxGrid[i][j];
                q.push({next_i,next_j});
            }
        }
    }
}

void calculate_min_distance_to_all(std::vector<std::pair<int,int> > &v) {
    int k, i, j, sz = v.size();
    for (k = 0; k < sz; ++k) {
        std::pair<int,int> curr = v[k];
        // reset the matrix;
        memcpy(auxGrid, grid, sizeof(grid));
        // calculate distance from this point to all other points
        lee_algorithm(curr);

    }
}

int calculate_min_time(int robot, int zones) {
    std::set<int> zones_to_go_to; 
    int point = 0, min_distance_so_far = INT32_MAX-1, total_num_of_points = all.size();
    for (int i = 0; i< (1 << to_clean.size()); ++i) {
        if ((1 << i) & zones) {
            zones_to_go_to.insert(zones_index[i]);
            if (min_distances[robot][zones_index[i]] < min_distance_so_far) {
                min_distance_so_far = min_distances[robot][zones_index[i]];
                point = zones_index[i];
            }
        }
    }
    if(zones_to_go_to.size() == 0)
        return 0;

    if(zones_to_go_to.find(point) != zones_to_go_to.end())
        zones_to_go_to.erase(zones_to_go_to.find(point));

    int res = min_distance_so_far, aux_point = 0;
    min_distance_so_far = INT32_MAX -1;
    
    while (!zones_to_go_to.empty()) {
        aux_point = -1;
        for (int i = 0; i < total_num_of_points; ++i) {
            if(zones_to_go_to.count(i)) {
                if(min_distances[point][i] < min_distance_so_far) {
                    min_distance_so_far = min_distances[point][i];
                    aux_point = i;
                }
            }
        }
        if(aux_point == -1)
            return -1;
        if(zones_to_go_to.find(point) != zones_to_go_to.end())
            zones_to_go_to.erase(zones_to_go_to.find(aux_point));
        point = aux_point;
        res += min_distance_so_far;
    }
    return (res == (INT32_MAX -1) ? 0 : res);
}


int start_backtrack() {
    int sol = INT32_MAX, number_of_robots = robots.size(), number_of_zones = to_clean.size();

    robot_index = std::vector<int> (number_of_robots + 1, 0);
    zones_index = std::vector<int> (number_of_zones + 1, 0);

    // associate robot index to the robots on the field
    for (int i = 0; i < number_of_robots;  ++i) {
        robot_index[i] = point_to_index[robots[i]];
    }

    for(int i = 0; i< number_of_zones; ++i) {
        zones_index[i] = point_to_index[to_clean[i]];
    }

    int robot, set_of_zones;
    std::vector<std::pair<int,int>> zones_for_robots;

    for(set_of_zones = 0; set_of_zones < (1 << (number_of_zones * number_of_robots)); ++set_of_zones) {
        if(set_of_zones == 6) {
            int x= 3;
        }
        int current_time = -1, zones_cleaned = 0, distinct = (1 << (number_of_zones) - 1);
        zones_for_robots.clear();
        for(robot = 0; robot < number_of_robots; ++robot) {
            int robot_index_on_map = robot_index[robot];

            int start_of_zone = robot * number_of_zones;

            int zones_cleaned_by_robot = set_of_zones >> start_of_zone;
            zones_cleaned_by_robot &= ((1 << number_of_zones) -1);

            zones_cleaned |= zones_cleaned_by_robot;
            distinct &= zones_cleaned_by_robot;
            zones_for_robots.push_back({robot_index_on_map,zones_cleaned_by_robot});

            
        }
        if (zones_cleaned == (1 << number_of_zones) - 1) {
            for (auto &it : zones_for_robots) {
                int current_min_time =  calculate_min_time(it.first, it.second);
                if(current_min_time != -1)
                    current_time = std::max(current_time, calculate_min_time(it.first, it.second));
                else {
                    current_time = INT32_MAX;
                    break;
                }
            }
            sol = std::min(sol, current_time);
        }
    }

    return sol;
}

void print_output(int res) {
    std::ofstream fout(OUTPUT_FILE);

    fout << res;
    fout.close();
}

void solve() {
    read_input();
    calculate_min_distance_to_all(all);
    int res = start_backtrack();
    print_output(res);
}


int main() {
    solve();
}