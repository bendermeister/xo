#include "xo.h"
#include <stdio.h>
#include <ucontext.h>

static ucontext_t yield_ctx = {0};
static char yield_stack[4096] = {0}; 

static ucontext_t main_ctx = {0};

void worker(void) {
}

void xo_yield(void) {
}

int main(void) {
    int status;
    status = getcontext(&yield_ctx);
    if (status == -1) {
        fprintf(stderr, "PANIC: could not init xo\n");
        return -1;
    }
    yield_ctx.uc_stack.ss_sp = yield_stack;
    yield_ctx.uc_stack.ss_size = sizeof(yield_stack);
    yield_ctx.uc_link = &main_ctx;
    makecontext(&yield_ctx, worker, 0);

    return 0;
}
