#include "src/map.h"
#include <iostream>

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
    const int num_elements = map_width * map_height;
    int retries = 2;

    map_vec.resize(num_elements);
    std::vector<int> new_map(num_elements);

    // Инициализация случайным образом
    for (int i = 0; i < num_elements; i++) {
        map_vec[i] = rand_pick();  // например, WALL или AIR
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
                    if (map_vec[ny * map_width + nx] == WALL)
                        wall_count++;
                }

                int index = y * map_width + x;
                int cell = map_vec[index];

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
        map_vec.swap(new_map);
    }


    // create_texture();

    // Вывод карты
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            std::cout << (map_vec[y * map_width + x] == WALL ? '#' : '.');
        }
        std::cout << "\n";
    }
}


Texture2D Map::create_texture(){
    const int tile_size = 16;
    render_tex = LoadRenderTexture(map_width*tile_size,map_height*tile_size);
    BeginTextureMode(render_tex);
    ClearBackground(WHITE);
    for(int y = 0; y<map_height;y++){
        for(int x = 0;x<map_width;x++){
            int cell = map_vec[y*map_height+x];
            Vector2 pos = {(float)(x*tile_size),(float)(y*tile_size)};
            if (cell == WALL) {
            DrawRectangleV(pos, { (float)tile_size, (float)tile_size }, DARKGRAY);
        } else {
            DrawRectangleV(pos, { (float)tile_size, (float)tile_size }, LIGHTGRAY);
        }
        }
    }
    EndTextureMode();
    return render_tex.texture;
}

void Map::calculate_colliders(){
    
}

Map::Map(){

}
Map::~Map(){

}