#ifndef DV2520_OBJINSTANCE_FX
#define DV2520_OBJINSTANCE_FX

struct ObjInstance {
	row_major float4x4 worldInv;
	uint indexStart;
	uint indexCnt;
	uint vertexCnt;
	uint pad;
};

#endif // DV2520_OBJINSTANCE_FX