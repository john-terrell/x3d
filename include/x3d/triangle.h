/*
 *  triangle.h
 *  x3d
 *
 *  Created by John on 1/13/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef x3d__triangle_h
#define x3d__triangle_h

int coplanar_tri_tri(float N[3],float V0[3],float V1[3],float V2[3], float U0[3],float U1[3],float U2[3]);

int tri_tri_intersect(float V0[3],float V1[3],float V2[3], float U0[3],float U1[3],float U2[3]);
					  
int tri_tri_intersect_with_isectline(float V0[3],float V1[3],float V2[3], float U0[3],float U1[3],float U2[3],int *coplanar, float isectpt1[3],float isectpt2[3]);

int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3], float U0[3],float U1[3],float U2[3]);

#endif // x3d__triangle_h
