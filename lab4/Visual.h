#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imfilebrowser.h"
#include <glut.h>
#include <thread>
#include <future>
#include <queue>
#include "Pochmann.h"
#include "Cube.h"
#include "GenAlgo.h"
#include <chrono>
#include <fstream>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const float CUBE_SIZE = 12;
const int TIMER = 5;
const int CAMERA_SPEED = 5;
float ROTATE_SPEED = 3;
//white yellow blue green red orange
uint32_t COLORS[6] = { 0x00FFFFFF, 0x0000FFFF, 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00006FFF };

enum small_cube_faces { c_up = 0, c_down = 1, c_front = 2, c_back = 3, c_left = 4, c_right = 5 };
enum cube_sides { s_up = 0, s_mu = 1, s_down = 2, s_left = 3, s_ml = 4, s_right = 5,
        s_front = 6, s_mf = 7, s_back = 8};

float zoom = -35.0;
float x_rot = -60;
float y_rot = 0;
float z_rot = 117;
float side_rot = 0;

std::pair<std::string, int> Pochmann();

Cube RealCube;

struct Rotator {
    Rotator(int side, int angle) : side(side), angle(angle) {}

    int side = 0;
    int angle = 0;
};

class SmallCube {
public:
    SmallCube() = default;

    void SetSize(float size) {
        size_ = size;
    }

    void SetColor(int side, uint32_t col) {
        color_[side] = col;
    }

    void Draw() {
        glBegin(GL_QUADS);

        //up
        glColor3ubv(GetColor(0));
        glVertex3f(size_, size_, size_);
        glVertex3f(0, size_, size_);
        glVertex3f(0, 0, size_);
        glVertex3f(size_, 0, size_);

        //down
        glColor3ubv(GetColor(1));
        glVertex3f(size_, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, size_, 0);
        glVertex3f(size_, size_, 0);

        //front
        glColor3ubv(GetColor(2));
        glVertex3f(size_, 0, size_);
        glVertex3f(0, 0, size_);
        glVertex3f(0, 0, 0);
        glVertex3f(size_, 0, 0);

        //back
        glColor3ubv(GetColor(3));
        glVertex3f(size_, size_, 0);
        glVertex3f(0, size_, 0);
        glVertex3f(0, size_, size_);
        glVertex3f(size_, size_, size_);

        //left
        glColor3ubv(GetColor(4));
        glVertex3f(0, size_, size_);
        glVertex3f(0, size_, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, size_);

        //right
        glColor3ubv(GetColor(5));
        glVertex3f(size_, size_, 0);
        glVertex3f(size_, size_, size_);
        glVertex3f(size_, 0, size_);
        glVertex3f(size_, 0, 0);

        glEnd();
    }

    void Draw(double x, double y, double z) {
        glPushMatrix();
        glTranslated(x, y, z);
        Draw();
        glPopMatrix();
    }

    unsigned char* GetColor(int x) {
        memcpy(col_, color_+x, sizeof(color_[x]));
        return col_;
    }

    void RotateZ() {
        unsigned int t = color_[c_right];
        color_[c_right] = color_[c_back];
        color_[c_back] = color_[c_left];
        color_[c_left] = color_[c_front];
        color_[c_front] = t;
    }

    void RotateY() {
        unsigned int t = color_[c_front];
        color_[c_front] = color_[c_down];
        color_[c_down] = color_[c_back];
        color_[c_back] = color_[c_up];
        color_[c_up] = t;
    }

    void RotateX() {
        unsigned int t = color_[c_up];
        color_[c_up] = color_[c_left];
        color_[c_left] = color_[c_down];
        color_[c_down] = color_[c_right];
        color_[c_right] = t;
    }

private:
    uint32_t color_[6] = {0};
    uint8_t col_[4] = {0};
    float size_ = 0;
};


class BigCube {
public:
    BigCube() = default;

