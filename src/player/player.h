#pragma once

struct player_t {
    worker_t *worker;
    canvas_t *canvas;
    bool toggle_light_button_is_pressed;
    bool step_button_is_pressed;
    bool run_button_is_pressed;
    bool end_button_is_pressed;

    bool is_running;
    size_t running_frame_count;
    size_t running_speed; // times per second

    hash_t *node_hash;
    hash_t *node_model_hash;
    node_physics_t *node_physics;
};

player_t *player_new(worker_t *worker);
void player_destroy(player_t **self_pointer);

void player_toggle_light(player_t *self);
bool player_is_any_button_pressed(player_t *self);

hash_t *player_new_node_hash(player_t *self);
void player_update(player_t *self);

void player_start(worker_t *worker);
