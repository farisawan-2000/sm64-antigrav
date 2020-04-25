#include <ultra64.h>

#include "sm64.h"
#include "game/level_update.h"
#include "game/debug.h"
#include "game/print.h"
#include "game/camera.h"
#include "game/mario.h"
#include "game/mario_step.h"
#include "game/level_update.h"
#include "behavior_script.h"
#include "surface_collision.h"
#include "surface_load.h"
#include "game/object_list_processor.h"

static struct Surface *find_Xfloor_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight) {
    register struct Surface *surf;
    struct MarioState *m = gMarioState;
    register s32 y1, z1, y2, z2, y3, z3;
    struct Surface *ceil = NULL;
    f32 dist = 99999.0f;
    ceil = NULL;

    // Stay in this loop until out of ceilings.
    while (surfaceNode != NULL) {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        y1 = surf->vertex1[1];
        z1 = surf->vertex1[2];
        y2 = surf->vertex2[1];
        z2 = surf->vertex2[2];
        y3 = surf->vertex3[1];
        z3 = surf->vertex3[2];

        // Checking if point is in bounds of the triangle laterally.
        if ((y1 - y) * (z2 - z1) - (z1 - z) * (y2 - y1) > 0) {
            continue;
        }

        // Slight optimization by checking these later.
        if ((y2 - y) * (z3 - z2) - (z2 - z) * (y3 - y2) > 0) {
            continue;
        }
        if ((y3 - y) * (z1 - z3) - (z3 - z) * (y1 - y3) > 0) {
            continue;
        }

        // Determine if checking for the camera or not.
        if (gCheckingSurfaceCollisionsForCamera != 0) {
            if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) {
                continue;
            }
        }
        // Ignore camera only surfaces.
        else if (surf->type == SURFACE_CAMERA_BOUNDARY) {
            continue;
        }

        {
            f32 nx = surf->normal.x;
            f32 ny = surf->normal.y;
            f32 nz = surf->normal.z;
            f32 oo = surf->originOffset;
            f32 height;

            if (ny != 0.0f) {
                continue;
            }

            // Find the ceil height at the specific point.
            height = -(y * ny + nz * z + oo) / nx;

            // Checks for ceiling interaction with a 78 unit buffer.
            //! (Exposed Ceilings) Because any point above a ceiling counts
            //  as interacting with a ceiling, ceilings far below can cause
            // "invisible walls" that are really just exposed ceilings.
            if (x - (height - -78.0f) > 0.0f) {
                continue;
            }
            if (x > 0) {
                if (height - m->pos[0] < dist) {
                    dist = height - m->pos[0];
                    *pheight = height;
                    ceil = surf;
                }
            } else {
                if (x - height < dist) {
                    dist = x - height;
                    *pheight = height;
                    ceil = surf;
                }
            }
            // break;
        }
    }

    //! (Surface Cucking) Since only the first ceil is returned and not the lowest,
    //  lower ceilings can be "cucked" by higher ceilings.
    return ceil;
}

static struct Surface *find_Xceil_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight) {
    register struct Surface *surf;
    struct MarioState *m = gMarioState;
    register s32 y1, z1, y2, z2, y3, z3;
    struct Surface *ceil = NULL;
    f32 dist = 99999.0f;
    ceil = NULL;

    // Stay in this loop until out of ceilings.
    while (surfaceNode != NULL) {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        y1 = surf->vertex1[1];
        z1 = surf->vertex1[2];
        y2 = surf->vertex2[1];
        z2 = surf->vertex2[2];
        y3 = surf->vertex3[1];
        z3 = surf->vertex3[2];

        // Checking if point is in bounds of the triangle laterally.
        if ((y1 - y) * (z2 - z1) - (z1 - z) * (y2 - y1) < 0) {
            continue;
        }

        // Slight optimization by checking these later.
        if ((y2 - y) * (z3 - z2) - (z2 - z) * (y3 - y2) < 0) {
            continue;
        }
        if ((y3 - y) * (z1 - z3) - (z3 - z) * (y1 - y3) < 0) {
            continue;
        }

        // Determine if checking for the camera or not.
        if (gCheckingSurfaceCollisionsForCamera != 0) {
            if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) {
                continue;
            }
        }
        // Ignore camera only surfaces.
        else if (surf->type == SURFACE_CAMERA_BOUNDARY) {
            continue;
        }

