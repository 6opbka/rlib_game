#include "src/map_gen.h"
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

char WALL = '#';
char AIR = '.';

int rand_pick(){
    int fill_prob = 45;
    if(rand()%100<fill_prob){
        return WALL;
    }
    else{
        return AIR;
    }
}



vector<Vector2> calculate_neighbours(int x, int y) {
    vector<Vector2> out;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < 64 && ny >= 0 && ny < 64) {
                out.push_back({(float)nx, (float)ny});
            }
        }
    }

    return out;
}

void Map::gen_map() {
    const int map_width = 64;
    const int map_height = 64;
    const int num_elements = map_width * map_height;
    int retries = 2;

    std::vector<int> map(num_elements);
    std::vector<int> new_map(num_elements);

    // Инициализация случайным образом
    for (int i = 0; i < num_elements; i++) {
        map[i] = rand_pick();  // например, WALL или AIR
    }

    // Один шаг клеточного автомата
    for(int i = 0; i<retries;i++){
        for (int y = 0; y < map_height; y++) {
            for (int x = 0; x < map_width; x++) {

                std::vector<Vector2> neighbours = calculate_neighbours(x,y);
                int wall_count = 0;

                for (Vector2 n : neighbours) {
                    int nx = (int)n.x;
                    int ny = (int)n.y;
                    if (nx < 0 || ny < 0 || nx >= map_width || ny >= map_height)
                        continue;
                    if (map[ny * map_width + nx] == WALL)
                        wall_count++;
                }

                int index = y * map_width + x;
                int cell = map[index];

                // Классические правила:
                if (cell == WALL) {
                    new_map[index] = (wall_count >= 5) ? WALL : AIR;
                } else {
                    new_map[index] = (wall_count >= 6) ? WALL : AIR;
            }
        }
    
    }

        for(int y = 0; y<map_height;y++){
        for(int x = 0; x<map_width;x++){
            if((x==0 || x==(map_width-1))||(y==0||y==(map_height-1))){
                new_map[y * map_width + x] = WALL;
            }
        }
    }
    map.swap(new_map);


    }
    

    //closing borders



    // Применяем изменения

    // Вывод карты
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            std::cout << (map[y * map_width + x] == WALL ? '#' : '.');
        }
        std::cout << "\n";
    }
}

Map::Map(){

}
Map::~Map(){

}