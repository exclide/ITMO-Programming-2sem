#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <ctime>

std::unordered_map<std::string, int> const map_sides{ {"up", 0}, {"front", 1}, {"left", 2},
                                                      {"right", 3}, {"down", 4}, {"back", 5} };
std::unordered_map<int, std::vector<int>> const changed_sides{ {0, {2, 5, 3, 1}}, //from left cw
                                                               {1, {2, 0, 3, 4}},
                                                               {2, {5, 0, 1, 4}},
                                                               {3, {1, 0, 5, 4}},
                                                               {4, {2, 1, 3, 5}},
                                                               {5, {3, 0, 2, 4}},
                                                               {6, {1, 0, 5, 4}},
                                                               {7, {1, 3, 5, 2}},
                                                               {8, {0, 3, 4, 2}} };
enum sides {
    up = 0, front = 1, left = 2, right = 3, down = 4, back = 5, front_vertical = 6,
    front_horizontal = 7, up_horizontal = 8, x_axis = 9, y_axis = 10, z_axis = 11,
    lw = 12, dw = 13
};
std::vector<std::string> const sides_vec{ "up", "front", "left", "right", "down", "back" };

std::vector<std::string> const single_moves{ "U", "U'", "U2", "D", "D'", "D2",
                                             "R", "R'", "R2", "L", "L'", "L2",
                                             "F", "F'", "F2", "B", "B'", "B2" };

std::vector<std::string> const full_rotations{ "x", "x'", "x2", "y", "y'", "y2" };

std::vector<std::string> const orientations{ "z", "z'", "z2" };

std::vector<std::string> const permutations{
        "F' L' B' R' U' R U' B L F R U R' U",
        "F R B L U L' U B' R' F' L' U' L U'",
        "U2 B U2 B' R2 F R' F' U2 F' U2 F R'",
        "U2 R U2 R' F2 L F' L' U2 L' U2 L F'",
        "U' B2 D2 L' F2 D2 B2 R' U'",
        "U B2 D2 R F2 D2 B2 L U",
        "D' R' D R2 U' R B2 L U' L' B2 U R2",
        "D L D' L2 U L' B2 R' U R B2 U' L2",
        "R' U L' U2 R U' L R' U L' U2 R U' L U'",
        "L U' R U2 L' U R' L U' R U2 L' U R' U",
        "F' U B U' F U B' U'",
        "F U' B' U F' U' B U",
        "L' U2 L R' F2 R",
        "R' U2 R L' B2 L",
        "M2 U M2 U2 M2 U M2"
};


/*
        0 1 2                                           _______
        3 4 5                                          |       |
        6 7 8                                          |  up   |
0 1 2   0 1 2   0 1 2   0 1 2                   _______|_______| _______ _______
3 4 5   3 4 5   3 4 5   3 4 5                  |       |       ||       |       |
6 7 8   6 7 8   6 7 8   6 7 8                  |  left | front || right | back  |
        0 1 2                                  |_______|_______||_______|_______|
        3 4 5                                          |       |
        6 7 8                                          |  down |
                                                       |_______|
*/

class RotateParams {
public:
    RotateParams() = default;
    RotateParams(int side, int times) : side(side), times(times) {};

    int side, times;
};

std::unordered_map<std::string, RotateParams> const moves_map{
        //front rotations
        {{"F"}, RotateParams(front, 1)},
        {{"F'"}, RotateParams(front, 3)},
        {{"F2"}, RotateParams(front, 2)},
        //up rotations
        {{"U"}, RotateParams(up, 1)},
        {{"U'"}, RotateParams(up, 3)},
        {{"U2"}, RotateParams(up, 2)},
        //down rotations
        {{"D"}, RotateParams(down, 1)},
        {{"D'"}, RotateParams(down, 3)},
        {{"D2"}, RotateParams(down, 2)},
        //left rotations
        {{"L"}, RotateParams(left, 1)},
        {{"L'"}, RotateParams(left, 3)},
        {{"L2"}, RotateParams(left, 2)},
        //right rotations
        {{"R"}, RotateParams(right, 1)},
        {{"R'"}, RotateParams(right, 3)},
        {{"R2"}, RotateParams(right, 2)},
        //back rotations
        {{"B"}, RotateParams(back, 1)},
        {{"B'"}, RotateParams(back, 3)},
        {{"B2"}, RotateParams(back, 2)},
        //front middle vertical
        {{"M"}, RotateParams(front_vertical, 1)},
        {{"M'"}, RotateParams(front_vertical, 3)},
        {{"M2"}, RotateParams(front_vertical, 2)},
        //front middle horizontal
        {{"E"}, RotateParams(front_horizontal, 1)},
        {{"E'"}, RotateParams(front_horizontal, 3)},
        {{"E2"}, RotateParams(front_horizontal, 2)},
        //up middle horizontal
        {{"S"}, RotateParams(up_horizontal, 1)},
        {{"S'"}, RotateParams(up_horizontal, 3)},
        {{"S2"}, RotateParams(up_horizontal, 2)},
        //x
        {{"x"}, RotateParams(x_axis, 1)},
        {{"x'"}, RotateParams(x_axis, 3)},
        {{"x2"}, RotateParams(x_axis, 2)},
        //y
        {{"y"}, RotateParams(y_axis, 1)},
        {{"y'"}, RotateParams(y_axis, 3)},
        {{"y2"}, RotateParams(y_axis, 2)},
        //z
        {{"z"}, RotateParams(z_axis, 1)},
        {{"z'"}, RotateParams(z_axis, 3)},
        {{"z2"}, RotateParams(z_axis, 2)},
        //lw
        {{"Lw"}, RotateParams(lw, 1)},
        {{"Lw'"}, RotateParams(lw, 3)},
        {{"Lw2"}, RotateParams(lw, 2)},
        //dw
        {{"Dw"}, RotateParams(dw, 1)},
        {{"Dw'"}, RotateParams(dw, 3)},
        {{"Dw2"}, RotateParams(dw, 2)}
};


