#include <stddef.h>

#define DEFAULT_ITERATIONS_COUNT 300
#define DEFAULT_EPSILON 0.001

typedef double *Vector;
typedef double Distance;

typedef struct Cluster {
    Vector centroid;
} Cluster;

Distance euclidean_distance(Vector p, Vector q, size_t size);
void init_clusters(Cluster **clusters, Vector *vectors, size_t vector_size, size_t k);
void fit(Cluster **clusters, size_t vectors_count, Vector *vectors, size_t vector_size, size_t k, size_t iter, double epsilon);