// player.c
// Simple simulation of a music player / gallery viewer using a doubly linked list.
// Compile: gcc player.c -o player
// On Windows: compile with a gcc that supports windows.h (e.g. MinGW)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #include <windows.h>
  #define PAUSE(ms) Sleep(ms)
#else
  #include <unistd.h>
  #define PAUSE(ms) sleep(ms/1000)
#endif

#define MAX_TITLE 128
#define MAX_ARTIST 128

typedef enum { TYPE_SONG, TYPE_IMAGE } ItemType;

typedef struct Node {
    ItemType type;
    char title[MAX_TITLE];
    char meta[MAX_ARTIST]; // artist for song, caption or details for image
    struct Node *prev, *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    Node *current;
    int size;
    int loop; // if true, navigation wraps around
} Player;

// Utility: create node
Node* create_node(ItemType t, const char *title, const char *meta) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) { perror("malloc"); exit(EXIT_FAILURE); }
    n->type = t;
    strncpy(n->title, title ? title : "", MAX_TITLE-1);
    n->title[MAX_TITLE-1] = '\0';
    strncpy(n->meta, meta ? meta : "", MAX_ARTIST-1);
    n->meta[MAX_ARTIST-1] = '\0';
    n->prev = n->next = NULL;
    return n;
}

// Initialize player
void init_player(Player *p) {
    p->head = p->tail = p->current = NULL;
    p->size = 0;
    p->loop = 0;
}

// Add to end
void add_item(Player *p, ItemType t, const char *title, const char *meta) {
    Node *n = create_node(t, title, meta);
    if (!p->head) {
        p->head = p->tail = p->current = n;
    } else {
        p->tail->next = n;
        n->prev = p->tail;
        p->tail = n;
    }
    p->size++;
}

// Insert after current
void insert_after_current(Player *p, ItemType t, const char *title, const char *meta) {
    if (!p->current) { add_item(p, t, title, meta); return; }
    Node *n = create_node(t, title, meta);
    Node *cur = p->current;
    n->next = cur->next;
    n->prev = cur;
    cur->next = n;
    if (n->next) n->next->prev = n;
    else p->tail = n;
    p->size++;
}

// Remove current node and move current to next (or prev if next NULL)
void remove_current(Player *p) {
    if (!p->current) {
        printf("No current item to remove.\n");
        return;
    }
    Node *rem = p->current;
    if (rem->prev) rem->prev->next = rem->next;
    else p->head = rem->next;

    if (rem->next) rem->next->prev = rem->prev;
    else p->tail = rem->prev;

    // choose new current
    if (rem->next) p->current = rem->next;
    else if (rem->prev) p->current = rem->prev;
    else p->current = NULL;

    free(rem);
    p->size--;
    printf("Item removed. New size: %d\n", p->size);
}

// Move to next item
void next_item(Player *p) {
    if (!p->current) { printf("List empty.\n"); return; }
    if (p->current->next) p->current = p->current->next;
    else if (p->loop && p->head) p->current = p->head;
    else printf("Already at last item.\n");
}

// Move to previous item
void prev_item(Player *p) {
    if (!p->current) { printf("List empty.\n"); return; }
    if (p->current->prev) p->current = p->current->prev;
    else if (p->loop && p->tail) p->current = p->tail;
    else printf("Already at first item.\n");
}

void go_first(Player *p) {
    if (!p->head) printf("List empty.\n");
    else p->current = p->head;
}

void go_last(Player *p) {
    if (!p->tail) printf("List empty.\n");
    else p->current = p->tail;
}

void print_item(const Node *n, int show_index, int index) {
    if (!n) return;
    if (show_index) printf("[%d] ", index);
    if (n->type == TYPE_SONG) {
        printf("Song: \"%s\" - Artist: %s\n", n->title, n->meta);
    } else {
        printf("Image: \"%s\" - Info: %s\n", n->title, n->meta);
    }
}

// Print full list
void show_list(Player *p) {
    if (!p->head) { printf("List empty.\n"); return; }
    Node *it = p->head;
    int idx = 1;
    printf("Playlist (%d items):\n", p->size);
    while (it) {
        if (it == p->current) printf(" -> ");
        else printf("    ");
        print_item(it, 1, idx);
        it = it->next; idx++;
    }
}

// Show current
void show_current(Player *p) {
    if (!p->current) { printf("No current item (empty list).\n"); return; }
    printf("CURRENT -> ");
    print_item(p->current, 0, 0);
}

// Free all nodes
void free_all(Player *p) {
    Node *it = p->head;
    while (it) {
        Node *n = it;
        it = it->next;
        free(n);
    }
    p->head = p->tail = p->current = NULL;
    p->size = 0;
}

