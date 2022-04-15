#pragma once

#include <array>
#include <map>
#include "Visual.h"


std::string edge_swap = "R U R' U' R' F R2 U' R' U' R U R' F'";
std::string corner_swap = "R U' R' U' R U R' F' R U R' U' R' F R";
std::string parity = "R U' R' U' R U R D R' U' R D' R' U2 R' U'";


std::map<std::array<char, 2>, int> edge_colors_to_index = {
        {{'w', 'o'}, 0},
        {{'w', 'b'}, 1},
        {{'w', 'r'}, 2},
        {{'w', 'g'}, 3},

        {{'g', 'w'}, 4},
        {{'g', 'r'}, 5},
        {{'g', 'y'}, 6},
        {{'g', 'o'}, 7},

        {{'r', 'w'}, 8},
        {{'r', 'b'}, 9},
        {{'r', 'y'}, 10},
        {{'r', 'g'}, 11},

        {{'b', 'w'}, 12},
        {{'b', 'o'}, 13},
        {{'b', 'y'}, 14},
        {{'b', 'r'}, 15},

        {{'o', 'w'}, 16},
        {{'o', 'g'}, 17},
        {{'o', 'y'}, 18},
        {{'o', 'b'}, 19},

        {{'y', 'r'}, 20},
        {{'y', 'b'}, 21},
        {{'y', 'o'}, 22},
        {{'y', 'g'}, 23},
};

std::map<std::array<char, 3>, int> corner_colors_to_index = {
        {{'w', 'o', 'g'}, 0},
        {{'w', 'g', 'o'}, 0},
        {{'w', 'o', 'b'}, 1},
        {{'w', 'b', 'o'}, 1},
        {{'w', 'r', 'b'}, 2},
        {{'w', 'b', 'r'}, 2},
        {{'w', 'r', 'g'}, 3},
        {{'w', 'g', 'r'}, 3},

        {{'g', 'w', 'o'}, 4},
        {{'g', 'o', 'w'}, 4},
        {{'g', 'w', 'r'}, 5},
        {{'g', 'r', 'w'}, 5},
        {{'g', 'y', 'r'}, 6},
        {{'g', 'r', 'y'}, 6},
        {{'g', 'y', 'o'}, 7},
        {{'g', 'o', 'y'}, 7},

        {{'r', 'w', 'g'}, 8},
        {{'r', 'g', 'w'}, 8},
        {{'r', 'w', 'b'}, 9},
        {{'r', 'b', 'w'}, 9},
        {{'r', 'y', 'b'}, 10},
        {{'r', 'b', 'y'}, 10},
        {{'r', 'y', 'g'}, 11},
        {{'r', 'g', 'y'}, 11},

        {{'b', 'w', 'r'}, 12},
        {{'b', 'r', 'w'}, 12},
        {{'b', 'w', 'o'}, 13},
        {{'b', 'o', 'w'}, 13},
        {{'b', 'y', 'o'}, 14},
        {{'b', 'o', 'y'}, 14},
        {{'b', 'y', 'r'}, 15},
        {{'b', 'r', 'y'}, 15},

        {{'o', 'w', 'b'}, 16},
        {{'o', 'b', 'w'}, 16},
        {{'o', 'w', 'g'}, 17},
        {{'o', 'g', 'w'}, 17},
        {{'o', 'y', 'g'}, 18},
        {{'o', 'g', 'y'}, 18},
        {{'o', 'y', 'b'}, 19},
        {{'o', 'b', 'y'}, 19},

        {{'y', 'r', 'g'}, 20},
        {{'y', 'g', 'r'}, 20},
        {{'y', 'r', 'b'}, 21},
        {{'y', 'b', 'r'}, 21},
        {{'y', 'o', 'b'}, 22},
        {{'y', 'b', 'o'}, 22},
        {{'y', 'o', 'g'}, 23},
        {{'y', 'g', 'o'}, 23},
};

int PosToIndex(std::array<int, 3> t, bool corner = false) {
    int res = 0;
    std::vector<int> tmp = {0, 8, 4, 12, 20, 16};
    res = tmp[t[0]];
    std::array<int, 2> k = {t[1], t[2]};
    std::vector<std::array<int, 2>> ind;
    if (!corner)
        ind = {{0,1}, {1, 2}, {2,1}, {1,0}};
    else
        ind = {{0,0}, {0,2}, {2,2}, {2,0}};
    int j = 0;
    while (ind[j] != k)
        j++;

    return res + j;
}


std::string reverse_move(std::string move) {
    if (move.size() == 1) {
        return move + '\'';
    }
    else if (move[1] == '2') {
        return move;
    }
    else if (move[1] == '\'') {
        move.pop_back();
        return move;
    }
    else {
        if (move.size() == 2) {
            return move + '\'';
        }
        else if (move[2] == '2') {
            return move;
        }
        else {
            move.pop_back();
            return move;
        }
    }
}

std::vector<std::string> edge_setups = {
        {"Lw2 D' L2"},
        {""},
        {"Lw2 D L2"},
        {""},

        {"L Dw' L"},
        {"Dw' L"},
        {"L' Dw' L"},
        {"Dw L'"},

        {"Lw D' L2"},
        {"Dw2 L"},
        {"Lw D L2"},
        {"L'"},

        {""},
        {"Dw L"},
        {"D2 L' Dw' L"},
        {"Dw' L'"},

        {"Lw' D L2"},
        {"L"},
        {"Lw' D' L2"},
        {"Dw2 L'"},

        {"D' L2"},
        {"D2 L2"},
        {"D L2"},
        {"L2"},

};

