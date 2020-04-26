#ifndef _SURFACE_LOAD_H
#define _SURFACE_LOAD_H

#include "types.h"

struct SurfaceNode
{
    struct SurfaceNode *next;
    struct Surface *surface;
};

enum
{
    SPATIAL_PARTITION_FLOORS,
    SPATIAL_PARTITION_CEILS,
    SPATIAL_PARTITION_WALLS
};

typedef struct SurfaceNode SpatialPartitionCell[3];

#define CELL_COUNT 4

extern SpatialPartitionCell gStaticSurfacePartition[CELL_COUNT][CELL_COUNT];
extern SpatialPartitionCell gDynamicSurfacePartition[CELL_COUNT][CELL_COUNT];
extern struct SurfaceNode *sSurfaceNodePool;
extern struct Surface *sSurfacePool;
extern s16 sSurfacePoolSize;

void alloc_surface_pools(void);
void load_area_terrain(s16 index, s16 *data, s8 *surfaceRooms, s16 *macroObjects);
void clear_dynamic_surfaces(void);
void load_object_collision_model(void);

#endif
