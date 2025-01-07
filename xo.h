#ifndef XO_H
#define XO_H

typedef struct xo_routine_t xo_routine_t;
struct xo_routine_t {
    void (*run)(void *ctx);
    void *ctx;
};

/// Yields execution of current routine
void xo_yield(void);

/// Pushes routine into execution queue
/// # Return Value
/// returns `-1` on error and `0` on success
int xo_routine_push(xo_routine_t routine);

/// Initilize libxo
/// # Return Value
/// returns `-1` on error and `0` on success
int xo_init(void);

#endif // XO_H
