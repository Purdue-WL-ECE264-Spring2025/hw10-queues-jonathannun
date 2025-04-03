#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) { 
  struct list_node * n = malloc(sizeof(struct list_node));
  n -> value = value;
  n -> next = NULL;
  return n;
}

void insert_at_head(struct linked_list *list, size_t value) {
  //Constucting new head
  struct list_node * new_head = new_node(value);
  //Setting next of new head to old head
  new_head -> next = list -> head;
  //Setting new head as the head
  list -> head = new_head;
}

void insert_at_tail(struct linked_list *list, size_t value) {
  //If the list is empty we can not search throug it for the tail
  if (list->head == NULL)
  {
    insert_at_head(list,value);
    return;
  }
  
  //Setting up pointer to head
  struct list_node * head = list->head;
  //Finding tail
  while (head -> next != NULL)
  {
    head = head->next;
  }
  //Adding new node to tail
  struct list_node * add_node = new_node(value);
  head->next = add_node;
}

size_t remove_from_head(struct linked_list *list) { 
  //Making sure there is a head to remove
  if (list->head == NULL) return -1;
  
  //Retriving head
  struct list_node * delete = list->head;
  //Setting new head of list to next in list
  list->head = delete -> next;
  //Retriving and freeing the head to be removed
  size_t value = delete -> value;
  free(delete);

  return value;
}

size_t remove_from_tail(struct linked_list *list) { 
  //Making sure there is a head to remove
  if (list->head == NULL) return -1;
  if (list->head->next == NULL) {
    //There is only one value in list which should be removed
    return remove_from_head(list);
  }

  //Setting up the pointer which should be removed, and the pointer which next has to be updated to null
  struct list_node * update = list->head;
  struct list_node * delete = update->next;
  //Finding the real end of the list
  while (delete->next != NULL)
  {
    update = delete;
    delete = update -> next;
  }
  //Retriving value, updating the new end of the list to point to null, freeing delete
  size_t value = delete->value;
  update->next = NULL;
  free(delete);

  return value; 
}

void free_list(struct linked_list list) {
  //Making sure there is a list to free
  if (list.head == NULL) return;
  //Setting up pointers to keep track
  struct list_node * head = list.head;
  struct list_node * next = NULL;
  //Freeing each node in list
  while (head -> next != NULL)
  {
    next = head -> next; //Saving next in a pointer
    free(head); //Freeing node
    head = next; //Resetting head to next node to be freed
  }
  //Freeing last node for which next == null
  free(head);
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
