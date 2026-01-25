#include "src/level_map.h"
#include <iostream>
#include <stack>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <memory>
#include "src/static_line_collider.h"
#include "src/static_collider.h"

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

void LevelMap::gen_map() {
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

void LevelMap::normalize_edge(Line& edge){
    if(edge.a.x>edge.b.x||edge.a.y>edge.b.y){
        swap(edge.a,edge.b);    
    }
}


Texture2D LevelMap::create_texture(){
    
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

void LevelMap::calculate_edges(){
    vector<bool> visited(map_height*map_width);

    // Calculating raw_islands 
    vector<vector<Vector2>> raw_islands;
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
                raw_islands.push_back(island);
            }
        }
    }
    // Calculating island colliders
    int i = 0;
    
    // vector<vector<Line>> islands(raw_islands.size());
    islands.resize(raw_islands.size());
    for(auto island : raw_islands){
        vector<Line> island_edges;
        for(Vector2& pos : island){
            // cout<<"("<<pos.x<<", "<<pos.y<<"), ";
            int x = pos.x;
            int y = pos.y;

            Vector2 tl = {x*tile_size, y*tile_size};
            Vector2 tr = {(x+1)*tile_size, y*tile_size};
            Vector2 bl = {x*tile_size, (y+1)*tile_size};
            Vector2 br = {(x+1)*tile_size, (y+1)*tile_size};
            // UP
            if (!inside(x, y-1) || map_vec[(y-1)*map_width + x] != WALL) {
                island_edges.push_back({ tl, tr });
            }

            // DOWN
            if (!inside(x, y+1) || map_vec[(y+1)*map_width + x] != WALL) {
                island_edges.push_back({ bl, br });
            }

            // LEFT
            if (!inside(x-1, y) || map_vec[y*map_width + (x-1)] != WALL) {
                island_edges.push_back({ tl, bl });
            }

            // RIGHT
            if (!inside(x+1, y) || map_vec[y*map_width + (x+1)] != WALL) {
                island_edges.push_back({ tr, br });
            }
        }
        islands[i] = island_edges;
        i++;

    }

    // Normalizing egdes
    for (auto& island : islands){
        for(Line& edge : island){
            normalize_edge(edge);
        }
    }

    // Merging lines 
    // merged_islands.resize(islands.size());

    for (const auto& island : islands){
        map<int,vector<Line>> hor_edges;
        map<int,vector<Line>> ver_edges;

        for(const Line& edge : island){
            if(edge.a.y==edge.b.y){
                hor_edges[edge.a.y].push_back(edge);
            }
            else if(edge.a.x==edge.b.x){
                ver_edges[edge.a.x].push_back(edge);
            }
        }

        for(auto& [y, edges] : hor_edges){
            merge_collinear(edges,all_edges_hor,true);
        }
        for(auto& [x,edges] : ver_edges){
            merge_collinear(edges,all_edges_ver,false);
        }
    }
    cout<<"num raw_islands: "<<raw_islands.size()<<endl;
    cout<<"num islands: "<<islands.size()<<endl;

}

void LevelMap::generate_collider_list(){
    for(Line& edge:all_edges_ver){
        AABB aabb = aabb_from_line(edge);

        std::unique_ptr<StaticLineCollider> col = make_unique<StaticLineCollider>(LAYER_WALL,LAYER_NONE,LINE,edge,aabb);
        static_colliders.push_back(std::move(col));
    }

    for(Line& edge:all_edges_hor){
        AABB aabb = aabb_from_line(edge);

        std::unique_ptr<StaticLineCollider> col = make_unique<StaticLineCollider>(LAYER_WALL,LAYER_NONE,LINE,edge,aabb);
        static_colliders.push_back(std::move(col));
    }

    cout<<"num all_edges_hor: "<<all_edges_hor.size()<<endl;
    cout<<"num all_edges_ver: "<<all_edges_ver.size()<<endl;
    cout<<"num colliders: "<<static_colliders.size()<<endl;

}