std::vector<std::string> corner_setups = {
        {""},
        {"R2"},
        {"F2 D"},
        {"F2"},

        {""},
        {"F' D"},
        {"F'"},
        {"D' R"},

        {"F R'"},
        {"R'"},
        {"F' R'"},
        {"F2 R'"},

        {"F"},
        {"R' F"},
        {"R2 F"},
        {"R F"},

        {"R D'"},
        {""},
        {"D F'"},
        {"R"},

        {"D"},
        {""},
        {"D'"},
        {"D2"},
};


int GetLetterEdge() {
    std::array<char, 2> t = {RealCube[up][1][2], RealCube[right][0][1]};
    return edge_colors_to_index[t];
}

int GetLetterCorner() {
    std::array<char, 3> t = {RealCube[left][0][0], RealCube[up][0][0], RealCube[back][0][2]};
    return corner_colors_to_index[t];
}

bool IsBufferEdge() {
    return ((RealCube[up][1][2] == 'w' && RealCube[right][0][1] == 'b') ||
            (RealCube[up][1][2] == 'b' && RealCube[right][0][1] == 'w'));
}

bool IsBufferCorner() {
    std::string colors = {RealCube[left][0][0], RealCube[up][0][0], RealCube[back][0][2]};
    int k = 0;
    for (auto& c : colors)
        if (c == 'w' || c == 'g' || c == 'o')
            k++;

    return k == 3;
}

std::string ReverseString(std::string& str) {
    std::stringstream stream(str);
    std::vector<std::string> words;
    std::string rev;
    for (std::string word; stream >> word; )
        words.push_back(reverse_move(word));
    for (auto it = words.rbegin(); it != words.rend(); it++) {
        rev += *it;
        rev += ' ';
    }
    rev.pop_back();
    return rev;
}

void SwapEdgeBuffers(int letter) {
    if (edge_setups[letter].size())
        VisualCube.MakeMoves(edge_setups[letter]);
    VisualCube.MakeMoves(edge_swap);
    if (edge_setups[letter].size()) {
        std::string rev = ReverseString(edge_setups[letter]);
        VisualCube.MakeMoves(rev);
    }
}

void SwapCornerBuffers(int letter) {
    if (corner_setups[letter].size())
        VisualCube.MakeMoves(corner_setups[letter]);
    VisualCube.MakeMoves(corner_swap);
    if (corner_setups[letter].size()) {
        std::string rev = ReverseString(corner_setups[letter]);
        VisualCube.MakeMoves(rev);
    }
}

std::array<int, 3> AllEdgesSolved() {
    std::vector<std::pair<int,int>> t = {{0,1}, {1, 0}, {1, 2}, {2, 1}};
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 4; j++)
            if (RealCube[i][1][1] != RealCube[i][t[j].first][t[j].second]
            && !((i == 0 && j == 2) || (i == 3 && j == 0)))
                return {i, t[j].first, t[j].second};

    return {-1, -1, -1};
}

std::array<int, 3> AllCornersSolved() {
    std::vector<std::pair<int,int>> t = {{0,0}, {0,2}, {2, 2}, {2,0}};
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 4; j++)
            if (RealCube[i][1][1] != RealCube[i][t[j].first][t[j].second]
            && !((i == left && j == 0) || (i == up && j == 0) || (i == back && j == 1)))
                return {i, t[j].first, t[j].second};

    return {-1, -1, -1};
}

int SolveEdges() {
    std::array<int, 3> t = {};
    int letters = 0;
    while ((t = AllEdgesSolved())[0] != -1) {
        letters++;
        if (!IsBufferEdge()) {
            int l = GetLetterEdge();
            SwapEdgeBuffers(l);
        }
        else {
            SwapEdgeBuffers(PosToIndex(t, 0));
        }

        if (letters > 100)
            return -1;
    }

    return letters;
}

int SolveCorners() {
    std::array<int, 3> t = {};

    int letters = 0;
    while ((t = AllCornersSolved())[0] != -1) {
        letters++;
        if (!IsBufferCorner()) {
            int l = GetLetterCorner();
            //std::cout << "NOT buffer\n" << "letter: " << l << "\n";
            SwapCornerBuffers(l);
        }
        else {
            //std::cout << "Is buffer\n" << t[0] << t[1] << t[2] << "\n";
            SwapCornerBuffers(PosToIndex(t, 1));
            //std::cout << PosToIndex(t, 1) << "\n";
        }

        if (letters > 100)
            return -1;
    }

    return letters;
}

void OrientCube() {
    for (int i = 0; i < 6; i++) {
        if (RealCube[i][1][1] == 'w') {
            if (i == left || i == right || i == down) {
                while (RealCube[up][1][1] != 'w') {
                    std::string t{"z"};
                    VisualCube.MakeMoves(t);
                }
            }
            else if (i == front || i == back) {
                while (RealCube[up][1][1] != 'w') {
                    std::string t{"x"};
                    VisualCube.MakeMoves(t);
                }
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        if (RealCube[i][1][1] == 'r') {
            if (i != front) {
                while (RealCube[front][1][1] != 'r') {
                    std::string t{"y"};
                    VisualCube.MakeMoves(t);
                }
            }
        }
    }
}


std::pair<std::string, int> Pochmann() {
    RealCube.ResetMoveHistory();
    OrientCube();

    int parity_check = SolveEdges();

    if (parity_check == -1)
        return {"No edge solution exists\n", 0};
    else if (parity_check % 2)
        VisualCube.MakeMoves(parity);

    if (SolveCorners() == -1)
        return {"No corner solution exists\n", 0};

    std::string res;
    for (auto& m : RealCube.GetMoveHistory()) {
        res += m;
        res += ' ';
    }

    return {res, RealCube.GetMoveCount()};

}
