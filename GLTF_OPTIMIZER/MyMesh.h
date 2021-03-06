#pragma once
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "vcg/complex/complex.h"
#include "wrap/io_trimesh/export_ply.h"
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>

using namespace vcg;
using namespace tri;
using namespace std;
namespace tinygltf
{
	class Material;
}
// The class prototypes.
class MyVertex;
class MyEdge;
class MyFace;

struct MyUsedTypes : public UsedTypes<Use<MyVertex>::AsVertexType, Use<MyEdge>::AsEdgeType, Use<MyFace>::AsFaceType> {};
class MyVertex : public Vertex< MyUsedTypes,
    vertex::VFAdj,
    vertex::Coord3f,
	vertex::Color4b,// The color component will be the one to store batchId, stored in little endian
    vertex::Normal3f,
    vertex::Mark,
    vertex::Qualityf,
    vertex::BitFlags,
	vertex::Coord3f> {
public:
    vcg::math::Quadric<double> &Qd() { return q; }
private:
    math::Quadric<double> q;
};

class MyEdge : public Edge< MyUsedTypes> {};

typedef BasicVertexPair<MyVertex> VertexPair;

class MyFace : public Face < MyUsedTypes,
    face::VFAdj,
    face::FFAdj,
    face::Mark,
    face::Normal3f,
    face::WedgeRealNormal3f,
    face::VertexRef,
    face::BitFlags > {};

// the main mesh class
class MyMesh : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace> > 
{
public:
        bool hasUv;
};


class MyTriEdgeCollapse : public vcg::tri::TriEdgeCollapseQuadric< MyMesh, VertexPair, MyTriEdgeCollapse, QInfoStandard<MyVertex>  > {
public:
    typedef  vcg::tri::TriEdgeCollapseQuadric< MyMesh, VertexPair, MyTriEdgeCollapse, QInfoStandard<MyVertex>  > TECQ;
    typedef  MyMesh::VertexType::EdgeType EdgeType;
    inline MyTriEdgeCollapse(const VertexPair &p, int i, BaseParameterClass *pp) :TECQ(p, i, pp) {}
};

typedef typename MyMesh::VertexPointer VertexPointer;
typedef typename MyMesh::ScalarType ScalarType;
typedef typename MyMesh::VertexType VertexType;
typedef typename MyMesh::FaceType FaceType;
typedef typename MyMesh::VertexIterator VertexIterator;
typedef typename MyMesh::FaceIterator FaceIterator;
typedef typename MyMesh::EdgeIterator EdgeIterator;

struct MyMeshInfo
{
    MyMesh* myMesh;
    tinygltf::Material* material;
};

struct TileInfo
{
    int level;
    std::vector<MyMeshInfo> myMeshInfos;
    std::vector<TileInfo*> children;
    vcg::Box3f* boundingBox;
    float geometryError;
    int originalVertexCount;
    std::string contentUri;
};