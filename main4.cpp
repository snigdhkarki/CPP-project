#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

int x_length_of_world = 60;
int y_length_of_world = 60;
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 650;
int FONT_SIZE = 20;
SDL_Color WHITE = {255, 255, 255};
SDL_Color BLACK = {0, 0, 0};

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
        organisms.push_back(this);
    }

    
    int energy;
    std::vector<int> position;
    std::vector<std::pair<std::vector<int>, int>> body;    
    static std::vector<Organism*> organisms;

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
                    for (Organism* organism : organisms) {
                        
                        if (organism != this) {
                            for (auto elem : organism->body) {
                                if (add_arrays_1d(elem.first, organism->position) == point_of_attack) {
                                    if (elem.second == 1) {
                                        for (auto elem_body : organism->body) {
                                            world[add_arrays_1d(elem_body.first, organism->position)[0]][add_arrays_1d(elem_body.first, organism->position)[1]] = 5;
                                        }
                                        organism->body.clear();
                                    } else {
                                        organism->body.erase(std::remove(organism->body.begin(), organism->body.end(), elem), organism->body.end());
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


std::vector<Organism*> Organism::organisms;

Organism Ram = Organism({30,8});
Organism Shyam = Organism({30,52});

int find_num_of_legs(Organism organism) {
    int no_of_legs = 0;
    for (auto element : organism.body) {
        if (element.second == 3) { 
            ++no_of_legs;
        }
    }
    return no_of_legs;
}

void list_to_evolution_caller(std::vector<int> list) {
    int num_chunks = list.size() / 3;
    for (int i = 0; i < num_chunks; ++i) {
        
        std::vector<std::vector<int>> matrix(7, std::vector<int>(7, 0));        
        matrix[3 + list[1 + i * 3]][3 + list[0 + i * 3]] = 1;       
        Ram.evolve(matrix, list[2 + i * 3]);
    }
}

void list_to_evolution_caller2(std::vector<int> list) {
    int num_chunks = list.size() / 3;
    for (int i = 0; i < num_chunks; ++i) {
        
        std::vector<std::vector<int>> matrix(7, std::vector<int>(7, 0));        
        matrix[3 + list[1 + i * 3]][3 + list[0 + i * 3]] = 1;       
        Shyam.evolve(matrix, list[2 + i * 3]);
    }
}

void display_opening_screen(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, 255);
    SDL_RenderClear(renderer);

    SDL_Surface* title_surface = TTF_RenderText_Solid(font, "K_Up/W: up, K_Down/S: down, K_Left/A: left, K_Right/D: right", BLACK);
    if (!title_surface) {
        std::cerr << "Failed to create title surface: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    SDL_Rect title_rect = { (SCREEN_WIDTH - title_surface->w) / 2, 50, title_surface->w, title_surface->h };
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);

    SDL_Surface* title_surface2 = TTF_RenderText_Solid(font, "up and down at once for clockwise rotation, right and left at once for anticlockwise rotation", BLACK);
    if (!title_surface2) {
        std::cerr << "Failed to create title surface2: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* title_texture2 = SDL_CreateTextureFromSurface(renderer, title_surface2);
    SDL_Rect title_rect2 = { (SCREEN_WIDTH - title_surface2->w) / 2, 90, title_surface2->w, title_surface2->h };
    SDL_RenderCopy(renderer, title_texture2, NULL, &title_rect2);

    SDL_Surface* instructions_surface = TTF_RenderText_Solid(font, "Enter a list of numbers separated by spaces for evolution of left then right:", BLACK);
    if (!instructions_surface) {
        std::cerr << "Failed to create instructions surface: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* instructions_texture = SDL_CreateTextureFromSurface(renderer, instructions_surface);
    SDL_Rect instructions_rect = { (SCREEN_WIDTH - instructions_surface->w) / 2, 200, instructions_surface->w, instructions_surface->h };
    SDL_RenderCopy(renderer, instructions_texture, NULL, &instructions_rect);

    SDL_RenderPresent(renderer);

    SDL_FreeSurface(title_surface);
    SDL_DestroyTexture(title_texture);
    SDL_FreeSurface(title_surface2);
    SDL_DestroyTexture(title_texture2);
    SDL_FreeSurface(instructions_surface);
    SDL_DestroyTexture(instructions_texture);
}

std::vector<int> get_user_input(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Rect input_box = { (SCREEN_WIDTH - 300) / 2, 250, 300, 40 };
    std::string input_text = "";
    bool input_active = true;

    while (input_active) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_Quit();
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN) {
                    std::vector<int> numbers;
                    try {
                        size_t pos = 0;
                        while ((pos = input_text.find(" ")) != std::string::npos) {
                            numbers.push_back(std::stoi(input_text.substr(0, pos)));
                            input_text.erase(0, pos + 1);
                        }
                        numbers.push_back(std::stoi(input_text));
                        return numbers;
                    } catch (...) {
                        std::cout << "Invalid input. Please enter numbers separated by spaces." << std::endl;
                    }
                } else if (e.key.keysym.sym == SDLK_BACKSPACE && input_text.length() > 0) {
                    input_text.pop_back();
                } else if (e.key.keysym.sym == SDLK_SPACE) {
                    input_text += ' ';
                } else {
                    input_text += e.key.keysym.sym;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, 255);
        SDL_RenderClear(renderer);
        display_opening_screen(renderer, font);

        SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, 255);
        SDL_RenderDrawRect(renderer, &input_box);
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, input_text.c_str(), BLACK);
        if (!text_surface) {
            std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
            continue;
        }
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_Rect text_rect = { input_box.x + 5, input_box.y + 10, text_surface->w, text_surface->h };
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        SDL_RenderPresent(renderer);

        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }

    return std::vector<int>();
}
// void start_game(const std::vector<int>& numbers, SDL_Renderer* renderer) {
//     std::vector<std::vector<std::vector<int>>> array = map_colors(world);
//     SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, 255);
//     SDL_RenderClear(renderer);
//     std::cout << "Starting game with numbers: ";
//     for (int num : numbers) {
//         std::cout << num << " ";
//     }
//     std::cout << std::endl;

//     SDL_Event e;
//     bool running = true;
//     while (running) {
//         while (SDL_PollEvent(&e)) {
//             if (e.type == SDL_QUIT) {
//                 running = false;
//             }
//         }

//         SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, 255);
//         SDL_RenderClear(renderer);

//         // Dummy draw array
//         for (size_t i = 0; i < array.size(); ++i) {
//             for (size_t j = 0; j < array[i].size(); ++j) {
//                 SDL_SetRenderDrawColor(renderer, array[i][j][0], array[i][j][1], array[i][j][2], 255);
//                 SDL_Rect cell_rect = { static_cast<int>(j * 10), static_cast<int>(i * 10), 10, 10 };
//                 SDL_RenderFillRect(renderer, &cell_rect);
//             }
//         }

//         SDL_RenderPresent(renderer);
//         SDL_Delay(100);
//     }
// }

void draw_array(SDL_Renderer* renderer, std::vector<std::vector<std::vector<int>>> array){
        for (size_t i = 0; i < array.size(); ++i) {
            for (size_t j = 0; j < array[i].size(); ++j) {
                SDL_SetRenderDrawColor(renderer, array[i][j][0], array[i][j][1], array[i][j][2], 255);
                SDL_Rect cell_rect = { static_cast<int>(j * 10), static_cast<int>(i * 10), 10, 10 };
                SDL_RenderFillRect(renderer, &cell_rect);
            }
        }
        SDL_RenderPresent(renderer);
}

void start_game(const std::vector<int>& numbers, SDL_Renderer* renderer) {         
    std::vector<std::vector<std::vector<int>>> array = map_colors(world);    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);    
    std::cout << "Starting game with numbers:";
    for (int number : numbers) {
        std::cout << " " << number;
    }
    std::cout << std::endl;    
    SDL_Event e;
    bool quit = false;

    

    // Define movement functions similar to the Python version
    auto go_up = [&]() { Ram.move({0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_down = [&]() { Ram.move({0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_left = [&]() { Ram.move({0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_right = [&]() { Ram.move({0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_up_right = [&]() { Ram.move({0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_right_down = [&]() { Ram.move({0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_down_left = [&]() { Ram.move({0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_left_up = [&]() { Ram.move({0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}); array = map_colors(world); };
    auto go_clockwise = [&]() { Ram.move({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}); array = map_colors(world); };
    auto go_anticlockwise = [&]() { Ram.move({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}); array = map_colors(world); };

    auto go_up2 = [&]() { Shyam.move({0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_down2 = [&]() { Shyam.move({0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_left2 = [&]() { Shyam.move({0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_right2 = [&]() { Shyam.move({0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_up_right2 = [&]() { Shyam.move({0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_right_down2 = [&]() { Shyam.move({0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_down_left2 = [&]() { Shyam.move({0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}); array = map_colors(world); };
    auto go_left_up2 = [&]() { Shyam.move({0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}); array = map_colors(world); };
    auto go_clockwise2 = [&]() { Shyam.move({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}); array = map_colors(world); };
    auto go_anticlockwise2 = [&]() { Shyam.move({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}); array = map_colors(world); };

    auto handle_events = [&]() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
        }
        return true;
    };

    auto handle_continuous_keys = [&](const Uint8* keys) {
        if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_RIGHT]) go_up_right();
        if (keys[SDL_SCANCODE_RIGHT] && keys[SDL_SCANCODE_DOWN]) go_right_down();
        if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_LEFT]) go_down_left();
        if (keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_UP]) go_left_up();
        if (keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT]) go_anticlockwise();
        if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_UP]) go_clockwise();
        if (keys[SDL_SCANCODE_UP]) go_up();
        if (keys[SDL_SCANCODE_DOWN]) go_down();
        if (keys[SDL_SCANCODE_RIGHT]) go_right();
        if (keys[SDL_SCANCODE_LEFT]) go_left();
    };

    auto handle_continuous_keys2 = [&](const Uint8* keys) {
        if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_D]) go_up_right2();
        if (keys[SDL_SCANCODE_D] && keys[SDL_SCANCODE_S]) go_right_down2();
        if (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_A]) go_down_left2();
        if (keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_W]) go_left_up2();
        if (keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_D]) go_anticlockwise2();
        if (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_W]) go_clockwise2();
        if (keys[SDL_SCANCODE_W]) go_up2();
        if (keys[SDL_SCANCODE_S]) go_down2();
        if (keys[SDL_SCANCODE_D]) go_right2();
        if (keys[SDL_SCANCODE_A]) go_left2();
    };

    while (!quit) {
        quit = !handle_events();
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        handle_continuous_keys(keys);
        handle_continuous_keys2(keys);
        draw_array(renderer, array);
        SDL_Delay(100); 
    }    
}

int main(){

    std::vector<int> no_of_success;    
    std::srand(std::time(0));
    for (int i = 0; i < 5; ++i) {
        int x = std::rand() % x_length_of_world;
        int y = 20 + (std::rand() % 21); 
        world[x][y] = 5;
    } 
    
     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Opening Screen Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf", FONT_SIZE);
    if (font == nullptr) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return -1;
    }

    display_opening_screen(renderer, font);
    std::vector<int> numbers = get_user_input(renderer, font);    
    list_to_evolution_caller(numbers);
    display_opening_screen(renderer, font);
    std::vector<int> numbers2 = get_user_input(renderer, font);
    list_to_evolution_caller2(numbers2);
    start_game(numbers, renderer);
    // for(int num:numbers){
    //     std::cout<< num<<std::endl;
    // }
    // for(int num:numbers2){
    //     std::cout<< num<<std::endl;
    // }

    

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit(); 
    
    



}

