#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <algorithm>
int x_length_of_world = 60;
int y_length_of_world = 60;
std::vector<std::vector<int>> world(x_length_of_world, std::vector<int>(y_length_of_world, 0));
std::vector<std::vector<int>> color_names = {
        {255, 255, 255},
        {255, 255, 0},
        {0, 255, 0},
        {0, 0, 255},
        {165, 42, 42},
        {0, 0, 0}
    };  

void initial_body_positioner(const std::vector<int> position) {
    int x_posi = position[0];
    int y_posi = position[1];
    world[x_posi][y_posi] = 1;
    world[x_posi][y_posi + 1] = 2;
    world[x_posi][y_posi - 1] = 3;
    world[x_posi + 1][y_posi] = 4;
    world[x_posi - 1][y_posi] = 4;
}
void printworld(){
    for (std::vector<int> row : world) {
        for (int num: row){
            std::cout << num << " ";

        }        
        std::cout<<"\n";       
    }

}
std::vector<std::vector<std::vector<int>>> map_colors(std::vector<std::vector<int>>& array) {
    std::vector<std::vector<std::vector<int>>> color_array;

    for (auto& row : array) {
        std::vector<std::vector<int>> color_row;
        for (int number : row) {
            if (number < color_names.size()) {
                color_row.push_back(color_names[number]);
            } else {
                color_row.push_back({255, 0, 0}); 
            }
        }
        color_array.push_back(color_row);
    }
    
    return color_array;
}

std::vector<int> add_arrays_1d(std::vector<int> array1, std::vector<int> array2) {
    size_t length = array1.size();
    std::vector<int> result(length, 0); // Initialize result with zeros

    for (size_t i = 0; i < length; ++i) {
        result[i] = array1[i] + array2[i];
    }

    return result;
}
std::vector<int> max_position_finder_2d_matrix(std::vector<std::vector<int>> matrix) {
    int max_value = -10000;
    std::vector<int> max_position = {0, 0};
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            int value = matrix[i][j];
            if (value > max_value) {
                max_value = value;
                max_position = {i,j};
            }
        }
    }

    return max_position;
}
void body_positioner(
    const std::vector<int> old_position,
    const std::vector<int> new_position,
    const std::vector<std::pair<std::vector<int>, int>> old_body,
    const std::vector<std::pair<std::vector<int>, int>> new_body
) {
    
    for (auto element : old_body) {
        std::vector<int> actual_element_position = add_arrays_1d(element.first, old_position);
        world[actual_element_position[0]][actual_element_position[1]] = 0;
    }

    
    for (auto element : new_body) {
        std::vector<int> new_element_position = add_arrays_1d(element.first, new_position);
        world[new_element_position[0]][new_element_position[1]] = element.second;
    }
}

std::vector<std::vector<int>> single_body_positioner(std::vector<std::pair<std::vector<int>, int>> body) {
    std::vector<std::vector<int>> single_organism_world(9, std::vector<int>(9, 0)); 

    for (auto element : body) {
        std::vector<int> new_element_position = add_arrays_1d(element.first, {4, 4});
        single_organism_world[new_element_position[0]][new_element_position[1]] = element.second;
    }

    return single_organism_world;
}
std::vector<int> rotate_90_clockwise(std::vector<int> vector) {
    return {vector[1], -vector[0]};
}


std::vector<int> rotate_90_anticlockwise(std::vector<int> vector) {
    return {-vector[1], vector[0]};
}

// int find_num_of_legs(Organism organism) {
//     int no_of_legs = 0;
//     for (auto element : organism.body) {
//         if (element.second == 3) { 
//             ++no_of_legs;
//         }
//     }
//     return no_of_legs;
// }

// void list_to_evolution_caller(std::vector<int> list) {
//     int num_chunks = list.size() / 3;
//     for (int i = 0; i < num_chunks; ++i) {
        
//         std::vector<std::vector<int>> matrix(7, std::vector<int>(7, 0));        
//         matrix[3 + list[1 + i * 3]][3 + list[0 + i * 3]] = 1;       
//         Ram.evolve(matrix, list[2 + i * 3]);
//     }
// }

// void list_to_evolution_caller2(std::vector<int> list) {
//     int num_chunks = list.size() / 3;
//     for (int i = 0; i < num_chunks; ++i) {
        
//         std::vector<std::vector<int>> matrix(7, std::vector<int>(7, 0));        
//         matrix[3 + list[1 + i * 3]][3 + list[0 + i * 3]] = 1;       
//         Shyam.evolve(matrix, list[2 + i * 3]);
//     }
// }

