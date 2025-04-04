#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <time.h>

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

void add_to_visited(struct linked_list *visited, struct game_state curr) {
    int steps = curr.num_steps;
    curr.num_steps = 0;
    size_t serial = (size_t) serialize(curr);
    insert_at_head(visited, serial);
    curr.num_steps = steps;
}

struct list_node *list_search(struct linked_list *list, size_t v) {
    struct list_node * p = list->head;
    while (p != NULL)
    {
      if ((p->value) == v) return p;
      p = p->next;
    }
    return p;
}

bool in_visited(struct linked_list *visited, struct game_state state) {
    int steps = state.num_steps;
    state.num_steps = 0;
    size_t serial = (size_t) serialize(state);
    state.num_steps = steps;
    if(list_search(visited, serial) != NULL) return true;
    return false;
}


int final_state(struct game_state state) {
    if (state.empty_col != 3 || state.empty_row != 3) return false;
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            if (i == 3 && j == 3) continue;
            if (state.tiles[i][j] != (i*4 + j + 1)) return false;
        }
    }
    return true;
}

int number_of_moves(struct game_state start) {
    //clock_t start_time = clock();
    //Initializing queue
    struct linked_list * queue_list = malloc(sizeof(struct linked_list));
    queue_list -> head = NULL;
    struct queue * state_queue = malloc(sizeof(struct queue));
    state_queue -> data = *queue_list;
    
    //Enquing first state
    enqueue(state_queue,start);
    struct linked_list * visited_list = malloc(sizeof(struct linked_list));
    visited_list -> head = NULL;
    //Running BFS - Assuming there is a solution, it will run forever if no solution exist
    int num_moves = -1;
    while (true)
    {
        //double seconds = (double) (clock()-start_time)/CLOCKS_PER_SEC;
        //if (seconds > 50) break;
        //Popping current state
        struct game_state curr = dequeue(state_queue);
        add_to_visited(visited_list,curr);
        uint64_t serial_curr = serialize(curr);
        //Testing if we have found the solution
        if (final_state(curr))
        {
            num_moves = curr.num_steps;
            break;
        }
        
        //Applying moves to state
        struct game_state up = deserialize(serial_curr);
        move_up(&up);
        struct game_state down = deserialize(serial_curr);
        move_down(&down);
        struct game_state left = deserialize(serial_curr);
        move_left(&left);
        struct game_state right = deserialize(serial_curr);
        move_right(&right);
        //Queueing new states
        if (serial_curr != serialize(up) && !in_visited(visited_list,up)) enqueue(state_queue,up);
        if (serial_curr != serialize(down) && !in_visited(visited_list,down)) enqueue(state_queue,down);
        if (serial_curr != serialize(left) && !in_visited(visited_list,left)) enqueue(state_queue,left);
        if (serial_curr != serialize(right) && !in_visited(visited_list,right)) enqueue(state_queue,right);
    }

    //Freeing used structures
    free_list(state_queue->data); //Frees remaining nodes in queue after solution found
    free(queue_list);   //Frees the list structure (same as free(state_queue->data);)
    free(state_queue);  //Frees the queue structure
    free_list(*visited_list);
    free(visited_list);
    return num_moves;
}