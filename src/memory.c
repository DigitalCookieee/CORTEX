#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memory.h"

static void copy_text(char *destination, size_t size, const char *source) //safely copies text to prevent buffer overflow
{
    if (size == 0) return;
    snprintf(destination, size, "%s", source ? source : " ");
}



void memory_init(Memory *memory) //sets the initial count to 0 and the id next to the first one to 1
{
    memory->count = 0;
    memory->next_id = 1;
}



int memory_load(Memory *memory, const char *filename) //loading a file
{
    FILE *file = fopen(filename, "r");
    char line[4 * MAX_TEXT + 128];
    memory_init(memory);
    if (file == NULL) {
        // checking if the file is empty
        return 0;
    }

    
    while (fgets(line, sizeof line, file) != NULL &&
           memory->count < MAX_FACTS) {
        char *id_text, *category, *topic, *content, *end;
        long parsed_id;
        line[strcspn(line, "\r\n")] = '\0'; //clears out any newline characters in the string
        id_text = strtok(line, "|"); //adding delimiters after every line, to seperate id, category, topic, content
        category = strtok(NULL, "|");
        topic = strtok(NULL, "|");
        content = strtok(NULL, "");
        if (!id_text || !category || !topic || !content) continue;
            parsed_id = strtol(id_text, &end, 10);
        if (*end != '\0' || parsed_id <= 0 || parsed_id > 2147483647L)
            continue;
        Fact *fact = &memory->facts[memory->count]; //storing the fact into the array memory
        fact->id = (int)parsed_id; //assigning the converted integer id of the text/string
        copy_text(fact->category, sizeof fact->category, category); //safely copying using the helper function
        copy_text(fact->topic, sizeof fact->topic, topic);
        copy_text(fact->content, sizeof fact->content, content);
        memory->count++;
        if (fact->id >= memory->next_id) memory->next_id = fact->id + 1; //changing the count
    }
    fclose(file);
    return 1;
}



int memory_save(const Memory *memory, const char *filename)
{
    FILE *file = fopen(filename, "w"); //opening the file in write mode, overwriting the file with new data
    int i;
    if (file == NULL) {
        perror("Could not save memory file");
        return 0;
    }
    for (i = 0; i < memory->count; i++) {
        const Fact *fact = &memory->facts[i];
        /* Do not put the separator or a newline inside a field. */
        if (fprintf(file, "%d|%s|%s|%s\n", fact->id, fact->category, //printing every saved fact, seperated by delimiters
                    fact->topic, fact->content) < 0) {
            fclose(file);
            return 0;
        }
    }
    if (fclose(file) != 0) return 0;
    return 1;
}


int memory_add(Memory *memory, const char *topic,
 const char *content, const char *category)
{
 Fact *fact;
 if (memory->count >= MAX_FACTS) return -1; //checking the memory is not empty
 if (!topic || !content || !category || 
 topic[0] == '\0' || content[0] == '\0' || category[0] == '\0')
 return -1;
 if (strchr(topic, '|') || strchr(content, '|') || //checking if they contain any new line characters or delimiters
 strchr(category, '|') || strchr(topic, '\n') ||
 strchr(content, '\n') || strchr(category, '\n'))
 return -1; //program fails if that is the case
 
 fact = &memory->facts[memory->count];
 fact->id = memory->next_id++; //giving the new id to the next fact
 copy_text(fact->topic, sizeof fact->topic, topic); //safely copying the topics, content and category using the safe helper function
 copy_text(fact->content, sizeof fact->content, content);
 copy_text(fact->category, sizeof fact->category, category);
 memory->count++; //increasing the count of facts
 return fact->id; //returning the new id
}

const Fact *memory_get_by_id(const Memory *memory, int id)
{
 int i;
 for (i = 0; i < memory->count; i++)
 if (memory->facts[i].id == id) return &memory->facts[i];// Matches the id if same then passes the facts
 return NULL;
}

int memory_delete(Memory *memory, int id)
{
 int i, j;
 for (i = 0; i < memory->count; i++) {
 if (memory->facts[i].id == id) {  // Checks the id if matched  
 for (j = i; j < memory->count - 1; j++)
 memory->facts[j] = memory->facts[j + 1];// Shifting the remaining facts to the left (where the deleted facts left there position and created an empty space)
 memory->count--; // Decrements the count after deleting the fact
 return 1;
 }
 }
  return 0;
}

int memory_update(Memory *memory, int id, const char *content)
{
 int i;

  if (!content || content[0] == '\0' || strchr(content, '|') || strchr(content, '\n'))
   return 0; // Checking if the field is completely empty/NULL or if it contains any delimters and null characters

 for (i = 0; i < memory->count; i++) {  
 if (memory->facts[i].id == id) { // Matches the user entered id in the file 
 copy_text(memory->facts[i].content,
 sizeof memory->facts[i].content, content);// Safely overwrites the existing content buffer with the new content.
 return 1;
 }
 }
 return 0;
}

void memory_list(const Memory *memory)
{
 int i;
 if (memory->count == 0) { // Checks whether the memory is empty or not
 puts("Memory is empty. Use 'add' to store a fact.");
 return;
 }
 for (i = 0; i < memory->count; i++) { // Loops through all stored facts 
 const Fact *fact = &memory->facts[i]; // Obtaining read only pointer for fact at the index [i]
 printf("[%d] (%s) %s: %s\n", fact->id, fact->category, fact->topic, fact->content); // Formats and display the facts in the proper format
 }
}






























































