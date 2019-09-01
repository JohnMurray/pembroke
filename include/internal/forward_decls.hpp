#pragma once

/* 
 * A single header to import if an event2 C-struct or function is needed in a publicly
 * visible header. This forward declares whatever is needed.
 */

extern "C" {

    // ---
    // Structs
    // ---
    struct event_base;
    struct event;

    // ---
    // Functions
    // ---
    void event_base_free(event_base *);
    void event_free(event *);

}
