#include "demo.h"

#define DEMO_MAIN_ITEM(name) \
    { #name, demo_##name##_main }

typedef struct demo {
    char *name;
    int (*main)(void);
} demo;

static demo g_demo[] = {
    DEMO_MAIN_ITEM(stdout),
};

int main(int argc, char *argv[]) {
    log_stdout_verbose(LOG_INFO);
    int r = 0;
    char *name = NULL;
    size_t size = sizeof(g_demo) / sizeof(g_demo[0]);
    for (size_t i = 0; i < size; i++) {
        if (g_demo[i].name && !strcmp(g_demo[i].name, argv[1])) {
            name = g_demo[i].name;
            r = g_demo[i].main();
            if (r) {
                log_error("%s demo failed.", g_demo[i].name);
            } else {
                log_info("%s demo done.", g_demo[i].name);
            }
            break;
        }
    }

    if (!name) {
        log_info("===========================================================");
        log_info("Usages: demo [demoname]");

        for (int i = 0; i < size; i++) log_info("demoname: %s", g_demo[i].name);
    }

    return r;
}