// Auto-play / auto-view simulation
void autoplay(Player *p, int seconds, int steps) {
    if (!p->current) { printf("Nothing to autoplay.\n"); return; }
    printf("Starting autoplay: %d steps, %d second(s) between items. Looping is %s.\n",
           steps, seconds, p->loop ? "ON" : "OFF");
    for (int i = 0; i < steps; ++i) {
        show_current(p);
        if (i < steps-1) {
            PAUSE(seconds * 1000); // ms
            if (p->current->next) p->current = p->current->next;
            else if (p->loop && p->head) p->current = p->head;
            else { printf("Reached end. Stopping autoplay.\n"); break; }
        }
    }
}

// Sample data to populate the list
void sample_data(Player *p) {
    add_item(p, TYPE_SONG, "Here Comes the Sun", "The Beatles");
    add_item(p, TYPE_SONG, "Imagine", "John Lennon");
    add_item(p, TYPE_IMAGE, "Sunset.jpg", "Beach sunset, 1920x1080");
    add_item(p, TYPE_SONG, "Bohemian Rhapsody", "Queen");
    add_item(p, TYPE_IMAGE, "Portrait.png", "Studio portrait");
}

// Simple input helpers
void read_line(char *buf, int n) {
    if (!fgets(buf, n, stdin)) { buf[0] = '\0'; return; }
    size_t l = strlen(buf); if (l && buf[l-1] == '\n') buf[l-1] = '\0';
}

int menu() {
    printf("\n--- Player Menu ---\n");
    printf("1. Show playlist\n");
    printf("2. Show current\n");
    printf("3. Next\n");
    printf("4. Previous\n");
    printf("5. Go to first\n");
    printf("6. Go to last\n");
    printf("7. Add item (end)\n");
    printf("8. Insert after current\n");
    printf("9. Remove current\n");
    printf("10. Toggle looping\n");
    printf("11. Autoplay / Auto-view\n");
    printf("12. Populate sample data (clears existing)\n");
    printf("0. Exit\n");
    printf("Choose: ");
    char line[16]; read_line(line, sizeof(line));
    return atoi(line);
}

int main(void) {
    Player p;
    init_player(&p);

    printf("Doubly Linked List Player/Viewer Simulation\n");
    printf("Type: Music songs and Images supported.\n");

    // start with sample items
    sample_data(&p);

    while (1) {
        int choice = menu();
        if (choice == 0) break;

        if (choice == 1) {
            show_list(&p);
        } else if (choice == 2) {
            show_current(&p);
        } else if (choice == 3) {
            next_item(&p);
            show_current(&p);
        } else if (choice == 4) {
            prev_item(&p);
            show_current(&p);
        } else if (choice == 5) {
            go_first(&p);
            show_current(&p);
        } else if (choice == 6) {
            go_last(&p);
            show_current(&p);
        } else if (choice == 7 || choice == 8) {
            char type_s[8];
            char title[MAX_TITLE];
            char meta[MAX_ARTIST];
            printf("Enter type (song/image): ");
            read_line(type_s, sizeof(type_s));
            printf("Enter title/filename: ");
            read_line(title, sizeof(title));
            printf("Enter artist/caption: ");
            read_line(meta, sizeof(meta));
            ItemType t = (strcasecmp(type_s, "image") == 0) ? TYPE_IMAGE : TYPE_SONG;
            if (choice == 7) add_item(&p, t, title, meta);
            else insert_after_current(&p, t, title, meta);
            printf("Added.\n");
        } else if (choice == 9) {
            remove_current(&p);
        } else if (choice == 10) {
            p.loop = !p.loop;
            printf("Looping is now %s.\n", p.loop ? "ON" : "OFF");
        } else if (choice == 11) {
            char buf[32];
            int secs = 2, steps = p.size;
            printf("Seconds between items (default 2): ");
            read_line(buf, sizeof(buf));
            if (strlen(buf)) secs = atoi(buf) > 0 ? atoi(buf) : 2;
            printf("Number of steps (default playlist size %d): ", p.size);
            read_line(buf, sizeof(buf));
            if (strlen(buf)) steps = atoi(buf) > 0 ? atoi(buf) : p.size;
            autoplay(&p, secs, steps);
        } else if (choice == 12) {
            free_all(&p);
            sample_data(&p);
            printf("Sample data loaded.\n");
        } else {
            printf("Unknown option.\n");
        }
    }

    free_all(&p);
    printf("Goodbye.\n");
    return 0;
}
