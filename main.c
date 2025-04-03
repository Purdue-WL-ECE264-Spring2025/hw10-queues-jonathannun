#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [test file]\n", argv[0]);
    exit(1);
  }

  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    fprintf(stderr, "Failed to open input file %s\n", argv[1]);
    exit(2);
  }

  int expected;
  fscanf(fp, "%d", &expected);

  struct game_state start;
  start.num_steps = 0;
  for (uint8_t i = 0; i < 4; i++) {
    for (uint8_t j = 0; j < 4; j++) {
      int value;
      fscanf(fp, "%d", &value);
      start.tiles[i][j] = value;
      if (value == 0) {
        start.empty_row = i;
        start.empty_col = j;
      }
    }
  }
  fclose(fp);

  //--------------DEBUGGING AREA
  //Initializing tester list
  struct linked_list * tester_list = malloc(sizeof(struct linked_list));
  tester_list -> head = NULL;
  //Adding nodes
  insert_at_tail(tester_list, 1);
  insert_at_tail(tester_list, 2);
  insert_at_tail(tester_list, 3);
  insert_at_tail(tester_list, 4);
  //Printing result
  dump_list(stderr,*tester_list);
  //Removing nodes
  remove_from_tail(tester_list);
  dump_list(stderr,*tester_list);
  remove_from_tail(tester_list);
  dump_list(stderr,*tester_list);
  remove_from_tail(tester_list);
  dump_list(stderr,*tester_list);
  remove_from_tail(tester_list);
  //Printing remaining list
  dump_list(stderr,*tester_list);
  //Freeing list
  free_list(*tester_list);
  free(tester_list);

  //--------------END OF DEBUGGING AREA

  int output = number_of_moves(start);
  if (output != expected) {
    fprintf(stderr, "Expected %d moves, got %d\n", expected, output);
    exit(3);
  }

  return 0;
}
