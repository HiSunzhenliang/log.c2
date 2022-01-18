#include "demo.h"

#define DEMO_MAIN_ITEM(name) \
    { #name, demo_##name##_main }

typedef struct demo {
    char *name;
    int (*main)(void);
} demo;

static demo g_demo[] = {
    DEMO_MAIN_ITEM(stdout),
    DEMO_MAIN_ITEM(file),
};

int main(int argc, char *argv[]) {
    char *target_demo = argc == 2 ? argv[1] : "";
    int r = 0;
    char *name = NULL;
    size_t size = sizeof(g_demo) / sizeof(g_demo[0]);
    for (size_t i = 0; i < size; i++) {
        if (g_demo[i].name && !strcmp(g_demo[i].name, target_demo)) {
            name = g_demo[i].name;
            r = g_demo[i].main();
            if (r) {
                printf("%s demo failed.\n", g_demo[i].name);
            }
            break;
        }
    }

    if (!name) {
        printf("===========================================================\n"
                 "  Usages: demo [demoname]\n");

        for (int i = 0; i < size; i++) printf("Demoname: %s\n", g_demo[i].name);
    }

    return r;
}