class Organism {
public:
    
    Organism(std::vector<int> position) : energy(0), position(position) {        
        body = {
            {{0, 0}, 1},
            {{0, -1}, 3},
            {{0, 1}, 2},
            {{-1, 0}, 4},
            {{1, 0}, 4}
        };        
        initial_body_positioner(position);        
        organisms.push_back(*this);
    }

    
    int energy;
    std::vector<int> position;
    std::vector<std::pair<std::vector<int>, int>> body;    
    static std::vector<Organism> organisms;

    void move(std::vector<int> direction_and_turn_array) {
        int max_value = *std::max_element(direction_and_turn_array.begin(), direction_and_turn_array.begin() + 9);
        auto direction = std::distance(direction_and_turn_array.begin(),
                                       std::find(direction_and_turn_array.begin(), direction_and_turn_array.begin() + 9, max_value));

        int turn;
        if (direction_and_turn_array[9] >= direction_and_turn_array[10] && direction_and_turn_array[9] >= direction_and_turn_array[11]) {
            turn = 0;
        } else if (direction_and_turn_array[10] > direction_and_turn_array[11]) {
            turn = 1;
        } else {
            turn = 2;
        }

        std::vector<int> old_position = position;
        switch (direction) {
            case 1: position = {position[0] - 1, position[1]}; break;
            case 2: position = {position[0] - 1, position[1] + 1}; break;
            case 3: position = {position[0], position[1] + 1}; break;
            case 4: position = {position[0] + 1, position[1] + 1}; break;
            case 5: position = {position[0] + 1, position[1]}; break;
            case 6: position = {position[0] + 1, position[1] - 1}; break;
            case 7: position = {position[0], position[1] - 1}; break;
            case 8: position = {position[0] - 1, position[1] - 1}; break;
            default: std::cerr << "Invalid direction. Please enter a number from 1 to 8." << std::endl;
        }

        std::vector<std::pair<std::vector<int>, int>> old_body = body;
        if (turn == 2) {
            std::vector<std::vector<int>> new_body_positions;
            for (auto element : body) {
                new_body_positions.push_back(rotate_90_clockwise(element.first));
            }
            for (int i = 0; i < body.size(); ++i) {
                body[i].first = new_body_positions[i];
            }
        } else if (turn == 1) {
            std::vector<std::vector<int>> new_body_positions;
            for (auto element : body) {
                new_body_positions.push_back(rotate_90_anticlockwise(element.first));
            }
            for (int i = 0; i < body.size(); ++i) {
                body[i].first = new_body_positions[i];
            }
        }

        bool invalid_move = false;
        for (auto element : old_body) {
            auto actual_element_position = add_arrays_1d(element.first, old_position);
            world[actual_element_position[0]][actual_element_position[1]] = 0;
        }

        for (auto element : body) {
            auto new_pos = add_arrays_1d(element.first, position);
            if (new_pos[0] < 0 || new_pos[1] < 0 || new_pos[0] >= x_length_of_world || new_pos[1] >= y_length_of_world) {
                invalid_move = true;
                break;
            }
            if (world[new_pos[0]][new_pos[1]] != 0) {
                invalid_move = true;
                if (world[new_pos[0]][new_pos[1]] == 5) {
                    if (element.second == 2) {
                        int muscle_count = 0;
                        for (auto element1 : body) {
                            if (element1.second == 4) {
                                muscle_count++;
                            }
                        }
                        if (energy < muscle_count) {
                            energy++;
                            world[new_pos[0]][new_pos[1]] = 0;
                        }
                    }
                }
                if (element.second == 6) {
                    std::vector<int> point_of_attack = {new_pos[0], new_pos[1]};
                    if (world[point_of_attack[0]][point_of_attack[1]] == 6) {
                        break;
                    }
                    for (Organism organism : organisms) {
                        if (&organism != this) {
                            for (auto elem : organism.body) {
                                if (add_arrays_1d(elem.first, organism.position) == point_of_attack) {
                                    if (elem.second == 1) {
                                        for (auto elem_body : organism.body) {
                                            world[add_arrays_1d(elem_body.first, organism.position)[0]][add_arrays_1d(elem_body.first, organism.position)[1]] = 5;
                                        }
                                        organism.body.clear();
                                    } else {
                                        organism.body.erase(std::remove(organism.body.begin(), organism.body.end(), elem), organism.body.end());
                                    }
                                }
                            }
                        }
                    }
                    world[point_of_attack[0]][point_of_attack[1]] = 5;
                    
                }
            }
        }

        if (invalid_move) {
            body = old_body;
            position = old_position;
        }

        body_positioner(old_position, position, old_body, body);
    }
    void evolve(std::vector<std::vector<int>>& list_of_position_value_in_single_organism_world, int type) {
        const std::vector<std::pair<std::vector<int>, int>> old_body = body;
        std::vector<std::vector<int>> single_body_world = single_body_positioner(body);

        for (int i = 1; i < 8; ++i) {
            for (int j = 1; j < 8; ++j) {
                if (single_body_world[i][j] != 0) {
                    list_of_position_value_in_single_organism_world[i - 1][j - 1] = 0;
                }
                bool all_neighbors_empty = true;
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        if (di == 0 && dj == 0) continue;
                        if (single_body_world[i + di][j + dj] != 0) {
                            all_neighbors_empty = false;
                            break;
                        }
                    }
                    if (!all_neighbors_empty) break;
                }
                if (all_neighbors_empty) {
                    list_of_position_value_in_single_organism_world[i - 1][j - 1] = 0;
                }
            }
        }

        std::vector<int> position_to_add_type = add_arrays_1d(max_position_finder_2d_matrix(list_of_position_value_in_single_organism_world), {-3, -3});
        body.push_back({position_to_add_type, type});       
        body_positioner(position, position, old_body, body);
    }
    std::vector<int> scan() {
        std::vector<int> type_distance;
        std::vector<std::vector<int>> directions = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

        for (auto direction : directions) {
            int distance = 1;
            while (true) {
                bool inbody = false;

                
                for (auto element : body) {
                    if (element.first == std::vector<int>{distance * direction[0], distance * direction[1]}) {
                        inbody = true;                        
                    }
                }

                
                std::vector<int> new_position = add_arrays_1d(position, {distance * direction[0], distance * direction[1]});
                int x = new_position[0];
                int y = new_position[1];

                // Check if the position is within bounds and not part of the organism's body
                
                if (world[x][y] != 0 && !inbody) {
                    type_distance.push_back(world[x][y]);
                    type_distance.push_back(distance);
                    break;
                }
                
                distance++;
                new_position = add_arrays_1d(position, {distance * direction[0], distance * direction[1]});
                x = new_position[0];
                y = new_position[1];
                if (x < 0 || y < 0 || x >= x_length_of_world || y >= y_length_of_world) {
                    type_distance.push_back(0);
                    type_distance.push_back(distance);
                    break;  
                }  
            }          
                

                
            
        }

        return type_distance;
        }

};


