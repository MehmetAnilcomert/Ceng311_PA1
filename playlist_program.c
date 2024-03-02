#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

/*
    @author Mehmet Anıl Cömert

    This is an assignment given in Ceng 311 Computer Architecture course. This program creates an array of playlists.
  Each playlist is a simple linked list, with each node’s data pointing to a song_t object. Program creates 5 playlists by using 
  create_link_list and inserts them into array_of_playlist_ptrs by using set_element_of_array_of_linked_list_ptrs. 
  Then, resizes array_of_playlist_ptrs to a size of 10 by using resize_array_of_linked_list_ptrs, creates five more playlists,
  and inserts them into available areas in array_of_playlist_ptrs. Finally, removes the second song of each playlist by using 
  remove_from_linked_list and destroys them. 


*/


typedef struct linked_list_node_t {
    void* data;
    struct linked_list_node_t* next;
} linked_list_node_t;

void create_array_of_linked_list_ptrs(linked_list_node_t*** destination, int size) {
    *destination = (linked_list_node_t**) malloc(size * sizeof(linked_list_node_t*));
    for (int i = 0; i < size; i++) {
        (*destination)[i] = NULL;       // assigning NULL to each element
    }
}

void get_element_of_array_of_linked_list_ptrs(linked_list_node_t** array, int index, linked_list_node_t** destination){
    *destination = array[index];
}

void set_element_of_array_of_linked_list_ptrs(linked_list_node_t** array, int index, linked_list_node_t* head) {
    array[index] = head;
}


void destroy_array_of_linked_list_ptrs(linked_list_node_t*** array) {
    free(*array);
    *array = NULL;
}

void resize_array_of_linked_list_ptrs(linked_list_node_t*** destination, linked_list_node_t** array, int size, int new_size){
    *destination = (linked_list_node_t**) realloc(array, new_size * sizeof(linked_list_node_t*));
    for (int i = size; i < new_size; i++) {
        (*destination)[i] = NULL;               // assigning NULL to each new elements
    }
}

void create_link_list(linked_list_node_t** destination){
    *destination = NULL;
}

void destroy_linked_list(linked_list_node_t** head) {
    linked_list_node_t* temp = *head;
    while (temp != NULL) {
        linked_list_node_t* next = temp->next;
        free(temp);
        temp = next;
    }
    *head = NULL;
}

void add_to_linked_list(linked_list_node_t** head, const void* data) {
    // Creating a new node and allocating memory for it
    linked_list_node_t* new_node = (linked_list_node_t*)malloc(sizeof(linked_list_node_t));

    // Setting the data of the new node
    new_node->data = (void*)data;
    new_node->next = NULL;

    // If the head is NULL, this is the first node
    if (*head == NULL) {
        *head = new_node;
    } else {
        // Finding the last node in the list
        linked_list_node_t* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Adding the new node at the end of the linked list
        current->next = new_node;
    } 
}    


void remove_from_linked_list(linked_list_node_t** destination, linked_list_node_t* head, const void* data) {
    linked_list_node_t* current = head;
    linked_list_node_t* prev = NULL;
    while (current != NULL) {
        if (current->data == data) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                *destination = current->next;
            }
            free(current->data);
            free(current);
            break; // Assuming data is unique in the linked list
        }
        prev = current;
        current = current->next;
    }
}

typedef struct song_t {
    const char* name;
    float duration;
} song_t;

void create_song(song_t* destination,const char* name, float duration) { 
    destination->name = strdup(name);                   
    destination -> duration = duration;
}


linked_list_node_t** array_of_playlist_ptrs = NULL;

int main(void){
    int playlist_count = 5; // The total number of playlists at first

    // Creating an array of linked lists with an initial size of 5
    create_array_of_linked_list_ptrs(&array_of_playlist_ptrs, playlist_count);

    // Creating 5 playlists 
    for (int i = 0; i < 5; i++) {
        create_link_list(&array_of_playlist_ptrs[i]);
    }
    
    // Resizing the array to a size of 10
    int old_size = 5;
    int new_size = 10;
    resize_array_of_linked_list_ptrs(&array_of_playlist_ptrs, array_of_playlist_ptrs, old_size, new_size);

    // Creating five more playlists 
    for (int i = playlist_count; i < new_size; i++) {
        create_link_list(&(array_of_playlist_ptrs[i]));
        set_element_of_array_of_linked_list_ptrs(array_of_playlist_ptrs, i, array_of_playlist_ptrs[i]);
    }

    // Inserting four songs for each playlist
    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < 4; j++) {
            song_t* song = malloc(sizeof(song_t));                      // allocating memory before creating song structs
            char song_name[25];
            sprintf(song_name, "Playlist %d - Song %d", i + 1, j+1);
            srand((unsigned int)(time(NULL) + i + j)*((rand() % 10)));    // Seeding the random number generator 
            create_song(song, song_name, (float)((rand() % 200) + 60));  // Random duration between 60 and 260 seconds
            add_to_linked_list(&array_of_playlist_ptrs[i], song);
        }  
    }    
    
    // Removing the second song from each playlist
    int i = 0;
    while (i<new_size)
    {
        linked_list_node_t* playlist = NULL;
        get_element_of_array_of_linked_list_ptrs(array_of_playlist_ptrs, i, &playlist);
        if (playlist != NULL && playlist->next != NULL) {
            linked_list_node_t* second_song = playlist->next;
            remove_from_linked_list(&playlist, playlist, second_song->data);
        }
        i++;
    }
    

    // Printing all playlists and their contents
    for (int i = 0; i < new_size; i++) {
        printf("Playlist #%d:\n", i + 1);
        linked_list_node_t* playlist = NULL;
        get_element_of_array_of_linked_list_ptrs(array_of_playlist_ptrs, i, &playlist); //Getting element to print
        linked_list_node_t* current = playlist;                                         // Assigning it to temporary variable
        while (current != NULL) {
            song_t* song = (song_t*)current->data;
            printf("Name: %s, Duration: %.2f seconds\n", song->name, song->duration);
            current = current->next;
        }
    }

    // Destroying each playlist
    for (int i = 0; i < new_size; i++) {
        destroy_linked_list(&array_of_playlist_ptrs[i]);
    }

    // Destroying the array of playlists
    destroy_array_of_linked_list_ptrs(&array_of_playlist_ptrs);
    
    return 0;
}