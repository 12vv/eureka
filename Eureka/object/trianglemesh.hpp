//
//  trianglemesh.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/2/1.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef TRIANGLEMESH_HPP
#define TRIANGLEMESH_HPP

#include "object.hpp"



//bool rayTriangleIntersect(
//                          const Ray &ray,
//                          const Vec3f &v0, const Vec3f &v1, const Vec3f &v2,
//                          float &t, float &u, float &v)
//{
//    Vec3f orig = ray.origin();
//    Vec3f dir = ray.direction();
//#ifdef MOLLER_TRUMBORE
//    Vec3f v0v1 = v1 - v0;
//    Vec3f v0v2 = v2 - v0;
//    Vec3f pvec = dir.crossProduct(v0v2);
//    float det = v0v1.dotProduct(pvec);
//#ifdef CULLING
//    // if the determinant is negative the triangle is backfacing
//    // if the determinant is close to 0, the ray misses the triangle
//    if (det < kEpsilon) return false;
//#else
//    // ray and triangle are parallel if det is close to 0
//    if (fabs(det) < kEpsilon) return false;
//#endif
//    float invDet = 1 / det;
//
//    Vec3f tvec = orig - v0;
//    u = tvec.dotProduct(pvec) * invDet;
//    if (u < 0 || u > 1) return false;
//
//    Vec3f qvec = tvec.crossProduct(v0v1);
//    v = dir.dotProduct(qvec) * invDet;
//    if (v < 0 || u + v > 1) return false;
//
//    t = v0v2.dotProduct(qvec) * invDet;
//
//    return true;
//#else
//    // compute plane's normal
//    Vec3f v0v1 = v1 - v0;
//    Vec3f v0v2 = v2 - v0;
//    // no need to normalize
//    Vec3f N = v0v1.crossProduct(v0v2); // N
//    float denom = N.dotProduct(N);
//
//    // Step 1: finding P
//
//    // check if ray and plane are parallel ?
//    float NdotRayDirection = N.dotProduct(dir);
//    if (fabs(NdotRayDirection) < kEpsilon) // almost 0
//        return false; // they are parallel so they don't intersect !
//
//    // compute d parameter using equation 2
//    float d = N.dotProduct(v0);
//
//    // compute t (equation 3)
//    t = (N.dotProduct(orig) + d) / NdotRayDirection;
//    // check if the triangle is in behind the ray
//    if (t < 0) return false; // the triangle is behind
//
//    // compute the intersection point using equation 1
//    Vec3f P = orig + t * dir;
//
//    // Step 2: inside-outside test
//    Vec3f C; // vector perpendicular to triangle's plane
//
//    // edge 0
//    Vec3f edge0 = v1 - v0;
//    Vec3f vp0 = P - v0;
//    C = edge0.crossProduct(vp0);
//    if (N.dotProduct(C) < 0) return false; // P is on the right side
//
//    // edge 1
//    Vec3f edge1 = v2 - v1;
//    Vec3f vp1 = P - v1;
//    C = edge1.crossProduct(vp1);
//    if ((u = N.dotProduct(C)) < 0)  return false; // P is on the right side
//
//    // edge 2
//    Vec3f edge2 = v0 - v2;
//    Vec3f vp2 = P - v2;
//    C = edge2.crossProduct(vp2);
//    if ((v = N.dotProduct(C)) < 0) return false; // P is on the right side;
//
//    u /= denom;
//    v /= denom;
//
//    return true; // this ray hits the triangle
//#endif
//}

bool rayTriangleIntersect(
                          const Ray &ray,
                          const Vec3f &v0, const Vec3f &v1, const Vec3f &v2,
                          float &t, float &u, float &v)
{
    Vec3f orig = ray.origin();
    Vec3f dir = ray.direction();
    
    Vec3f v0v1 = v1 - v0;
    Vec3f v0v2 = v2 - v0;
    Vec3f pvec = dir.crossProduct(v0v2);
    float det = v0v1.dotProduct(pvec);
    
    // ray and triangle are parallel if det is close to 0
    if (fabs(det) < kEpsilon) return false;
    
    float invDet = 1 / det;
    
    Vec3f tvec = orig - v0;
    u = tvec.dotProduct(pvec) * invDet;
    if (u < 0 || u > 1) return false;
    
    Vec3f qvec = tvec.crossProduct(v0v1);
    v = dir.dotProduct(qvec) * invDet;
    if (v < 0 || u + v > 1) return false;
    
    t = v0v2.dotProduct(qvec) * invDet;
    
    return (t > 0) ? true : false;
}