        {
            f32 nx = surf->normal.x;
            f32 ny = surf->normal.y;
            f32 nz = surf->normal.z;
            f32 oo = surf->originOffset;
            f32 height;

            if (ny != 0.0f) {
                continue;
            }

            // Find the ceil height at the specific point.
            height = -(y * ny + nz * z + oo) / nx;

            // Checks for ceiling interaction with a 78 unit buffer.
            //! (Exposed Ceilings) Because any point above a ceiling counts
            //  as interacting with a ceiling, ceilings far below can cause
            // "invisible walls" that are really just exposed ceilings.
            // if (x - (height - -78.0f) > 0.0f) {
                // continue;
            // }
            if (x - height > 0) {
                dist = x - height;
                *pheight = height;
                ceil = surf;
            }
            break;
        }
    }

    //! (Surface Cucking) Since only the first ceil is returned and not the lowest,
    //  lower ceilings can be "cucked" by higher ceilings.
    return ceil;
}

static struct Surface *find_Zfloor_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight) {
    register struct Surface *surf;
    struct MarioState *m = gMarioState;
    register s32 y1, z1, y2, z2, y3, z3;
    struct Surface *ceil = NULL;
    f32 dist = 99999.0f;
    ceil = NULL;

    // Stay in this loop until out of ceilings.
    while (surfaceNode != NULL) {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        y1 = surf->vertex1[1];
        z1 = surf->vertex1[0];
        y2 = surf->vertex2[1];
        z2 = surf->vertex2[0];
        y3 = surf->vertex3[1];
        z3 = surf->vertex3[0];

        // Checking if point is in bounds of the triangle laterally.
        if ((z1 - z) * (y2 - y1) - (y1 - y) * (z2 - z1) < 0) {
            continue;
        }

        // Slight optimization by checking these later.
        if ((z2 - z) * (y3 - y2) - (y2 - y) * (z3 - z2) < 0) {
            continue;
        }
        if ((z3 - z) * (y1 - y3) - (y3 - y) * (z1 - z3) < 0) {
            continue;
        }

        // Determine if checking for the camera or not.
        if (gCheckingSurfaceCollisionsForCamera != 0) {
            if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) {
                continue;
            }
        }
        // Ignore camera only surfaces.
        else if (surf->type == SURFACE_CAMERA_BOUNDARY) {
            continue;
        }

        {
            f32 nx = surf->normal.x;
            f32 ny = surf->normal.y;
            f32 nz = surf->normal.z;
            f32 oo = surf->originOffset;
            f32 height;

            if (ny != 0.0f) {
                continue;
            }

            // Find the ceil height at the specific point.
            height = -(y * ny + nx * x + oo) / nz;

            // Checks for ceiling interaction with a 78 unit buffer.
            //! (Exposed Ceilings) Because any point above a ceiling counts
            //  as interacting with a ceiling, ceilings far below can cause
            // "invisible walls" that are really just exposed ceilings.
            // if (z - (height - -78.0f) > 0.0f) {
            //     continue;
            // }
            if (z > 0) {
                if (height - z < dist) {
                    dist = height - z;
                    *pheight = height;
                    ceil = surf;
                }
            } else {
                if (z - height < dist) {
                    dist = z - height;
                    *pheight = height;
                    ceil = surf;
                }
            }
            // break;
        }
    }

    //! (Surface Cucking) Since only the first ceil is returned and not the lowest,
    //  lower ceilings can be "cucked" by higher ceilings.
    return ceil;
}

static struct Surface *find_Zceil_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight) {
    register struct Surface *surf;
    struct MarioState *m = gMarioState;
    register s32 y1, x1, y2, x2, y3, x3;
    struct Surface *ceil = NULL;
    f32 dist = 99999.0f;
    ceil = NULL;

