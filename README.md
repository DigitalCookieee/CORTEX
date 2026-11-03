# CORTEX — Offline Memory Assistant 
CORTEX is a small offline knowledge-retrieval project written in standard C11.
It stores facts in a text file, searches them using keyword scoring and a tiny
alias map, and shows why a fact matched. It is not a large language model and
does not independently verify whether stored facts are true.

## Requirements
- GCC or Clang with C11 support
- A terminal/command prompt- No network access or third-party libraries required

## Folder layout
- `src/main.c`: program entry point and startup/shutdown
- `src/memory.c`, `src/memory.h`: fact structure, CRUD, file persistence
- `src/nlp.c`, `src/nlp.h`: normalization, tokenization, relevance scoring
- `src/chatbot.c`, `src/chatbot.h`: command loop and user-facing responses
- `data/memory.txt`: saved facts
- `tests/test_cases.md`: manual test checklist

## Build from the project root
Linux/macOS:
    gcc -std=c11 -Wall -Wextra -Wpedantic src/main.c src/memory.c src/nlp.c src/chatbot.c -o cortex
Run:
    ./cortex
Windows (MinGW GCC):
    gcc -std=c11 -Wall -Wextra -Wpedantic src/main.c src/memory.c src/nlp.c src/chatbot.c -o cortex.
exe
Run:
    .\\cortex.exe

## Commands
- `help`
- `add` (then enter topic, category, and fact/content)
- `list`- `ask your question`
- `update ID new content`
- `delete ID`
- `quit`

## Important limitations
- The first version supports up to 200 facts.
- Fields are limited by fixed-size arrays.
- The text-file format uses `|` as a separator, so do not put `|` or newlines inside a fact field.
- Keyword scores are heuristic rankings, not probabilities.
- The synonym/alias list is deliberately tiny and must be expanded and tested carefully