#ifndef DV2520_RDROBJ_H
#define DV2520_RDROBJ_H

#include <Vertex.h>
#include <ObjFace.h>
#include <MathSimple.h>

class Obj;

static const char RDROBJ_SEPARATOR_DEFAULT	= ' ';
static const char RDROBJ_SEPARATOR_FACE		= '/';

static const std::string RDROBJ_INDICATOR_VERTEX		= "v";		// Vertex
static const std::string RDROBJ_INDICATOR_TEX			= "vt";		// Texture coordinates
static const std::string RDROBJ_INDICATOR_NORM			= "vn";		// Normal
static const std::string RDROBJ_INDICATOR_FACE			= "f";		// Face
static const std::string RDROBJ_INDICATOR_GROUP			= "g";		// Group
static const std::string RDROBJ_INDICATOR_MATERIAL		= "mtllib";	// Material Template Library
static const std::string RDROBJ_INDICATOR_MATERIAL_USE	= "usemtl"; // Use MTL

static const unsigned int RDROBJ_PARAMS						= 1;
static const unsigned int RDROBJ_PARAMS_NUM_VERTEX			= 3 + RDROBJ_PARAMS;	// v x y z
static const unsigned int RDROBJ_PARAMS_NUM_VERTEX_OPTIONAL	= 4 + RDROBJ_PARAMS;	// v x y z w
static const unsigned int RDROBJ_PARAMS_NUM_TEX				= 2 + RDROBJ_PARAMS;	// vt u v
static const unsigned int RDROBJ_PARAMS_NUM_TEX_OPTIONAL	= 3 + RDROBJ_PARAMS;	// vt u v w
static const unsigned int RDROBJ_PARAMS_NUM_NORM			= 3 + RDROBJ_PARAMS;	// vn x y z
static const unsigned int RDROBJ_PARAMS_NUM_FACES			= 3 + RDROBJ_PARAMS;	// f v/vt/vn v/vt/vn v/vt/vn
static const unsigned int RDROBJ_PARAMS_NUM_FACE			= 3;					// v vt vn
static const unsigned int RDROBJ_PARAMS_NUM_GROUP			= 1 + RDROBJ_PARAMS;	// g groupname
static const unsigned int RDROBJ_PARAMS_NUM_MATERIAL		= 1 + RDROBJ_PARAMS;	// mtllib material.mtl
static const unsigned int RDROBJ_PARAMS_NUM_MATERIAL_USE	= 1 + RDROBJ_PARAMS;	// usemtl material123

static const unsigned int RDROBJ_PARAMS_INDEX_VERTEX_X		= 0 + RDROBJ_PARAMS;
static const unsigned int RDROBJ_PARAMS_INDEX_VERTEX_Y		= 1 + RDROBJ_PARAMS;
static const unsigned int RDROBJ_PARAMS_INDEX_VERTEX_Z		= 2 + RDROBJ_PARAMS;
static const unsigned int RDROBJ_PARAMS_INDEX_VERTEX_W		= 3 + RDROBJ_PARAMS;
static const unsigned int RDROBJ_PARAMS_INDEX_TEX_U			= 0 + RDROBJ_PARAMS;
static const unsigned int RDROBJ_PARAMS_INDEX_TEX_V			= 1 + RDROBJ_PARAMS;
static const unsigned int RDROBJ_PARAMS_INDEX_TEX_W			= 2 + RDROBJ_PARAMS;
static const unsigned int RDROBJ_PARAMS_INDEX_FACE_X		= 0;
static const unsigned int RDROBJ_PARAMS_INDEX_FACE_Y		= 1;
static const unsigned int RDROBJ_PARAMS_INDEX_FACE_Z		= 2;
static const unsigned int RDROBJ_PARAMS_INDEX_GROUP_NAME	= 0 + RDROBJ_PARAMS;

// Describes a read symbol. An action is then selected based off of this enum.
enum RdrObjSymbol { 
	RdrObjSymbol_VERTEX,		// v
	RdrObjSymbol_TEX,			// vt
	RdrObjSymbol_NORM,			// vn
	RdrObjSymbol_FACE,			// f
	RdrObjSymbol_GROUP,			// g	
	RdrObjSymbol_MATERIAL,		// mtllib
	RdrObjSymbol_MATERIAL_USE,	// usemtl

	RdrObjSymbol_IGNORE,
	RdrObjSymbol_INVALID
};
enum RdrObjFaceParam
{
	RdrObjFaceParam_VERTEX_INDEX	= 0x01,
	RdrObjFaceParam_TEXCOORD_INDEX	= 0x02,
	RdrObjFaceParam_NORMAL_INDEX	= 0x04
};

class RdrObj {
public:
	RdrObj( std::string p_filePath, std::string p_fileName );
	~RdrObj();

	Obj* read( bool& io_sucess );
protected:
private:
	Obj* readObj();

	void getLine( std::string& io_line );
	std::vector< std::string >& split( const std::string& p_s, char p_d, std::vector< std::string >& io_split );
	RdrObjSymbol getSymbol( const std::vector< std::string >& p_params );
	void loadSymbol( const RdrObjSymbol p_symbol, const std::vector< std::string >& p_params );
	void loadPos( const std::vector< std::string >& p_params );
	void loadNorm( const std::vector< std::string >& p_params );
	void loadTex( const std::vector< std::string >& p_params );
	void loadFaces( const std::vector< std::string >& p_params );
	unsigned parseFaceParams( const std::string& p_face );
	void loadFace( const std::vector<std::string>& p_face, const unsigned int p_faceParams );
	unsigned loadVertex( unsigned p_iPos, unsigned p_iTex, unsigned p_iNorm);

	unsigned m_lineCnt;
	std::string m_filePath;
	std::string m_fileName;
	std::string m_fileFull;
	std::ifstream m_ifs;

	// Intermediate variables used when parsing .objs.
	std::vector< Vec3F > m_pos;	// Spatial attributes read from file.
	std::vector< Vec3F > m_nor;	// Normal attributes read from file.
	std::vector< Vec2F > m_tex;	// Texture elements read from file.
	std::vector< ObjFace > m_faces; // Utility-vector holding faces read form file to save on memory.

	std::vector< Vertex > m_vertices;
	std::vector< unsigned > m_indices;
};

#endif // DV2520_RDROBJ_H

// I have birthed the devil!