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

class TriangleMesh : public Object{
public:
    TraingleMesh(const Matrix44f &o2w,
                 const uint32_t nfaces,
                 const std::unique_ptr<uint32_t []> &faceIndex,
                 const std::unique_ptr<uint32_t []> &vertsIndex,
                 const std::unique_ptr<Vec3f []> &verts,
                 std::unique_ptr<Vec3f []> &normals,
                 std::unique_ptr<Vec2f []> &st) :
    Object(o2w),
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
            objectToWorld.multVecMatrix(verts[i], P[i]);
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
                
                transformNormals.multDirMatrix(normals[k], N[l]);
                transformNormals.multDirMatrix(normals[k + j + 1], N[l + 1]);
                transformNormals.multDirMatrix(normals[k + j + 2], N[l + 2]);
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
    }
    uint32_t numTris;                       // number of triangles
    std::unique_ptr<Vec3f []> P;            // triangles vertex position
    std::unique_ptr<uint32_t []> trisIndex; // vertex index array
    std::unique_ptr<Vec3f []> N;            // triangles vertex normals
    std::unique_ptr<Vec2f []> sts;          // triangles texture coordinates
    bool smoothShading = true;              // smooth shading by default
};


#endif /* trianglemesh_h */