    // Stay in this loop until out of wallz.
    while (surfaceNode != NULL) {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        x1 = surf->vertex1[0];
        y1 = surf->vertex1[1];
        x2 = surf->vertex2[0];
        y2 = surf->vertex2[1];
        x3 = surf->vertex3[0];
        y3 = surf->vertex3[1];

        // Checking if point is in bounds of the triangle laterally.
        if ((y1 - y) * (x2 - x1) - (x1 - x) * (y2 - y1) > 0) {
            continue;
        }

        // Slight optimixation by checking these later.
        if ((y2 - y) * (x3 - x2) - (x2 - x) * (y3 - y2) > 0) {
            continue;
        }
        if ((y3 - y) * (x1 - x3) - (x3 - x) * (y1 - y3) > 0) {
            continue;
        }

        // Determine if checking for the camera or not.
        if (gCheckingSurfaceCollisionsForCamera != 0) {
            if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) {
                continue;
            }
        }
        // Ignore camera only surfaces.
        else if (surf->type == SURFACE_CAMERA_BOUNDARY) {
            continue;
        }

        {
            f32 nx = surf->normal.x;
            f32 ny = surf->normal.y;
            f32 nz = surf->normal.z;
            f32 oo = surf->originOffset;
            f32 height;

            if (ny != 0.0f) {
                continue;
            }

            // Find the ceil height at the specific point.
            height = -(y * ny + nx * x + oo) / nz;


            // if (z - (height - -78.0f) > 0.0f) {
            //     continue;
            // }
            // if (z > 0) {
                // if (height - z < dist) {
                    dist = height - z;
                    *pheight = height;
                    ceil = surf;
                // }
            // } else {
                // if (z - height < dist) {
                    // dist = z - height;
                    // *pheight = height;
                    // ceil = surf;
                // }
            // }
            // break;
        }
    }

    //! (Surface Cucking) Since only the first ceil is returned and not the lowest,
    //  lower ceilings can be "cucked" by higher ceilings.
    return ceil;
}