std::vector<Organism> Organism::organisms;



int main(){
    std::vector<int> no_of_success;    
    std::srand(std::time(0));
    for (int i = 0; i < 5; ++i) {
        int x = std::rand() % x_length_of_world;
        int y = 20 + (std::rand() % 21); 
        world[x][y] = 5;
    }  
    Organism Ram = Organism({2,2});   
    Organism Hari = Organism({5,2});
    Organism Shyam = Organism({2,5});
    Hari.body.push_back({{0,2},6});
    body_positioner(Hari.position, Hari.position, Hari.body, Hari.body);
    Organism Krishna = Organism({5,7});
    printworld();
    std::cout<<"--------------------------------------\n";
    Ram.move({0,0,0,3,1,2,0,1,0,2,0,2});
    printworld();
    std::cout<<"--------------------------------------\n";
    Hari.move({0,0,0,2,0,0,0,0,0,2,0,0});
    printworld();
    std::cout<<"--------------------------------------\n";
    Hari.move({0,0,0,2,0,0,0,0,0,2,0,0});
    printworld();
    std::cout<<"--------------------------------------\n";
    Krishna.move({0,0,0,0,0,0,0,2,0,2,0,0});
    printworld();
    std::cout<<"--------------------------------------\n";
    Krishna.move({0,0,0,2,0,0,0,0,0,2,0,0});
    Krishna.move({0,0,0,2,0,0,0,0,0,2,0,0});
    printworld();
    std::cout<<"--------------------------------------\n";
    std::cout<<Hari.energy<<"\n";
    Hari.move({0,0,0,0,0,2,0,0,0,2,0,0});
    Hari.move({0,0,0,2,0,0,0,0,0,2,0,0});
    Hari.move({0,0,0,2,0,0,0,0,0,2,0,0});
    Hari.move({0,2,0,0,0,0,0,0,0,2,0,0});
    printworld();
    std::cout<<"--------------------------------------\n";
    std::cout<<Hari.energy<<"\n";
    



}