class TriangleMesh : public Object{
public:
    TriangleMesh(const Matrix44f &o2w,
                 Material *m,
                 const uint32_t nfaces,
                 const std::unique_ptr<uint32_t []> &faceIndex,
                 const std::unique_ptr<uint32_t []> &vertsIndex,
                 const std::unique_ptr<Vec3f []> &verts,
                 std::unique_ptr<Vec3f []> &normals,
                 std::unique_ptr<Vec2f []> &st) :
    Object(o2w, m),
    numTris(0)
    {
        uint32_t k = 0, maxVertIndex = 0;
        // find out how many triangles we need to create for this mesh
        for (uint32_t i = 0; i < nfaces; ++i) {
            numTris += faceIndex[i] - 2;
            for (uint32_t j = 0; j < faceIndex[i]; ++j)
                if (vertsIndex[k + j] > maxVertIndex)
                    maxVertIndex = vertsIndex[k + j];
            k += faceIndex[i];
        }
        maxVertIndex += 1;
        
        // allocate memory to store the position of the mesh vertices
        P = std::unique_ptr<Vec3f []>(new Vec3f[maxVertIndex]);
        for (uint32_t i = 0; i < maxVertIndex; ++i) {
            // transforming vertices to world space
            objectToWorld.multPtMatrix(verts[i], P[i]);
        }
        // allocate memory to store triangle indices
        trisIndex = std::unique_ptr<uint32_t []>(new uint32_t [numTris * 3]);
        uint32_t l = 0;
        N = std::unique_ptr<Vec3f []>(new Vec3f[numTris * 3]);
        sts = std::unique_ptr<Vec2f []>(new Vec2f[numTris * 3]);
        
        Matrix44f transformNormals = worldToObject.transpose();
        // generate the triangle index array and set normals and st coordinates
        for (uint32_t i = 0, k = 0; i < nfaces; ++i) { // for each  face
            for (uint32_t j = 0; j < faceIndex[i] - 2; ++j) { // for each triangle in the face
                trisIndex[l] = vertsIndex[k];
                trisIndex[l + 1] = vertsIndex[k + j + 1];
                trisIndex[l + 2] = vertsIndex[k + j + 2];
                
                transformNormals.multVecMatrix(normals[k], N[l]);
                transformNormals.multVecMatrix(normals[k + j + 1], N[l + 1]);
                transformNormals.multVecMatrix(normals[k + j + 2], N[l + 2]);
                N[l].normalize();
                N[l + 1].normalize();
                N[l + 2].normalize();
                sts[l] = st[k];
                sts[l + 1] = st[k + j + 1];
                sts[l + 2] = st[k + j + 2];
                l += 3;
            }
            k += faceIndex[i];
        }
    }
    
    bool intersect(const Ray &ray, float &tNear,
                   uint32_t &triIndex,
                   Vec2f &uv) const
    {
        uint32_t j = 0;
        bool isect = false;
        for(uint32_t i = 0; i < numTris; ++i){
            const Vec3f &v0 = P[trisIndex[j]];
            const Vec3f &v1 = P[trisIndex[j+1]];
            const Vec3f &v2 = P[trisIndex[j+2]];
            float t = kInfinity, u, v;
            if(rayTriangleIntersect(ray, v0, v1, v2, t, u, v) && t < tNear){
                tNear = t;
                uv.x = u;
                uv.y = v;
                triIndex = i;
                isect = true;
            }
            j += 3;
        }
        return isect;
    }

    void getSurfaceData(const Vec3f &Phit,
                        const Vec3f &viewDirection,
                        const uint32_t &triIndex,   // not used for sphere
                        const Vec2f &uv,            // not used for sphere
                        Vec3f &Nhit,
                        Vec2f &textureCoordinates) const
    {
        if(smoothShading){
            // vertex normal
            const Vec3f &n0 = N[triIndex * 3];
            const Vec3f &n1 = N[triIndex * 3 + 1];
            const Vec3f &n2 = N[triIndex * 3 + 2];
            Nhit = (1 - uv.x - uv.y) * n0 + uv.x * n1 + uv.y * n2;
        }else{
            // face normal
            const Vec3f &v0 = P[trisIndex[triIndex * 3]];
            const Vec3f &v1 = P[trisIndex[triIndex * 3 + 1]];
            const Vec3f &v2 = P[trisIndex[triIndex * 3 + 2]];
            Nhit = (v1 - v0).crossProduct(v2 - v0);
        }
        // for safety
        Nhit.normalize();
        // texture coordinates
        const Vec2f &st0 = sts[triIndex * 3];
        const Vec2f &st1 = sts[triIndex * 3 + 1];
        const Vec2f &st2 = sts[triIndex * 3 + 2];
        textureCoordinates = (1 - uv.x - uv.y) * st0 + uv.x * st1 + uv.y * st2;
    }
    // member variables
    uint32_t numTris;                       // number of triangles
    std::unique_ptr<Vec3f []> P;            // triangles vertex position
    std::unique_ptr<uint32_t []> trisIndex; // vertex index array
    std::unique_ptr<Vec3f []> N;            // triangles vertex normals
    std::unique_ptr<Vec2f []> sts;          // triangles texture coordinates
    bool smoothShading = true;              // smooth shading by default
};


#endif /* trianglemesh_h */