Texture2D LevelMap::redraw_colliders_as_tex(){
    
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
    int i = 1;
    for (const auto& edge : all_edges_ver) {
    

    unsigned char r = (unsigned char)((i * 50) % 256);
    unsigned char g = (unsigned char)((i * 80) % 256);
    unsigned char b = (unsigned char)((i * 110) % 256);

    Color c = { r, g, b, 255 };

    DrawLineV(edge.a, edge.b, c);
    i++;
    
    }
    for (const auto& edge : all_edges_hor) {
    
    unsigned char r = (unsigned char)((i * 50) % 256);
    unsigned char g = (unsigned char)((i * 80) % 256);
    unsigned char b = (unsigned char)((i * 110) % 256);
    Color c = { r, g, b, 255 };

    DrawLineV(edge.a, edge.b, c);
    i++;
    }
    EndTextureMode();
    return render_tex.texture;
}

bool LevelMap::inside(int x, int y){
    return (x>=0&&x<map_width&&
            y>=0&&y<map_height);
}

void LevelMap::merge_collinear(vector<Line>& edges, vector<Line>& merged, bool horizontal){
    if (edges.empty()) return;

    // 1. Сортировка
    std::sort(edges.begin(), edges.end(),
        [horizontal](const Line& e1, const Line& e2) {
            return horizontal
                ? e1.a.x < e2.a.x
                : e1.a.y < e2.a.y;
        });

    Line current = edges[0];

    // 2. Линейный проход
    for (int i = 1; i < edges.size(); i++) {
        float curr_end = horizontal ? current.b.x : current.b.y;
        float next_start = horizontal ? edges[i].a.x : edges[i].a.y;

        if (next_start <= curr_end) {
            // расширяем текущий
            if (horizontal)
                current.b.x = std::max(current.b.x, edges[i].b.x);
            else
                current.b.y = std::max(current.b.y, edges[i].b.y);
        } else {
            merged.push_back(current);
            current = edges[i];
        }
    }
    // calculating length
    float dist_x = current.a.x-current.b.x;
    float dist_y = current.a.y-current.b.y;
    float len = sqrt((dist_x*dist_x)+(dist_y*dist_y));
    cout<<len<<endl;
    current.length = len;
    merged.push_back(current);
}

void LevelMap::grid_add_colliders(){
    
    int grid_w = pixel_width  / spatial_collider_grid_size;
    int grid_h = pixel_height / spatial_collider_grid_size;

    static_grid.clear();
    static_grid.resize(grid_w*grid_h);

    for (auto& col: static_colliders){
        grid_add_wall(col.get());
    }

    int non_empty = 0;
int total = 0;

for (auto& cell : static_grid) {
    if (!cell.empty()) {
        non_empty++;
        total += cell.size();
    }
}

cout << "avg per non-empty cell: "
     << (float)total / non_empty << endl;
    // cout<<"colliders in cells: "<<colliders_in_cells<<endl;
    
}

void LevelMap::grid_add_wall(StaticCollider* col)
{
    AABB box = col->get_aabb();

    int grid_w = pixel_width  / spatial_collider_grid_size;
    int grid_h = pixel_height / spatial_collider_grid_size;

    int x0 = static_cast<int>(box.min.x) / spatial_collider_grid_size;
    int y0 = static_cast<int>(box.min.y) / spatial_collider_grid_size;
    int x1 = static_cast<int>(box.max.x) / spatial_collider_grid_size;
    int y1 = static_cast<int>(box.max.y) / spatial_collider_grid_size;

    x0 = clamp(x0, 0, grid_w - 1);
    x1 = clamp(x1, 0, grid_w - 1);
    y0 = clamp(y0, 0, grid_h - 1);
    y1 = clamp(y1, 0, grid_h - 1);

    for (int y = y0; y <= y1; ++y){
        for (int x = x0; x <= x1; ++x){
            static_grid[y * grid_w + x].push_back(col);
            colliders_in_cells ++;
        }
    }

}

AABB LevelMap::aabb_from_line(const Line& line){
    AABB box;
    box.min.x = min(line.a.x,line.b.x);
    box.min.y = min(line.a.y,line.b.y);
    box.max.x = max(line.a.x,line.b.x);
    box.max.y = max(line.a.y,line.b.y);
    return box;
}


LevelMap::LevelMap(){
    pixel_width  = map_width  * tile_size;
    pixel_height = map_height * tile_size;

}
LevelMap::~LevelMap(){

}