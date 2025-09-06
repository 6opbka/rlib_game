#include "src/weapon.h"
#include "src/game_root.h"



Weapon::Weapon(){
    scale = {0.5f,0.5f};
}

Weapon::Weapon(const Weapon& other) :
    GameObject(other) 
{
    if (other.collider) {
        collider = other.collider->clone(); 
        collider->parent = shared_from_this(); // clone returns unique_ptr<Collider>
    } else {
        collider.reset();
    }
    // Note: parent and children are handled by GameObject copy constructor or cleared elsewhere
}

Weapon::~Weapon(){}


void Weapon::update(const float delta_time){
    read_input();
    follow_mouse();
    
}

void Weapon::follow_mouse(){
    Vector2 mouse_pos = GetMousePosition();
    direction = vector2_normalize(mouse_pos - get_world_position());
    float angle_rad = std::atan2(direction.y, direction.x);
    float angle_deg = angle_rad * 180.0f / PI;
    if (angle_deg < 0) angle_deg += 360.0f;
    rotation = angle_deg;
    // std::cout << "angle : " << angle_deg << "\n";
    sprite->flip_y = (angle_deg < 270 && angle_deg > 90);
    

}

void Weapon::read_input(){
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        shoot();
    }
}

void Weapon::initialize(shared_ptr<GameRoot> _root, shared_ptr<Bullet> bullet) {
    root = _root;
    bullet_template = bullet;
}

void Weapon::shoot() {
    auto new_bullet = bullet_template->clone();
    if (!new_bullet) return;
    
    new_bullet->local_position = get_world_position() + direction * barrel_offset;
    new_bullet->rotation = rotation;
    root->instantiate(new_bullet);  // Single instantiation call
}

shared_ptr<GameObject> Weapon::clone() const {

    //OPYAT kogda strelyaesh crash
    auto copy = make_shared<Weapon>(*this);
    // Reset parent and children, if needed
    copy->parent.reset();
    copy->children.clear();
    return copy;
}