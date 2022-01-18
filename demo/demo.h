#ifndef DEMO_H
#define DEMO_H

#include <stdlib.h>
#include "../log.h"

#define DEMO_MAIN_DECL(name) int demo_##name##_main(void)

DEMO_MAIN_DECL(stdout);
DEMO_MAIN_DECL(file);

#endif /* DEMO_H */