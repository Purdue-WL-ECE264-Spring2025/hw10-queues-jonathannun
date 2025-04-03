#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>

void enqueue(struct queue *q, struct game_state state) {
    //Serializing game state
    size_t stored_state = (size_t) serialize(state); //We must assume that the system is a 64-bit system
    //Enquing with a new list node
    insert_at_tail(&(q->data),stored_state);
}

struct game_state dequeue(struct queue *q) { 
    size_t serialized_state = remove_from_head(&(q->data));
    return deserialize(serialized_state);
}

void print_game_state(struct game_state state) {
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            fprintf(stderr,"%d,",state.tiles[i][j]);
        }
        fprintf(stderr,"\n");
    }
    fprintf(stderr,"empty: %d,%d\n",state.empty_row,state.empty_col);
    fprintf(stderr,"num_steps: %d\n",state.num_steps);
}

struct game_state copy_state(struct game_state state) {
    struct game_state copy;
    copy.num_steps = state.num_steps;
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            copy.tiles[i][j] = state.tiles[i][j];
        }
    }
    copy.empty_col = state.empty_col;
    copy.empty_row = state.empty_row;
    return copy;
}

int final_state(struct game_state state) {
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            if (i == 3 && j == 3) continue;
            if (state.tiles[i][j] != (i*4 + j + 1)) return false;
        }
    }
    return true;
}

int number_of_moves(struct game_state start) {
    //Initializing queue
    struct linked_list * queue_list = malloc(sizeof(struct linked_list));
    queue_list -> head = NULL;
    struct queue * state_queue = malloc(sizeof(struct queue));
    state_queue -> data = *queue_list;
    
    //Enquing first state
    enqueue(state_queue,start);

    //Running BFS - Assuming there is a solution, it will run forever if no solution exist
    int num_moves = -1;
    while (true)
    {
        //Popping current state
        struct game_state curr = dequeue(state_queue);
        //Testing if we have found the solution
        if (final_state(curr))
        {
            num_moves = curr.num_steps;
            break;
        }
        
        //Applying moves to state
        struct game_state up = copy_state(curr);
        move_up(&up);
        struct game_state down = copy_state(curr);
        move_down(&down);
        struct game_state left = copy_state(curr);
        move_left(&left);
        struct game_state right = copy_state(curr);
        move_right(&right);
        //Queueing new states
        if (serialize(curr) != serialize(up)) enqueue(state_queue,up);
        if (serialize(curr) != serialize(down)) enqueue(state_queue,down);
        if (serialize(curr) != serialize(left)) enqueue(state_queue,left);
        if (serialize(curr) != serialize(right)) enqueue(state_queue,right);
    }

    //Freeing used structures
    free_list(state_queue->data); //Frees remaining nodes in queue after solution found
    free(queue_list);   //Frees the list structure (same as free(state_queue->data);)
    free(state_queue);  //Frees the queue structure
    return num_moves;
}
