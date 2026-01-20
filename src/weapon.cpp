#include "src/weapon.h"
#include "src/game_root.h"



Weapon::Weapon(){
    
}

Weapon::Weapon(const Weapon& other) :
    GameObject(other) 
{
    if (other.collider) {
        collider = other.collider->clone(); 
        collider->parent = shared_from_this(); // clone returns unique_ptr<DynamicCollider>
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
    auto r = get_root();
    Vector2 mouse_pos = r->mouse_world_pos;
    direction = Vector2Normalize(mouse_pos - get_world_position());
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
    auto bullet = bullet_template->clone();
    if (!bullet) return;

    bullet->local_position = get_world_position() + direction * barrel_offset;
    bullet->rotation = rotation;
    auto r = get_root();
    r->instantiate(bullet);
}

shared_ptr<GameObject> Weapon::clone() const {

    //OPYAT kogda strelyaesh crash
    auto copy = make_shared<Weapon>(*this);
    // Reset parent and children, if needed
    copy->parent.reset();
    copy->children.clear();
    return copy;
}