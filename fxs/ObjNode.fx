#ifndef DV2520_OBJNODE_FX
#define DV2520_OBJNODE_FX

struct ObjNode {
       float3 min;
       float3 max;
       int primitivesCnt;
       uint primitivesIdx;
};

#endif // DV2520_OBJNODE_FX