f32 find_surface_xplus(f32 posX, f32 posY, f32 posZ, struct Surface **pceil) {
    s16 cellZ, cellX;
    struct Surface *ceil, *dynamicCeil;
    struct SurfaceNode *surfaceList;
    f32 height = 20000.0f;
    f32 dynamicHeight = 20000.0f;
    s16 x, y, z;

    //! (Parallel Universes) Because position is casted to an s16, reaching higher
    // float locations  can return ceilings despite them not existing there.
    //(Dynamic ceilings will unload due to the range.)
    x = (s16) posX;
    y = (s16) posY;
    z = (s16) posZ;
    *pceil = NULL;

    if (x <= -LEVEL_BOUNDARY_MAX || x >= LEVEL_BOUNDARY_MAX) {
        return height;
    }
    if (z <= -LEVEL_BOUNDARY_MAX || z >= LEVEL_BOUNDARY_MAX) {
        return height;
    }

    // Each level is split into cells to limit load, find the appropriate cell.
    cellX = ((x + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & 0xF;
    cellZ = ((z + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & 0xF;

    // Check for surfaces belonging to objects.
    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    dynamicCeil = find_Xfloor_from_list(surfaceList, x, y, z, &dynamicHeight);

    // Check for surfaces that are a part of level geometry.
    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    ceil = find_Xfloor_from_list(surfaceList, x, y, z, &height);

    if (dynamicHeight < height) {
        ceil = dynamicCeil;
        height = dynamicHeight;
    }

    *pceil = ceil;

    // Increment the debug tracker.

    return height;
}

f32 find_surface_xminus(f32 posX, f32 posY, f32 posZ, struct Surface **pceil) {
    s16 cellZ, cellX;
    struct Surface *ceil, *dynamicCeil;
    struct SurfaceNode *surfaceList;
    f32 height = -20000.0f;
    f32 dynamicHeight = -20000.0f;
    s16 x, y, z;

    x = (s16) posX;
    y = (s16) posY;
    z = (s16) posZ;
    *pceil = NULL;

    if (x <= -LEVEL_BOUNDARY_MAX || x >= LEVEL_BOUNDARY_MAX) {
        return height;
    }
    if (z <= -LEVEL_BOUNDARY_MAX || z >= LEVEL_BOUNDARY_MAX) {
        return height;
    }

    // Each level is split into cells to limit load, find the appropriate cell.
    cellX = ((x + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & 0xF;
    cellZ = ((z + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & 0xF;
    
    gMarioState->numLives = cellX;
    gMarioState->numStars = cellZ;

    // Check for surfaces belonging to objects.
    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    dynamicCeil = find_Xceil_from_list(surfaceList, x, y, z, &dynamicHeight);

    // Check for surfaces that are a part of level geometry.
    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    ceil = find_Xceil_from_list(surfaceList, x, y, z, &height);

    if (dynamicHeight > height) {
        ceil = dynamicCeil;
        height = dynamicHeight;
    }

    *pceil = ceil;

    // Increment the debug tracker.

    return height;
}

f32 find_surface_zplus(f32 posX, f32 posY, f32 posZ, struct Surface **pceil) {
    s16 cellZ, cellX;
    struct Surface *ceil, *dynamicCeil;
    struct SurfaceNode *surfaceList;
    f32 height = 20000.0f;
    f32 dynamicHeight = 20000.0f;
    s16 x, y, z;

    //! (Parallel Universes) Because position is casted to an s16, reaching higher
    // float locations  can return ceilings despite them not existing there.
    //(Dynamic ceilings will unload due to the range.)
    x = (s16) posX;
    y = (s16) posY;
    z = (s16) posZ;
    *pceil = NULL;

    if (x <= -LEVEL_BOUNDARY_MAX || x >= LEVEL_BOUNDARY_MAX) {
        return height;
    }
    if (z <= -LEVEL_BOUNDARY_MAX || z >= LEVEL_BOUNDARY_MAX) {
        return height;
    }

    // Each level is split into cells to limit load, find the appropriate cell.
    cellX = ((x + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & 0xF;
    cellZ = ((z + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & 0xF;

    // Check for surfaces belonging to objects.
    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    dynamicCeil = find_Zfloor_from_list(surfaceList, x, y, z, &dynamicHeight);

    // Check for surfaces that are a part of level geometry.
    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    ceil = find_Zfloor_from_list(surfaceList, x, y, z, &height);

    if (dynamicHeight < height) {
        ceil = dynamicCeil;
        height = dynamicHeight;
    }

    *pceil = ceil;

    // Increment the debug tracker.

    return height;
}

f32 find_surface_zminus(f32 posX, f32 posY, f32 posZ, struct Surface **pceil) {
    s16 cellZ, cellX;
    struct Surface *ceil, *dynamicCeil;
    struct SurfaceNode *surfaceList;
    f32 height = -20000.0f;
    f32 dynamicHeight = -20000.0f;
    s16 x, y, z;

    x = (s16) posX;
    y = (s16) posY;
    z = (s16) posZ;
    *pceil = NULL;

    if (x <= -LEVEL_BOUNDARY_MAX || x >= LEVEL_BOUNDARY_MAX) {
        return height;
    }
    if (z <= -LEVEL_BOUNDARY_MAX || z >= LEVEL_BOUNDARY_MAX) {
        return height;
    }

    // Each level is split into cells to limit load, find the appropriate cell.
    cellX = ((x + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & 0xF;
    cellZ = ((z + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & 0xF;
    
    gMarioState->numLives = cellX;
    gMarioState->numStars = cellZ;

    // Check for surfaces belonging to objects.
    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    dynamicCeil = find_Zceil_from_list(surfaceList, x, y, z, &dynamicHeight);

    // Check for surfaces that are a part of level geometry.
    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    ceil = find_Zceil_from_list(surfaceList, x, y + 60.0f, z, &height);

    if (dynamicHeight > height) {
        ceil = dynamicCeil;
        height = dynamicHeight;
    }

    *pceil = ceil;


    return height;
}
