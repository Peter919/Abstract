#include <stdlib.h>
#include "log.h"

const enum e_LogLevel g_min_log_level = LOG_DEBUG;

// primary can be used anywhere in identifier names, secondary not in the beginning
char * g_primary_alphabet = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char * g_secondary_alphabet = "0123456789";

const char g_stack_element_t_format[] = "%lf";
const size_t g_max_stack_size = 100000;

/*

REGIONS:
* 512xINFx512 (can probably be changed)
* stores events and their seeds and data (x starting events when the region is loaded the first time, erosion (strength, god that did it etc),
  fire (origin etc) raid (strength, creature type that raids etc) etc)
* stores plans for future events and the gods that oversee those events
* stores a list of chunks that are changed while the player is there
* loads the chunks when needed (uses a seed and adds the events)
* stores general weather (rain, temperature, wind etc), biome (affects event- and chunk generation and might be updated if the region drastically changes)
* maps may exist over multiple regions, in which case some random regions will be loaded
* the events of nearby regions will be loaded so newspaper can write about them

CHUNKS:
* 16x4x16 (can probably be changed)
* a specific amount in width/depth and height is simulated at a time
* stores parent region and adds events to it
* adds itself to the list of changed chunks of the parent region when the player directly or indirectly changes the chunk
* stores pointer to neighbour chunks (could be from other region)

*/
