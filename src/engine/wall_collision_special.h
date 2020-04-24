#ifndef _WALL_COLLISION_SPECIAL_H
#define _WALL_COLLISION_SPECIAL_H

#define LEVEL_BOUNDARY_MAX 0x2000
#define CELL_SIZE          0x400

f32 find_surface_xplus(f32 posX, f32 posY, f32 posZ, struct Surface **xSurf);
f32 find_surface_zplus(f32 posX, f32 posY, f32 posZ, struct Surface **zSurf);
f32 find_surface_xminus(f32 posX, f32 posY, f32 posZ, struct Surface **xSurf);
f32 find_surface_zminus(f32 posX, f32 posY, f32 posZ, struct Surface **zSurf);


#endif /* _WALL_COLLISION_SPECIAL_H */
