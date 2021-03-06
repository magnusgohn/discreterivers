#ifndef MULTICALCULUS
#define MULTICALCULUS_H

#include "../common/gfx_primitives.h"

#include <utility>


// should make this header only...

namespace vmath = Vectormath::Aos;

namespace VectorGeometry
{
vmath::Vector3 linePlaneIntersection(vmath::Vector3 line_dir, vmath::Vector3 line_point,
                                     vmath::Vector3 plane_normal, vmath::Vector3 plane_point);

vmath::Vector3 projectPointIntoPlane(vmath::Vector3 point,
                                     vmath::Vector3 plane_normal,
                                     vmath::Vector3 plane_point);

/*struct Line {vmath::Vector3 point, dir;};
  Line planePlaneIntersection(vmath::Vector3 p1, vmath::Vector3 n1, vmath::Vector3 p2, vmath::Vector3 n2);*/

vmath::Vector3 triplePlaneIntersection(vmath::Vector3 n1, vmath::Vector3 p1,
                                       vmath::Vector3 n2, vmath::Vector3 p2,
                                       vmath::Vector3 n3, vmath::Vector3 p3);

vmath::Vector3 baryPointInTriangle(const vmath::Vector3 &p,
                                   const vmath::Vector3 &tp0,
                                   const vmath::Vector3 &tp1,
                                   const vmath::Vector3 &tp2);


vmath::Vector3 baryPointInTriangle(const vmath::Vector3 &p,
                                   const vmath::Vector3 &tp0,
                                   const vmath::Vector3 &tp1,
                                   const vmath::Vector3 &tp2,
                                   const vmath::Vector3 &tri_cross);

vmath::Vector3 findSphereBarycentric(const vmath::Vector3 &pt,   const vmath::Vector3 &tp0,
                                     const vmath::Vector3 &tp1,  const vmath::Vector3 &tp2);

vmath::Vector3 evalCubicBezierTri(const vmath::Vector3 &barycentric,
                                  const std::vector<vmath::Vector3> &control_points); // ten control points



vmath::Vector3 triangleNormal(const vmath::Vector3 &tp0,
                              const vmath::Vector3 &tp1,
                              const vmath::Vector3 &tp2);

vmath::Vector3 triangleCross(const vmath::Vector3 &tp0,
                             const vmath::Vector3 &tp1,
                             const vmath::Vector3 &tp2);

vmath::Vector3 triangleUnitNormal(const vmath::Vector3 &tp0,
                                  const vmath::Vector3 &tp1,
                                  const vmath::Vector3 &tp2);

//vmath::Vector3 barycentricCoords(const vmath::Vector3 &pt,
//                                const vmath::Vector3 &tp0,
//                                const vmath::Vector3 &tp1,
//                                const vmath::Vector3 &tp2);

bool pointInTriangle(const vmath::Vector3 &barycentric_coords);

int multinomialCoefficient2(int i, int j, int k);

vmath::Vector3 findPointInTriangle(const vmath::Vector3 &point,
                                   const gfx::Triangle &triangle,
                                   const vmath::Vector3 &barycentric_coords,
                                   const std::vector<vmath::Vector3> &points,
                                   const std::vector<vmath::Vector3> &point_normals);

}

#endif // MULTICALCULUS_H
