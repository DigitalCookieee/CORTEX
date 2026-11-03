#include <stdio.h>
#include "memory.h" //including memory header file
#include "chatbot.h" //including chatbot startup header file

int main (void)
{
    Memory memory;
    memory_init(&memory);
    if(!memory_load(&memory, "data/memory.txt")) //incase there's no memory file
    puts("No saved memory file yet; starting with an empty memory.");

    chatbot_run(&memory);

    if (memory_save(&memory, "data/memory.txt"))
        puts("Memory saved. Goodbye!");
        else
            puts("Warning: memory could not be saved. Check the data/ folder.");
        return 0;
}
