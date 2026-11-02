#ifndef MEMORY_H //prevents compilation
#define MEMORY_H // defining the header file


#define MAX_FACTS 200
#define MAX_TEXT 256
#define MAX_CATEGORY 32

//defining structures
typedef struct  {
    int id;
    char topic [MAX_TEXT];
    char content [MAX_TEXT];
    char category [MAX_CATEGORY];
} Fact;

typedef struct {
    Fact facts[MAX_FACTS];
    int count;
    int next_id;
} Memory;

//declaring functions
void memory_init(Memory *memory);
int memory_load(Memory *memory, const char *filename);
int memory_save(const Memory *memory, const char *filename);
int memory_add(Memory *memory, const char *topic, const char *content, const char *category);
int memory_delete(Memory *memory, int id);
int memory_update(Memory *memory, int id, const char *content);
const Fact *memory_get_by_id(const Memory *memory, int id);
void memory_list(const Memory *memory);

#endif