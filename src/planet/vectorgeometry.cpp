#include "vectorgeometry.h"

namespace VectorGeometry {


vmath::Vector3 linePlaneIntersection(vmath::Vector3 line_dir, vmath::Vector3 line_point,
                                     vmath::Vector3 plane_normal, vmath::Vector3 plane_point)
{
    // from wikipedia

    float discr = vmath::dot(plane_normal, line_dir);

    float d = 0.0f;
    if (discr==0.0f) // could do with less branching
    {
        std::cerr << "Error in line-plane intersection: line parallel to plane, " <<
                     "using line point as intersection point" << std::endl;
    }
    else
    {
        d = vmath::dot(plane_point-line_point, plane_normal)/discr;
    }

    return line_point + d*line_dir;
}

vmath::Vector3 linePlaneIntersectionNormalized(vmath::Vector3 line_dir_normalized, vmath::Vector3 line_point,
                                               vmath::Vector3 plane_normal_normalized, vmath::Vector3 plane_point)
{
    assert(abs(vmath::length(line_dir_normalized)-1.f)<0.0001);
    assert(abs(vmath::length(plane_normal_normalized)-1.f)<0.0001);

    float d = vmath::dot(plane_point-line_point, plane_normal_normalized);
    return line_point + d*line_dir_normalized;
}

inline vmath::Vector3 projectPointIntoPlane(vmath::Vector3 point,
                                            vmath::Vector3 plane_normal,
                                            vmath::Vector3 plane_point)
{
    return linePlaneIntersection(plane_normal, point, plane_normal, plane_point);
}


inline float areaOfTriangle(const vmath::Vector3 &tp0,
                            const vmath::Vector3 &tp1,
                            const vmath::Vector3 &tp2)
{
    vmath::Vector3 vt01 = tp1 - tp0;
    vmath::Vector3 vt02 = tp2 - tp0;
    vmath::Vector3 cross_vt = vmath::cross(vt01, vt02);
    float ta = vmath::length(cross_vt)/2.0f; // can't be zero!
    return ta;
}



inline float pointPlaneSquareDistance(  const vmath::Vector3 &point,
                                        const vmath::Vector3 &plane_normal,
                                        const vmath::Vector3 &plane_point )
{
//    float D = -vmath::dot(plane_point, plane_normal);
//    float n_squared = vmath::dot(plane_normal, plane_normal);
//    float d_squared = std::pow(vmath::dot(plane_normal, point)+D, 2.0f)/n_squared;

//    return d_squared;

    // alt
    return std::pow(vmath::dot(plane_normal, point-plane_point),2.0f)/vmath::dot(plane_normal, plane_normal);;

}

inline vmath::Vector3 triangleCross_inl(const vmath::Vector3 &tp0,
                                     const vmath::Vector3 &tp1,
                                     const vmath::Vector3 &tp2)
{
    vmath::Vector3 vt01 = tp1 - tp0;
    vmath::Vector3 vt02 = tp2 - tp0;
    vmath::Vector3 cross_vt = vmath::cross(vt01, vt02);
    return cross_vt;
}

vmath::Vector3 triangleCross(const vmath::Vector3 &tp0,
                                     const vmath::Vector3 &tp1,
                                     const vmath::Vector3 &tp2)
{
    return triangleCross_inl(tp0, tp1, tp2);
}


vmath::Vector3 triangleNormal(const vmath::Vector3 &tp0,
                             const vmath::Vector3 &tp1,
                             const vmath::Vector3 &tp2)
{
    return triangleCross_inl(tp0, tp1, tp2);
}

vmath::Vector3 triangleUnitNormal(const vmath::Vector3 &tp0,
                                  const vmath::Vector3 &tp1,
                                  const vmath::Vector3 &tp2)
{
    return vmath::normalize(triangleCross_inl(tp0, tp1, tp2));
}


float areaOfTriangle(const vmath::Vector3 tri_cross)
{
    float ta = vmath::length(tri_cross)/2.0f; // can't be zero!
    return ta;
}

vmath::Vector3 baryPointInTriangle(const vmath::Vector3 &p,
                                   const vmath::Vector3 &tp0,
                                   const vmath::Vector3 &tp1,
                                   const vmath::Vector3 &tp2)
{
    return baryPointInTriangle(p, tp0, tp1, tp2, triangleCross_inl(tp0, tp1, tp2));
}

vmath::Vector3 baryPointInTriangle(const vmath::Vector3 &p,
                                   const vmath::Vector3 &tp0,
                                   const vmath::Vector3 &tp1,
                                   const vmath::Vector3 &tp2,
                                   const vmath::Vector3 &tri_cross)
{
    // assert that the point is in triangle
    assert( (pointPlaneSquareDistance(p, triangleNormal(tp0, tp1, tp2), tp0) <
             0.01f*(vmath::lengthSqr(tp0-tp1))) );

    float ta = areaOfTriangle(tri_cross);

    // find vectors from triangle corners to point
    vmath::Vector3 vpt0 = tp0 - p;
    vmath::Vector3 vpt1 = tp1 - p;
    vmath::Vector3 vpt2 = tp2 - p;

    // find the areas of the triangles between edges and the point
    float ap01 = vmath::length(vmath::cross(vpt0, vpt1))/2.0f;
    float ap02 = vmath::length(vmath::cross(vpt0, vpt2))/2.0f;
    float ap12 = vmath::length(vmath::cross(vpt1, vpt2))/2.0f;

    // barycentric coordinates ( = area opposing corner/total triangle area)
    float b0 = ap12/ta;
    float b1 = ap02/ta;
    float b2 = ap01/ta;

    return vmath::Vector3(b0, b1, b2);
}

////can't inline?
//vmath::Vector3 barycentricCoords(const vmath::Vector3 &pt,
//                                        const vmath::Vector3 &tp0,
//                                        const vmath::Vector3 &tp1,
//                                        const vmath::Vector3 &tp2)
//{
//    // This method is an optimization candidate

//    //find the total triangle area ( = half the length of cross product of two side vectors)
//    vmath::Vector3 vt01 = tp1 - tp0;
//    vmath::Vector3 vt02 = tp2 - tp0;
//    vmath::Vector3 cross_vt = vmath::cross(vt01, vt02);
//    float ta = vmath::length(cross_vt)/2.0f; // can't be zero!

//    // project the point onto the triangle

//    vmath::Vector3 normal = vmath::normalize(cross_vt);
//    vmath::Vector3 pt_normal = vmath::normalize(pt);

//    std::cout << "pt: " << pt[0] << ", " << pt[1] << ", " <<pt[2] << std::endl;
//    std::cout << "pt_normal: " << pt_normal[0] << ", " << pt_normal[1] << ", " <<pt_normal[2] << std::endl;
//    std::cout << "normal: " << normal[0] << ", " << normal[1] << ", " <<normal[2] << std::endl;
//    std::cout << "tp0: " << tp0[0] << ", " << tp0[1] << ", " <<tp0[2] << std::endl;


//    vmath::Vector3 p = linePlaneIntersection(pt_normal, pt, normal, tp0);

//    std::cout << "p: " << p[0] << ", " <<p[1] << ", " <<p[2] << std::endl;

//    // find vectors from triangle corners to point
//    vmath::Vector3 vpt0 = tp0 - p;
//    vmath::Vector3 vpt1 = tp1 - p;
//    vmath::Vector3 vpt2 = tp2 - p;

//    // find the areas of the triangles between edges and the point
//    float ap01 = vmath::length(vmath::cross(vpt0, vpt1))/2.0f;
//    float ap02 = vmath::length(vmath::cross(vpt0, vpt2))/2.0f;
//    float ap12 = vmath::length(vmath::cross(vpt1, vpt2))/2.0f;

//    // barycentric coordinates ( = area opposing corner/total triangle area)
//    float b0 = ap12/ta;
//    float b1 = ap02/ta;
//    float b2 = ap01/ta;

//    std::cout << "ta: " << ta << std::endl;

//    return vmath::Vector3(b0, b1, b2);
//}

static constexpr float T_EPS = 0.000001f;

bool pointInTriangle(const vmath::Vector3 &barycentric_coords)
{
    //std::cout << "pointInTriangle" << std::endl;
    //std::cout << "barycentric_coords: "; vmath::print(barycentric_coords);

    const vmath::Vector3 &b = barycentric_coords;
    if (b[0]+b[1]+b[2]>1.0f+T_EPS) return false; else return true;
}

int multinomialCoefficient2(int i, int j, int k)
{
    assert(( (i+j+k==2) ));

    return std::max(i,std::max(j,k))==2 ? 1 : 2;
}

inline float bernesteinBarycentric(vmath::Vector3 b, int i, int j, int k)
{

    return (float)(multinomialCoefficient2(i, j, k)) * pow(b[0], i) * pow(b[1], j) * pow(b[2], k);
}

//vmath::Vector3 findPointInTriangle(const vmath::Vector3 &point,
//                                   const gfx::Triangle &triangle,
//                                   const std::vector<vmath::Vector3> &points,
//                                   const std::vector<vmath::Vector3> &point_normals)
//{
//    // project triangle to unit sphere
//    vmath::Vector3 pt = point;
//    vmath::Vector3 tp0 = vmath::normalize(points[triangle[0]]);
//    vmath::Vector3 tp1 = vmath::normalize(points[triangle[1]]);
//    vmath::Vector3 tp2 = vmath::normalize(points[triangle[2]]);

//    // find barycentric coordinates
//    vmath::Vector3 b = barycentricCoords(pt, tp0, tp1, tp2);


vmath::Vector3 findPointInTriangle(const vmath::Vector3 &point,
                                   const gfx::Triangle &triangle,
                                   const vmath::Vector3 &barycentric_coords,
                                   const std::vector<vmath::Vector3> &points,
                                   const std::vector<vmath::Vector3> &point_normals)
{
    vmath::Vector3 pt = vmath::normalize(point);
    vmath::Vector3 b = barycentric_coords;
    assert((b[0]+b[1]+b[2]<=1.0f+T_EPS));

    // evaluate bernstein basis at barycentric coords (6 polys)
    float bb200 = bernesteinBarycentric(b, 2,0,0); // bary, n = 2, i, j, k -> n is degree of polynomial
    float bb020 = bernesteinBarycentric(b, 0,2,0);
    float bb002 = bernesteinBarycentric(b, 0,0,2);

    float bb110 = bernesteinBarycentric(b, 1,1,0);
    float bb101 = bernesteinBarycentric(b, 1,0,1);
    float bb011 = bernesteinBarycentric(b, 0,1,1);

    // find the... fuark need edge normals for 3 edge planes
    // take the edge normals to be avg of point normals for now...
    vmath::Vector3 n01 = 0.5f*(point_normals[triangle[0]]+point_normals[triangle[1]]);
    vmath::Vector3 n02 = 0.5f*(point_normals[triangle[0]]+point_normals[triangle[2]]);
    vmath::Vector3 n12 = 0.5f*(point_normals[triangle[1]]+point_normals[triangle[2]]);

    // find the plane intersections
    vmath::Vector3 origin = vmath::Vector3(0.f, 0.f, 0.f);
    float z_n0 = vmath::length(linePlaneIntersection(pt, origin, point_normals[triangle[0]], points[triangle[0]]));
    float z_n1 = vmath::length(linePlaneIntersection(pt, origin, point_normals[triangle[1]], points[triangle[1]]));
    float z_n2 = vmath::length(linePlaneIntersection(pt, origin, point_normals[triangle[2]], points[triangle[2]]));

    float z_n01 = vmath::length(linePlaneIntersection(pt, origin, n01, 0.5f*(points[triangle[0]]+points[triangle[1]])));
    float z_n02 = vmath::length(linePlaneIntersection(pt, origin, n02, 0.5f*(points[triangle[0]]+points[triangle[2]])));
    float z_n12 = vmath::length(linePlaneIntersection(pt, origin, n12, 0.5f*(points[triangle[1]]+points[triangle[2]])));

    float z = bb200*z_n0 + bb020*z_n1 + bb002*z_n2 + bb110*z_n01 + bb101*z_n02 + bb011*z_n12;

    return z*pt;
}

}