    void Init() {
        Cube temp;
        RealCube = temp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < 3; k++) {
                    cube[i][j][k].SetSize((CUBE_SIZE / 3.0) * 0.95);
                }

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                ////white yellow blue green red orange
                cube[i][j][2].SetColor(c_up, COLORS[0]);
                cube[i][j][0].SetColor(c_down, COLORS[1]);
                cube[j][0][i].SetColor(c_front, COLORS[2]);
                cube[j][2][i].SetColor(c_back, COLORS[3]);
                cube[0][j][i].SetColor(c_left, COLORS[4]);
                cube[2][j][i].SetColor(c_right, COLORS[5]);
            }
    }

    void SetColors() {
        std::unordered_map<char, int> color_map {
            {'w', 0}, {'y', 1}, {'b', 2}, {'g', 3}, {'r', 4}, {'o', 5}
        };

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                cube[i][j][2].SetColor(c_up, COLORS[color_map[RealCube[up][2-i][2-j]]]);
                cube[i][j][0].SetColor(c_down, COLORS[color_map[RealCube[down][i][2-j]]]);
                cube[j][0][i].SetColor(c_front, COLORS[color_map[RealCube[right][2-i][j]]]);
                cube[j][2][i].SetColor(c_back, COLORS[color_map[RealCube[left][2-i][2-j]]]);
                cube[0][j][i].SetColor(c_left, COLORS[color_map[RealCube[front][2-i][2-j]]]);
                cube[2][j][i].SetColor(c_right, COLORS[color_map[RealCube[back][2-i][j]]]);
            }
    }

    void Draw() {
        memset(is_rotating, 0, sizeof(is_rotating));

        if (RotateQueSize()) {
            glPushMatrix();

            if (RotateQueFront().side == s_up || RotateQueFront().side == s_mu || RotateQueFront().side == s_down) {
                int k = RotateQueFront().side == s_down ? 0 : RotateQueFront().side == s_mu ? 1 : 2;

                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        is_rotating[i][j][k] = true;

                glTranslated(CUBE_SIZE / 2, CUBE_SIZE / 2, 0);
                glRotatef(side_angle[RotateQueFront().side], 0, 0, 1);
                glTranslated(-CUBE_SIZE / 2, -CUBE_SIZE / 2, 0);

                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++)
                        cube[i][j][k].Draw(CUBE_SIZE / 3 * i, CUBE_SIZE / 3 * j, CUBE_SIZE / 3 * k);
            }

            if (RotateQueFront().side == s_left || RotateQueFront().side == s_ml || RotateQueFront().side == s_right) {
                int k = RotateQueFront().side == s_left ? 2 : RotateQueFront().side == s_ml ? 1 : 0;

                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        is_rotating[i][k][j] = true;

                glTranslated(CUBE_SIZE / 2, 0, CUBE_SIZE / 2);
                glRotatef(side_angle[RotateQueFront().side], 0, 1, 0);
                glTranslated(-CUBE_SIZE / 2, 0, -CUBE_SIZE / 2);

                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++)
                        cube[i][k][j].Draw(CUBE_SIZE / 3 * i, CUBE_SIZE / 3 * k, CUBE_SIZE / 3 * j);
            }

            if (RotateQueFront().side == s_front || RotateQueFront().side == s_mf || RotateQueFront().side == s_back) {
                int k = RotateQueFront().side == s_front ? 0 : RotateQueFront().side == s_mf ? 1 : 2;

                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        is_rotating[k][i][j] = true;

                glTranslated(0, CUBE_SIZE / 2, CUBE_SIZE / 2);
                glRotatef(side_angle[RotateQueFront().side], 1, 0, 0);
                glTranslated(0, -CUBE_SIZE / 2, -CUBE_SIZE / 2);

                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++)
                        cube[k][i][j].Draw(CUBE_SIZE / 3 * k, CUBE_SIZE / 3 * i, CUBE_SIZE / 3 * j);
            }


            glPopMatrix();
        }


        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    if (!is_rotating[i][j][k])
                        cube[i][j][k].Draw(CUBE_SIZE/3 * i,CUBE_SIZE/3 * j, CUBE_SIZE/3 * k);
                }
            }
        }
    }

    void FinalizeRotation(int side, int num_rotations) {
        SmallCube temp[3][3];
        for (int m = 0; m < num_rotations; m++) {
            if (side == s_up || side == s_mu || side == s_down) {
                int k = side == s_down ? 0 : side == s_mu ? 1 : 2;
                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++) {
                        temp[j][2 - i] = cube[i][j][k];
                    }

                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++) {
                        temp[i][j].RotateZ();
                        cube[i][j][k] = temp[i][j];
                    }
            }

            if (side == s_left || side == s_ml || side == s_right) {
                int k = side == s_left ? 2 : side == s_ml ? 1 : 0;
                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++) {
                        temp[j][2 - i] = cube[i][k][j];
                    }

                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++) {
                        temp[i][j].RotateX();
                        cube[i][k][j] = temp[i][j];
                    }
            }

            if (side == s_front || side == s_mf || side == s_back) {
                int k = side == s_front ? 0 : side == s_mf ? 1 : 2;
                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++) {
                        temp[j][2 - i] = cube[k][i][j];
                    }

                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++) {
                        temp[i][j].RotateY();
                        cube[k][i][j] = temp[i][j];
                    }
            }
        }
    }

    void Rotate(Rotator cfg, bool push = false) {
        if (push)
            rotate_que.push(cfg);
        if (RotateQueSize()) {
            side_angle[RotateQueFront().side] += RotateQueFront().angle;

            if (side_angle[RotateQueFront().side] % 90 == 0) {
                if ((side_angle[RotateQueFront().side] < 0)
                        ^ (RotateQueFront().side == s_left || RotateQueFront().side == s_ml || RotateQueFront().side == s_right))
                    FinalizeRotation(RotateQueFront().side, 1);
                else
                    FinalizeRotation(RotateQueFront().side, 3);
                side_angle[RotateQueFront().side] = 0;
                rotate_que.pop();
            }

        }
    }

    void Rotate(char key, int times = 1, bool manual = 0) {
        if (manual) {
            if (key == 'x' || key == 'y' || key == 'z') {
                RealCube.MakeMove({key});
            } else if (key == 'X' || key == 'Y' || key == 'Z') {
                std::string t{key};
                t += '\'';
                RealCube.MakeMove(t);
            } else if (key == 'q' || key == 'Q') {
                islower(key) ? RealCube.MakeMove("Lw") : RealCube.MakeMove("Lw'");
            } else if (key == 'w' || key == 'W') {
                islower(key) ? RealCube.MakeMove("Dw") : RealCube.MakeMove("Dw'");
            } else if (islower(key)) {
                char t = toupper(key);
                RealCube.MakeMove({t});
            } else {
                std::string t{key};
                t += '\'';
                RealCube.MakeMove(t);
            }
        }

        switch(key) {
            case 'u': {
                Rotator t(s_up, -ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'U': {
                Rotator t(s_up, ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'd': {
                Rotator t(s_down, ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'D': {
                Rotator t(s_down, -ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'e': {
                Rotator t(s_mu, ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'E': {
                Rotator t(s_mu, -ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }

            case 'f': {
                Rotator t(s_front, ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'F': {
                Rotator t(s_front, -ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }

            case 'b': {
                Rotator t(s_back, -ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'B': {
                Rotator t(s_back, ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }

            case 's': {
                Rotator t(s_mf, ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'S': {
                Rotator t(s_mf, -ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }

            case 'l': {
                Rotator t(s_left, -ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'L': {
                Rotator t(s_left, ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'r': {
                Rotator t(s_right, ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'R': {
                Rotator t(s_right, -ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'm': {
                Rotator t(s_ml, -ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'M': {
                Rotator t(s_ml, ROTATE_SPEED);
                while (times--)
                    Rotate(t, 1);
                break;
            }
            case 'x': {
                while (times--) {
                    Rotate('L');
                    Rotate('M');
                    Rotate('r');
                }
                break;
            }
            case 'X': {
                while (times--) {
                    Rotate('l');
                    Rotate('m');
                    Rotate('R');
                }
                break;
            }
            case 'y': {
                while (times--) {
                    Rotate('u');
                    Rotate('E');
                    Rotate('D');
                }
                break;
            }
            case 'Y': {
                while (times--) {
                    Rotate('U');
                    Rotate('e');
                    Rotate('d');
                }
                break;
            }
            case 'z': {
                while (times--) {
                    Rotate('f');
                    Rotate('s');
                    Rotate('B');
                }
                break;
            }
            case 'Z': {
                while (times--) {
                    Rotate('F');
                    Rotate('S');
                    Rotate('b');
                }
                break;
            }
            case 'q': { //lw
                while (times--) {
                    Rotate('l');
                    Rotate('m');
                }
                break;
            }
            case 'Q': { //lw'
                while (times--) {
                    Rotate('L');
                    Rotate('M');
                }
                break;
            }
            case 'w': { //dw
                while (times--) {
                    Rotate('d');
                    Rotate('e');
                }
                break;
            }
            case 'W': { //dw'
                while (times--) {
                    Rotate('D');
                    Rotate('E');
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    void MakeMoves(std::string& moves) {
        std::stringstream split(moves);
        std::string move;
        RealCube.MakeMove(moves);
        for (int i = 0; split >> move; i++) {
            if (move.size() == 1) {
                Rotate((char)tolower(move[0]));
            }
            else if (move[1] == '2') {
                Rotate((char)tolower(move[0]), 2);
            }
            else if (move[1] == '\''){
                Rotate((char)toupper(move[0]));
            }
            else {
                if (move == "Lw") {
                    Rotate('q');
                }
                else if (move == "Lw2") {
                    Rotate('q', 2);
                }
                else if (move == "Lw'") {
                    Rotate('Q');
                }
                else if (move == "Dw") {
                    Rotate('w');
                }
                else if (move == "Dw2") {
                    Rotate('w', 2);
                }
                else if (move == "Dw'") {
                    Rotate('W');
                }
            }
        }
    }

    void RandomScramble() {
        std::vector<std::string> rand_moves = {
                "F", "F2", "F'", "U", "U2", "U'",
                "B", "B2", "B'", "D", "D2", "D'",
                "L", "L2", "L'", "R", "R2", "R'"
        };
        for (int i = 0; i < 20; i++) {
            int t = std::rand() % 18;
            MakeMoves(rand_moves[t]);
        }
    }

    int RotateQueSize() const { return rotate_que.size(); }
    Rotator RotateQueFront() const { return rotate_que.front(); }

private:
    SmallCube cube[3][3][3]; //27 small cubes
    std::queue<Rotator> rotate_que;
    int side_angle[9] = {0}; //angles for each sides
    bool is_rotating[3][3][3] = {false}; //flag if small cube is being rotated
};

BigCube VisualCube;


void ReshapeFunc(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, double(w)/double(h), 1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    ImGui_ImplGLUT_ReshapeFunc(w, h);
}

void ImguiWindow()
{
    static ImGuiTextBuffer log;
    static ImGui::FileBrowser fileDialog(ImGuiFileBrowserFlags_EnterNewFilename);

    static double elapsed_time_ms = 0;
    static int move_count = 0;
    static int population = 600;
    static int elitism = 50;
    static int file_state = 0;

    ImGui::Begin("Rubik's settings", 0, ImGuiWindowFlags_MenuBar);


    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..")) {
                fileDialog.Open();
                file_state = 1;
            }
            if (ImGui::MenuItem("Save"))   {
                fileDialog.Open();
                file_state = 2;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    fileDialog.Display();
    if(fileDialog.HasSelected())
    {
        if (file_state == 1) {
            std::ifstream file_cube(fileDialog.GetSelected().string());
            if (file_cube) {
                RealCube.Read(file_cube);
                VisualCube.SetColors();
            }
        }
        else if (file_state == 2) {
            std::ofstream file_cube(fileDialog.GetSelected().string());
            if (file_cube) {
                RealCube.Write(file_cube);
            }
        }

        fileDialog.ClearSelected();
    }

    static char str1[512] = "R F' D U x z y2";

    if (ImGui::Button("Reset"))
        VisualCube.Init();

    ImGui::SameLine();
    if (ImGui::Button("Scramble")) {
        std::string t = str1;
        VisualCube.MakeMoves(t);
        memset(str1, 0, sizeof(str1));
    }
    ImGui::SameLine();

    if (ImGui::Button("Print to console"))
        RealCube.Write(std::cout);

    if (ImGui::Button("Random scramble"))
        VisualCube.RandomScramble();

    ImGui::TextColored(ImVec4(1,0,0,1), "Gen algo options");
    ImGui::DragInt("Population", &population, 5, 50, 10000, "%d");
    ImGui::DragInt("Elitism", &elitism, 5, 5, 200, "%d");

    ImGui::TextColored(ImVec4(1,0,0,1), "Solve");
    if (ImGui::Button("Gen algo")) {
        auto t_start = std::chrono::high_resolution_clock::now();

        RealCube.ResetMoveHistory();
        log.clear();
        std::string solve_str;
        std::pair<std::string, int> data = SolveCube(RealCube, population, elitism);
        solve_str = data.first;
        move_count = data.second;

        auto t_end = std::chrono::high_resolution_clock::now();

        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();

        log.append(&solve_str[0]);
        if (move_count)
            VisualCube.MakeMoves(solve_str);
    }
    ImGui::SameLine();
    if (ImGui::Button("Pochmann")) {
        auto t_start = std::chrono::high_resolution_clock::now();

        log.clear();

        std::string solve_str;
        std::pair<std::string, int> data = Pochmann();
        solve_str = data.first;
        move_count = data.second;

        auto t_end = std::chrono::high_resolution_clock::now();
        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();


        log.append(&solve_str[0]);
    }
    ImGui::SameLine();
    if (ImGui::Button("1000 Poch")) {
        auto t_start = std::chrono::high_resolution_clock::now();

        log.clear();

        std::string solve_str;
        int move_sum = 0;
        for (int i = 0; i < 1000; i++) {
            VisualCube.RandomScramble();
            std::pair<std::string, int> data = Pochmann();
            solve_str = data.first;
            move_sum += data.second;
        }
        move_count = move_sum / 1000;

        auto t_end = std::chrono::high_resolution_clock::now();
        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();

    }

    ImGui::TextColored(ImVec4(1,0,0,1), "Controls");
    ImGui::Text("L R U D B F M E S X Y Z");
    ImGui::Text("Hold shift for ccw");

    ImGui::InputTextWithHint("str", "enter scramble string", str1, IM_ARRAYSIZE(str1));

    ImGui::SliderFloat("Zoom", &zoom, -100.0f, 100.0f);
    ImGui::SliderFloat("Rot X", &x_rot, -180.0f, 180.0f);
    ImGui::SliderFloat("Rot Y", &y_rot, -180.0f, 180.0f);
    ImGui::SliderFloat("Rot Z", &z_rot, -180.0f, 180.0f);

    ImGui::Text("Rot speed");
    if (ImGui::RadioButton("3", ROTATE_SPEED == 3)) { ROTATE_SPEED = 3; } ImGui::SameLine();
    if (ImGui::RadioButton("5", ROTATE_SPEED == 5))     { ROTATE_SPEED = 5; } ImGui::SameLine();
    if (ImGui::RadioButton("15", ROTATE_SPEED == 15)) { ROTATE_SPEED = 15; } ImGui::SameLine();
    if (ImGui::RadioButton("30", ROTATE_SPEED == 30)) { ROTATE_SPEED = 30; } ImGui::SameLine();
    if (ImGui::RadioButton("90", ROTATE_SPEED == 90)) { ROTATE_SPEED = 90; }

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Solution"); ImGui::SameLine();
    ImGui::Text("Time: %.0fms", elapsed_time_ms); ImGui::SameLine();
    ImGui::Text("Moves: %d", move_count);
    ImGui::BeginChild("Scrollable");
    ImGui::TextWrapped(log.begin(), log.end());
    ImGui::EndChild();

    ImGui::End();
}


void DisplayFunc()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    ImguiWindow();

    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();

    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);

    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0, 0, zoom);
    glRotatef(x_rot, 1, 0, 0);
    glRotatef(y_rot, 0, 1, 0);
    glRotatef(z_rot, 0, 0, 1);
    glTranslatef(CUBE_SIZE / -2.0f, CUBE_SIZE / -2.0f, CUBE_SIZE / -2.0f);

    VisualCube.Draw();
    glPopMatrix();

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN) {
        x_rot += CAMERA_SPEED;
        if (x_rot >= 360)
            x_rot -= 360;
        glutPostRedisplay();
    }

    if (key == GLUT_KEY_UP) {
        x_rot -= CAMERA_SPEED;
        if (x_rot < 0)
            x_rot += 360;
        glutPostRedisplay();
    }

    if (key == GLUT_KEY_RIGHT) {
        z_rot += CAMERA_SPEED;
        if (z_rot >= 360)
            z_rot -= 360;
        glutPostRedisplay();
    }

    if (key == GLUT_KEY_LEFT) {
        z_rot -= CAMERA_SPEED;
        if (z_rot < 0)
            z_rot += 360;
        glutPostRedisplay();
    }

    ImGui_ImplGLUT_SpecialFunc(key, x, y);
    ImGui_ImplGLUT_SpecialUpFunc(key, x, y);
}

std::string const keys {"uUmMdDlLeErRfFsSbB123xXyYzZqQwW" };

void KeyFunc(unsigned char key, int x, int y) {

    if (!ImGui::GetIO().WantCaptureKeyboard && key == '=') {
        zoom += CAMERA_SPEED;
        glutPostRedisplay();
    }
    else if (!ImGui::GetIO().WantCaptureKeyboard && key == '-') {
        zoom -= CAMERA_SPEED;
        glutPostRedisplay();
    }
    else if (!ImGui::GetIO().WantCaptureKeyboard && keys.find(key) != std::string::npos) {
        VisualCube.Rotate(key, 1, 1);
    }

    ImGui_ImplGLUT_KeyboardFunc(key, x, y);
    ImGui_ImplGLUT_KeyboardUpFunc(key, x, y);
}

void TimerFunc(int value) {
    if (VisualCube.RotateQueSize())
        VisualCube.Rotate(VisualCube.RotateQueFront());

    DisplayFunc();

    glutTimerFunc(TIMER, TimerFunc, 0);
}

int draw_cube(int argc, char **argv)
{
    //test_gui(argc, argv);
    std::srand(std::time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(1, 1);
    glutCreateWindow("Rubik's Cube");
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnable(GL_DEPTH_TEST);

    VisualCube.Init();

    glutDisplayFunc(DisplayFunc);
    glutReshapeFunc(ReshapeFunc);
    glutKeyboardFunc(KeyFunc);
    glutTimerFunc(TIMER, TimerFunc, 0);
    glutSpecialFunc(SpecialKeys);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGLUT_Init();

    glutMotionFunc(ImGui_ImplGLUT_MotionFunc);
    glutPassiveMotionFunc(ImGui_ImplGLUT_MotionFunc);
    glutMouseFunc(ImGui_ImplGLUT_MouseFunc);

    ImGui_ImplOpenGL2_Init();

    io.Fonts->AddFontFromFileTTF("../imgui/misc/fonts/Roboto-Medium.ttf", 14.0f);

    glutMainLoop();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
