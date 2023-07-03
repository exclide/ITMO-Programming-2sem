#pragma once

class GenAlgo {
public:
    GenAlgo(size_t population, size_t elitism) :
            population(population), elitism_min(elitism), max_generations(200) {};

    std::pair<std::string, int> Solve(Cube& cube_init) {
        std::vector<Cube> cubes;
        for (int i = 0; i < population; i++) {
            Cube cube(cube_init);
            cube.MakeMove(RandomSingleMove());
            cube.MakeMove(RandomSingleMove());
            RandomizeCube(cube);
            cubes.push_back(cube);
        }

        for (int g = 0; g < max_generations; g++) {
            std::sort(cubes.begin(), cubes.end(), [](Cube& a, Cube& b) { return a.GetFitness() < b.GetFitness(); });
            std::cout << "Generation: " << g << " Fitness: " << cubes[0].GetFitness() << "\n";
            for (int i = 0; i < cubes.size(); i++) {
                if (cubes[i].GetFitness() == 0) {
                    std::cout << "\nMove count: " << cubes[i].GetMoveCount() << '\n';
                    std::string buf;
                    for (auto& x : cubes[i].GetMoveHistory())
                        buf += x + " ";
                    std::cout << buf << '\n';
                    return {buf, cubes[i].GetMoveCount()};
                }

                if (i >= elitism_min) {
                    cubes[i] = cubes[std::rand() % elitism_min];
                    RandomizeCube(cubes[i]);
                }
            }
        }

        return {"No solution exists", 0};
    }

    void RandomizeCube(Cube& cb) {
        int t = std::rand() % 6;

        switch (t) {
            case 0: {
                cb.MakeMove(RandomPermutation());
                break;
            }
            case 1: {
                cb.MakeMove(RandomPermutation());
                cb.MakeMove(RandomPermutation());
                break;
            }
            case 2: {
                cb.MakeMove(RandomRotation());
                cb.MakeMove(RandomPermutation());
                break;
            }
            case 3: {
                cb.MakeMove(RandomOrientation());
                cb.MakeMove(RandomPermutation());
                break;
            }
            case 4: {
                cb.MakeMove(RandomRotation());
                cb.MakeMove(RandomOrientation());
                cb.MakeMove(RandomPermutation());
                break;
            }
            case 5: {
                cb.MakeMove(RandomOrientation());
                cb.MakeMove(RandomRotation());
                cb.MakeMove(RandomPermutation());
                break;
            }
            default: {
                break;
            }
        }
    }

    std::string RandomSingleMove() const {
        return single_moves[std::rand() % single_moves.size()];
    }
    std::string RandomPermutation() const {
        return permutations[std::rand() % permutations.size()];
    }
    std::string RandomRotation() const {
        return full_rotations[std::rand() % full_rotations.size()];
    }
    std::string RandomOrientation() const {
        return orientations[std::rand() % orientations.size()];
    }

private:
    size_t population;
    size_t max_generations;
    size_t elitism_min;
};

std::pair<std::string, int> SolveCube(Cube& cube, int population, int elitism) {
    GenAlgo solver(population, elitism);
    return solver.Solve(cube);
}
