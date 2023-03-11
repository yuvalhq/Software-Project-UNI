#ifndef COMMON_H
#define COMMON_H

#define FATAL_ERROR() {\
    printf("An Error Has Occurred\n");\
    exit(EXIT_FAILURE);\
}

#define FATAL_ERROR_IF_NULL(ptr) if (ptr == NULL) { FATAL_ERROR(); }

#define print_float_with_precision_4(number) if (number > -10e-4 || number < 10e-4) { printf("%.4f", number); } else { printf("0.0000"); }

#endif