class Cube {
public:
    Cube() {
        cube.resize(6, std::vector<std::vector<char>>(3, std::vector<char>(3)));
        move_count = 0;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cube[up][i][j] = 'w';
                cube[front][i][j] = 'r';
                cube[left][i][j] = 'g';
                cube[right][i][j] = 'b';
                cube[down][i][j] = 'y';
                cube[back][i][j] = 'o';
            }
        }
        fitness_score = 0;
    }
    Cube(std::string config) {
        cube.resize(6, std::vector<std::vector<char>>(3, std::vector<char>(3)));
        move_count = 0;

        int index = 0;
        for (int s = 0; s < 6; s++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    cube[s][i][j] = config[index++];
                }
            }
        }
        UpdateFitness();
    }
    void Rotate(RotateParams const& params) {
        Cube temp(*this);
        size_t rotate_times = params.times;

        if (params.side <= 5) {
            for (int j = 0; j < rotate_times; j++) {
                for (int i = 0; i < 3; i++) {
                    cube[params.side][0][i] = temp[params.side][2 - i][0];
                    cube[params.side][i][2] = temp[params.side][0][i];
                    cube[params.side][2][i] = temp[params.side][2 - i][2];
                    cube[params.side][i][0] = temp[params.side][2][i];
                }
                temp = *this;
            }
        }

        std::vector<int> adj;
        if (params.side <= 8)
            adj = changed_sides.at(params.side);

        for (int j = 0; j < rotate_times; j++) {
            if (params.side == front) {
                for (int i = 0; i < 3; i++) {
                    cube[adj[0]][i][2] = temp[adj[3]][0][i];
                    cube[adj[1]][2][i] = temp[adj[0]][2 - i][2];
                    cube[adj[2]][i][0] = temp[adj[1]][2][i];
                    cube[adj[3]][0][i] = temp[adj[2]][2 - i][0];
                }
            }
            else if (params.side == left) {
                for (int i = 0; i < 3; i++) {
                    cube[adj[0]][i][2] = temp[adj[3]][2 - i][0];
                    cube[adj[1]][i][0] = temp[adj[0]][2 - i][2];
                    cube[adj[2]][i][0] = temp[adj[1]][i][0];
                    cube[adj[3]][i][0] = temp[adj[2]][i][0];
                }
            }
            else if (params.side == right) {
                for (int i = 0; i < 3; i++) {
                    cube[adj[0]][i][2] = temp[adj[3]][i][2];
                    cube[adj[1]][i][2] = temp[adj[0]][i][2];
                    cube[adj[2]][i][0] = temp[adj[1]][2 - i][2];
                    cube[adj[3]][i][2] = temp[adj[2]][2 - i][0];
                }
            }
            else if (params.side == up) {
                for (int i = 0; i < 3; i++) {
                    cube[adj[0]][0][i] = temp[adj[3]][0][i];
                    cube[adj[1]][0][i] = temp[adj[0]][0][i];
                    cube[adj[2]][0][i] = temp[adj[1]][0][i];
                    cube[adj[3]][0][i] = temp[adj[2]][0][i];
                }
            }
            else if (params.side == down) {
                for (int i = 0; i < 3; i++) {
                    cube[adj[0]][2][i] = temp[adj[3]][2][i];
                    cube[adj[1]][2][i] = temp[adj[0]][2][i];
                    cube[adj[2]][2][i] = temp[adj[1]][2][i];
                    cube[adj[3]][2][i] = temp[adj[2]][2][i];
                }
            }
            else if (params.side == back) {
                for (int i = 0; i < 3; i++) {
                    cube[adj[0]][i][2] = temp[adj[3]][2][2 - i];
                    cube[adj[1]][0][i] = temp[adj[0]][i][2];
                    cube[adj[2]][i][0] = temp[adj[1]][0][2 - i];
                    cube[adj[3]][2][i] = temp[adj[2]][i][0];
                }
            }
            else if (params.side == front_vertical) {
                for (int i = 0; i < 3; i++) {
                    cube[adj[0]][i][1] = temp[adj[1]][i][1];
                    cube[adj[1]][i][1] = temp[adj[2]][2 - i][1];
                    cube[adj[2]][i][1] = temp[adj[3]][2 - i][1];
                    cube[adj[3]][i][1] = temp[adj[0]][i][1];
                }
            }
            else if (params.side == front_horizontal) {
                for (int i = 0; i < 3; i++) {
                    cube[adj[0]][1][i] = temp[adj[3]][1][i];
                    cube[adj[1]][1][i] = temp[adj[0]][1][i];
                    cube[adj[2]][1][i] = temp[adj[1]][1][i];
                    cube[adj[3]][1][i] = temp[adj[2]][1][i];
                }
            }
            else if (params.side == up_horizontal) {
                for (int i = 0; i < 3; i++) {
                    cube[adj[0]][1][i] = temp[adj[3]][2 - i][1];
                    cube[adj[1]][i][1] = temp[adj[0]][1][i];
                    cube[adj[2]][1][i] = temp[adj[1]][2 - i][1];
                    cube[adj[3]][i][1] = temp[adj[2]][1][i];
                }
            }

            else if (params.side == x_axis) {
                MakeMove("L' M' R", 1);
            }
            else if (params.side == y_axis) {
                MakeMove("U E' D'", 1);
            }
            else if (params.side == z_axis) {
                MakeMove("F S B'", 1);
            }
            else if (params.side == lw) {
                MakeMove("L M", 1);
            }
            else if (params.side == dw) {
                MakeMove("D E", 1);
            }

            temp = *this;
        }
    }

    void MakeMove(std::string const& moves, bool ignore = 0) {
        std::stringstream split(moves);
        std::string move;
        for (int i = 0; split >> move; i++) {
            RotateParams t(moves_map.at(move));
            Rotate(t);
            move_count++;
        }
        if (!ignore) {
            move_history.push_back(moves);
            UpdateFitness();
        }
    }

    void UpdateFitness() {
        size_t wrong_stickers = 0;
        for (int s = 0; s < 6; s++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (cube[s][i][j] != cube[s][1][1])
                        wrong_stickers++;
                }
            }
        }

        fitness_score = wrong_stickers;
    }

    size_t GetFitness() const {
        return fitness_score;
    }

    size_t GetMoveCount() const {
        return move_count;
    }

    std::vector<std::string> GetMoveHistory() const {
        return move_history;
    }

    void ResetMoveHistory() {
        move_count = 0;
        move_history.clear();
    }


    void Read(std::istream& in) {
        cube.resize(6, std::vector<std::vector<char>>(3, std::vector<char>(3)));
        move_count = 0;
        std::string side; //up front left right down back
        std::vector<std::vector<char>> temp(3, std::vector<char>(3));
        for (int i = 0; i < 6; i++) {
            in >> side;
            if (map_sides.count(side) == 0) {
                std::cerr << "Wrong side string\n";
                exit(1);
            }
            for (auto& j : temp) {
                for (auto& k : j) {
                    in >> k;
                    if (k != 'w' && k != 'r' && k != 'b' &&
                        k != 'y' && k != 'g' && k != 'o') {
                        std::cerr << "Wrong color letter\n";
                        exit(1);
                    }
                }
            }
            cube[map_sides.at(side)] = temp;
        }
        UpdateFitness();
    }

    void Write(std::ostream& out) {
        for (int i = 0; i < 6; i++) {
            out << sides_vec[i] << '\n';
            for (auto& row : cube[i]) {
                for (auto& col : row) {
                    out << col << " ";
                }
                out << '\n';
            }
        }
    }

    std::vector<std::vector<char>>const& operator[](size_t side) const {
        return cube[side];
    }

private:
    std::vector<std::vector<std::vector<char>>> cube;
    size_t fitness_score;
    std::vector<std::string> move_history;
    size_t move_count;
};
