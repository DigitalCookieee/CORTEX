#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chatbot.h"
#include "nlp.h"
#define INPUT_SIZE 512
#define MATCH_THRESHOLD 2


static int read_line(const char *prompt, char *buffer, size_t size)
{
    int ch;
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) return 0;
    if (strchr(buffer, '\n') == NULL) {
        while ((ch = getchar()) != '\n' && ch != EOF) { }
    }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    return 1;
}


static int parse_positive_int(const char *text, int *value)
{
    char *end;
    long n = strtol(text, &end, 10);
    while (*end == ' ') end++;
    if (text == end || *end != '\0' || n <= 0 || n > 2147483647L)
        return 0;
    *value = (int)n;
    return 1;
}


static void add_fact(Memory *memory)
{
    char topic[MAX_TEXT], content[MAX_TEXT], category[MAX_CATEGORY];
    int id;
    if (!read_line("Topic: ", topic, sizeof topic) ||
        !read_line("Category: ", category, sizeof category) ||
        !read_line("Fact/content: ", content, sizeof content)) return;
    id = memory_add(memory, topic, content, category);
    if (id < 0) {
        puts("Could not add fact. Check that fields are non-empty, fit the limits,");
        puts("contain no '|' character, and memory has fewer than 200 facts.");
        return;
    }
    puts("Fact added.");
}


static void delete_fact(Memory *memory, const char *argument)
{
    int id;
    if (!parse_positive_int(argument, &id)) {
        puts("Usage: delete ID   (example: delete 3)");
    } else if (memory_delete(memory, id)) {
        puts("Fact deleted.");
    } else {
        puts("No fact has that ID.");
    }
}


static void update_fact(Memory *memory, const char *argument)
{
    char id_text[32], content[MAX_TEXT];
    const char *space = strchr(argument, ' ');
    int id;
    if (!space) {
        puts("Usage: update ID new content");
        return;
    }
    if ((size_t)(space - argument) >= sizeof id_text) {
 puts("Invalid ID.");
        return;
    }
    memcpy(id_text, argument, (size_t)(space - argument));
    id_text[space - argument] = '\0';
    if (!parse_positive_int(id_text, &id)) {
        puts("Invalid ID. Usage: update ID new content");
        return;
    }
    while (*space == ' ') space++;
    if (!*space) {
        puts("Provide the new content after the ID.");
        return;
    }
    if (memory_update(memory, id, space))
        puts("Fact updated.");
    else
        puts("Update failed: ID not found, empty content, or unsupported '|'.");
}


static void answer_question(const Memory *memory, const char *query)
{
    int score = 0;
    char matches[MAX_TEXT] = "";
    const Fact *fact = find_best_match(memory, query, &score,
                                       matches, sizeof matches);
    if (!fact || score < MATCH_THRESHOLD) {
        puts("I don't know that yet. Try adding a relevant fact with 'add'.");
        return;
    }
    printf("CORTEX: %s\n", fact->content);
    printf("Source: [%d] %s (category: %s)\n",
           fact->id, fact->topic, fact->category);
    printf("Match score: %d; matched words: %s\n",
           score, matches[0] ? matches : "(none)");
    puts("Note: this is a simple keyword score, not a probability of truth.");
}


static void show_help(void)
{
    puts("\nCommands:");
    puts("  help                  Show commands");
    puts("  add                   Add a fact (you will be prompted)");
    puts("  list                  List saved facts");
    puts("  ask QUESTION          Search memory for a question");
    puts("  delete ID             Delete a fact by ID");
    puts("  update ID CONTENT     Replace a fact's content");
    puts("  quit                  Save and exit");
    puts("\nExample: ask what does a CPU do");
}


void chatbot_run(Memory *memory)
{
    char input[INPUT_SIZE];
    puts("CORTEX - Offline Memory Assistant");
    puts("Type 'help' to see commands. Type 'quit' to exit.");
    for (;;) {
        char *argument;
        if (!read_line("\n> ", input, sizeof input)) break;
        if (input[0] == '\0') continue;
        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            break;
        } else if (strcmp(input, "help") == 0) {
            show_help();
        } else if (strcmp(input, "add") == 0) {
            add_fact(memory);
        } else if (strcmp(input, "list") == 0) {
            memory_list(memory);
        } else if (strncmp(input, "ask ", 4) == 0) {
            answer_question(memory, input + 4);
        } else if (strncmp(input, "delete ", 7) == 0) {
            delete_fact(memory, input + 7);
        } else if (strncmp(input, "update ", 7) == 0) {
            update_fact(memory, input + 7);
        } else {
            puts("Unknown command. Type 'help'.");
        }
    }
}