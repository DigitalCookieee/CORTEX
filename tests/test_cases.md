# CORTEX test checklist
## Build and launch
- [ ] Build succeeds with the exact command in README.
- [ ] Program starts when data/memory.txt does not exist.
- [ ] `help` lists commands.
- [ ] `quit` exits and reports whether saving succeeded.

## Memory
- [ ] Add a normal fact.
- [ ] `list` shows the new fact and its ID.
- [ ] Quit, relaunch, then `list`: fact still exists.
- [ ] Update an existing ID.
- [ ] Delete an existing ID.
- [ ] Delete an ID that does not exist.
- [ ] Try adding an empty topic/content/category.
- [ ] Try a field containing `|`; program should reject it.
- [ ] Add facts until near the 200-fact limit (optional).

## Retrieval
- [ ] Ask using a word in the topic.
- [ ] Ask using a word only in the content.
- [ ] Ask using a stop word only; should not produce a confident answer.
- [ ] Ask an unrelated question; should say it does not know.
- [ ] Ask using CPU/processor or RAM/memory alias examples.
- [ ] Confirm score and matched-word explanation are shown.

## Evidence for demo- [ ] Capture a terminal screenshot of add/list/ask/unknown/quit.- [ ] Explain in your own words what each .c and .h file does.