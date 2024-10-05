#include <kernel/context.h>

static Context* instance;

Context::Context() {
    instance = this;
}

Context* Context::get_instance() {
    return instance;
}
