#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "nlp.h"


void normalize_text(char *text)
{
    size_t i;
    for (i = 0; text[i] != '\0'; i++)
        text[i] = (char)tolower((unsigned char)text[i]);
}


int tokenize(const char *text, char tokens[][MAX_WORD_LEN], int max_tokens)
{
    int count = 0;
    size_t i = 0;
    while (text[i] != '\0' && count < max_tokens) {
        size_t j = 0;
        while (text[i] != '\0' && !isalnum((unsigned char)text[i])) i++;
        if (text[i] == '\0') break;
        while (text[i] != '\0' && isalnum((unsigned char)text[i])) {
            if (j < MAX_WORD_LEN - 1)
                tokens[count][j++] = (char)tolower((unsigned char)text[i]);
            i++;
        }
        tokens[count][j] = '\0';
        if (j > 0) count++;
    }
    return count;
}


static int is_stop_word(const char *word)
{
    const char *stop_words[] = {
        "a", "an", "the", "is", "are", "what", "who", "how",
        "do", "does", "did", "of", "in", "on", "to", "for",
        "and", "or", "with", "me", "tell", "about"
    };
    size_t i;
    for (i = 0; i < sizeof stop_words / sizeof stop_words[0]; i++)
        if (strcmp(word, stop_words[i]) == 0) return 1;
    return 0;
}


static int related_words(const char *a, const char *b)
{
    /* Tiny illustrative alias map. Add pairs carefully as you test. */
    if ((strcmp(a, "cpu") == 0 || strcmp(a, "processor") == 0) &&
        (strcmp(b, "cpu") == 0 || strcmp(b, "processor") == 0)) return 1;
    if ((strcmp(a, "ram") == 0 || strcmp(a, "memory") == 0) &&
        (strcmp(b, "ram") == 0 || strcmp(b, "memory") == 0)) return 1;
    if ((strcmp(a, "ai") == 0 || strcmp(a, "artificial") == 0 ||
         strcmp(a, "intelligence") == 0) &&
        (strcmp(b, "ai") == 0 || strcmp(b, "artificial") == 0 ||
         strcmp(b, "intelligence") == 0)) return 1;
    return strcmp(a, b) == 0;
}


static int text_contains_related(const char *word, const char *text)
{
    char tokens[MAX_WORDS][MAX_WORD_LEN];
    int n = tokenize(text, tokens, MAX_WORDS);

int i;
    for (i = 0; i < n; i++)
        if (related_words(word, tokens[i])) return 1;
    return 0;
}
int calculate_score(const char *query, const Fact *fact,
                    char *matched_words, size_t matched_size)
{
    char query_tokens[MAX_WORDS][MAX_WORD_LEN];
    int n, i, score = 0;
    size_t used = 0;
    if (matched_size > 0) matched_words[0] = '\0';
    n = tokenize(query, query_tokens, MAX_WORDS);
    for (i = 0; i < n; i++) {
        const char *word = query_tokens[i];
        int weight = 0;
        char lower_topic[MAX_TEXT], lower_content[MAX_TEXT];
        char lower_category[MAX_CATEGORY];
        if (is_stop_word(word)) continue;
        snprintf(lower_topic, sizeof lower_topic, "%s", fact->topic);
        snprintf(lower_content, sizeof lower_content, "%s", fact->content);
        snprintf(lower_category, sizeof lower_category, "%s", fact->category);
        normalize_text(lower_topic);
        normalize_text(lower_content);
        normalize_text(lower_category);
        if (text_contains_related(word, lower_topic)) weight = 3;
        else if (text_contains_related(word, lower_content)) weight = 2;
        else if (text_contains_related(word, lower_category)) weight = 1;
        if (weight > 0) {
            int already_added = 0;
            int k;
            score += weight;
            /* Avoid repeating a word in the explanation. */
            for (k = 0; k < i; k++)
                if (strcmp(query_tokens[k], word) == 0) already_added = 1;
            if (!already_added && matched_size > used + 1) {
                int written = snprintf(matched_words + used,
                                       matched_size - used, "%s%s",
                                       used ? ", " : "", word);
                if (written > 0) used += (size_t)written;
                if (used >= matched_size) used = matched_size - 1;
            }
        }
    }
    return score;
}


const Fact *find_best_match(const Memory *memory, const char *query,
                            int *best_score, char *matched_words,
                            size_t matched_size)
{
    const Fact *best = NULL;
    int best_value = 0;
    int i;
    if (matched_size > 0) matched_words[0] = '\0';
    for (i = 0; i < memory->count; i++) {
        char current_matches[MAX_TEXT] = "";
        int score = calculate_score(query, &memory->facts[i],
                                    current_matches, sizeof current_matches);
        if (score > best_value) {
            best_value = score;
            best = &memory->facts[i];
            if (matched_size > 0)
                snprintf(matched_words, matched_size, "%s", current_matches);
        }
    }
    
    if (best_score) *best_score = best_value;
    return best;
}    