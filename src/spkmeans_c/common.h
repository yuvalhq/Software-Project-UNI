#ifndef COMMON_H
#define COMMON_H

#define FATAL_ERROR() {\
    printf("An Error Has Occurred\n");\
    exit(EXIT_FAILURE);\
}

#define FATAL_ERROR_IF_NULL(ptr) if (ptr == NULL) { FATAL_ERROR(); }

#endif
