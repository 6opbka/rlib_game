#include "src/map.h"
#include <iostream>
#include <stack>
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

    for (int y_offset = -1; y_offset <= 1; ++y_offset) {
        for (int x_offset = -1; x_offset <= 1; ++x_offset) {
            int n_x = x + x_offset;
            int n_y = y + y_offset;

            if (n_x >= 0 && n_x < 64 && n_y >= 0 && n_y < 64) {
                out.push_back({(float)n_x, (float)n_y});
            }
        }
    }

    return out;
}

void MapGen::gen_map() {
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
                    int n_x = (int)n.x;
                    int n_y = (int)n.y;
                    if (n_x < 0 || n_y < 0 || n_x >= map_width || n_y >= map_height)
                        continue;
                    if (map_vec[n_y * map_width + n_x] == WALL)
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


Texture2D MapGen::create_texture(){
    const int tile_size = 16;
    render_tex = LoadRenderTexture(map_width*tile_size,map_height*tile_size);
    BeginTextureMode(render_tex);
    ClearBackground(WHITE);
    for(int y = 0; y<map_height;y++){
        for(int x = 0;x<map_width;x++){
            int cell = map_vec[y*map_width+x];
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

void MapGen::calculate_colliders(){
    vector<bool> visited(map_height*map_width);

    vector<vector<Vector2>> islands;

    for(int y = 0; y<map_height; y++){
        for(int x = 0; x<map_width; x++){
            int index = y*map_width+x;
            if(map_vec[index]==WALL && !visited[index]){
                vector<Vector2> island;
                stack<Vector2> q;
                q.push(Vector2{(float)x,(float)y});
                visited[index] = true;

                while(!q.empty()){
                    Vector2 cur = q.top();
                    q.pop();
                    island.push_back(cur);
                    static const int x_offset[4] = {1,-1,0,0};
                    static const int y_offset[4] = {0,0,1,-1};
                    for(int i =0; i<4;i++){
                        int n_x = cur.x+x_offset[i];
                        int n_y = cur.y+y_offset[i];

                        if(!inside(n_x,n_y)) continue;
                        
                            int neighbour_idx = n_y*map_width+n_x;
                            if(!visited[neighbour_idx]&&map_vec[neighbour_idx]==WALL){
                                visited[neighbour_idx] = true;
                                q.push({(float)n_x,(float)n_y});
                            }


                    }

                }
                islands.push_back(island);

            }
        }
    }

    cout<<"num islands: "<<islands.size()<<endl;
}

bool MapGen::inside(int x, int y){
    return (x>=0&&x<map_width&&
            y>=0&&y<map_height);
}

MapGen::MapGen(){

}
MapGen::~MapGen(){

}