#include "kernel/processor.h"
class Context {
public:
    register64_t gp;
    static Context* get_instance();
    Context();

};
