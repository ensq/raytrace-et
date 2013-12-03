#include <stdafx.h>

#include <Obj.h>
#include <RdrObj.h>

RdrObj::RdrObj( std::string p_filePath, std::string p_fileName ) {
	m_filePath = p_filePath;
	m_fileName = p_fileName;
	m_fileFull = p_filePath + p_fileName;

	m_lineCnt = 0;
}
RdrObj::~RdrObj() {
}

Obj* RdrObj::read( bool& io_sucess ) {
	Obj* obj = nullptr;

	m_ifs.open( m_fileFull );
	if( m_ifs.is_open()==true ) {
		obj = readObj();
		m_ifs.close();
	} else {
		io_sucess = false;
	}
	return obj;
}
Obj* RdrObj::readObj() {
	RdrObjSymbol curSymbol = RdrObjSymbol_INVALID;
	std::string curLine;
	std::vector< std::string > curLineSplit;
	while( !m_ifs.eof() ) {
		// Get input string.
		getLine( curLine );

		// Split input string into substrings.
		curLineSplit.clear();
		split( curLine, RDROBJ_SEPARATOR_DEFAULT, curLineSplit );

		// Retrieve symbol and perform actions based off of that.
		curSymbol = getSymbol( curLineSplit );
		loadSymbol( curSymbol, curLineSplit );
	}
	return new Obj( m_vertices.size(), m_indices.size(), &m_vertices[ 0 ], &m_indices[ 0 ] );
}
RdrObjSymbol RdrObj::getSymbol( const std::vector< std::string >& p_params ) {
	RdrObjSymbol symbol = RdrObjSymbol_IGNORE;
	if( p_params.size()>0 ) {
		if( p_params.front() == RDROBJ_INDICATOR_VERTEX ) {
			symbol = RdrObjSymbol_VERTEX;
		} else if( p_params.front()	== RDROBJ_INDICATOR_TEX ) {
			symbol = RdrObjSymbol_TEX;
		} else if( p_params.front()	== RDROBJ_INDICATOR_NORM ) {
			symbol = RdrObjSymbol_NORM;
		} else if( p_params.front()	== RDROBJ_INDICATOR_FACE ) {
			symbol = RdrObjSymbol_FACE;
		}
	}
	return symbol;
}
void RdrObj::loadSymbol( const RdrObjSymbol p_symbol, const std::vector< std::string >& p_params ) {
	switch( p_symbol ) {
	case RdrObjSymbol_VERTEX:
		loadPos( p_params );
		break;
	case RdrObjSymbol_TEX:
		loadTex( p_params );
		break;
	case RdrObjSymbol_NORM:
		loadNorm( p_params );
		break;
	case RdrObjSymbol_FACE:
		loadFaces( p_params );
		break;
	}
}
void RdrObj::loadPos( const std::vector< std::string >& p_params ) {
	float x, y, z;
	float w = 1.0f; //optional
	x = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_VERTEX_X ].c_str() );
	y = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_VERTEX_Y ].c_str() );
	z = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_VERTEX_Z ].c_str() );
	if( p_params.size() == RDROBJ_PARAMS_NUM_VERTEX_OPTIONAL ) {
		w = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_VERTEX_W ].c_str() );
	}
	m_pos.push_back( Vec3F( x, y, z ) ); // Ignore w.
}
void RdrObj::loadNorm( const std::vector< std::string >& p_params ) {
	float x, y, z;
	x = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_VERTEX_X ].c_str() );
	y = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_VERTEX_Y ].c_str() );
	z = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_VERTEX_Z ].c_str() );
	m_nor.push_back( Vec3F( x, y, z ) );
}
void RdrObj::loadTex( const std::vector< std::string >& p_params ) {
	float u, v;
	float w = 0.0f; //optional
	u = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_TEX_U ].c_str() );
	v = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_TEX_V ].c_str() );

	if( p_params.size() > RDROBJ_PARAMS_NUM_TEX) {
		w = (float)::atof( p_params[ RDROBJ_PARAMS_INDEX_TEX_W ].c_str() );
	}
	m_tex.push_back( Vec2F( u, v ) );
}
void RdrObj::loadFaces( const std::vector< std::string >& p_params ) {
	std::string face;
	std::vector< std::string > faceSplit;
	for( unsigned i = 0; i<3; i++ ) {
		face = p_params[ 1 + i ];

		faceSplit.clear();
		split( face, RDROBJ_SEPARATOR_FACE, faceSplit );

		unsigned int faceParams = RdrObjFaceParam_VERTEX_INDEX | RdrObjFaceParam_TEXCOORD_INDEX | RdrObjFaceParam_NORMAL_INDEX;
		if( faceSplit.size()<RDROBJ_PARAMS_NUM_FACE ) {
			faceParams = parseFaceParams(face);
		}
		loadFace( faceSplit, faceParams );
	}
}
unsigned RdrObj::parseFaceParams( const std::string& p_face ) {
	// I swear I must've been drunk when I gave birth to this devilry.
	unsigned int params = 0;
	unsigned int curParam = 0;
	for( unsigned i = 0; i<p_face.length() && curParam<RDROBJ_PARAMS_NUM_FACE; i++) {
		if( p_face[ i ] == RDROBJ_SEPARATOR_FACE ) {
			curParam++;
		} else {
			if(curParam==0) { //Vertex
				params |= RdrObjFaceParam_VERTEX_INDEX;
			}
			if(curParam==1) { //Texcoord
				params |= RdrObjFaceParam_TEXCOORD_INDEX;
			}
			if(curParam==2) { //Normal
				params |= RdrObjFaceParam_NORMAL_INDEX;
			}
		}
	}
	return params;
}
void RdrObj::loadFace( const std::vector<std::string>& p_face, const unsigned int p_faceParams ) {
	unsigned int iPos = 0, iTex = 0, iNorm = 0; // Default values

	unsigned int curIndex = RDROBJ_PARAMS_INDEX_FACE_X;
	if( p_faceParams & RdrObjFaceParam_VERTEX_INDEX )	{
		iPos = ::atoi( p_face[ curIndex ].c_str() );
		curIndex++;
	}
	if( p_faceParams & RdrObjFaceParam_TEXCOORD_INDEX ) {
		iTex = ::atoi( p_face[ curIndex ].c_str() );
		curIndex++;
	}
	if( p_faceParams & RdrObjFaceParam_NORMAL_INDEX ) {
		iNorm = ::atoi( p_face[ curIndex ].c_str() );
		curIndex++;
	}

	ObjFace newFace( iPos, iTex, iNorm, 0) ;
	ObjFace preFace;
	bool foundFace = false;
	for( int i = m_faces.size() - 1; i>=0; i--) {
		if( m_faces[ i ] == newFace ) {
			foundFace = true;
			preFace = m_faces[ i ];
			break;
		}
	}

	unsigned int index;
	if( foundFace ) { 
		index = preFace.getIndex();
	} else {
		index = loadVertex( iPos, iTex, iNorm );
		newFace.setIndex( index );
		m_faces.push_back( newFace );
	}
	m_indices.push_back( index );
}
unsigned RdrObj::loadVertex( unsigned p_iPos, unsigned p_iTex, unsigned p_iNorm ) {
	unsigned posIndex	= p_iPos	- 1;
	unsigned texIndex	= p_iTex	- 1;
	unsigned normIndex	= p_iNorm	- 1;

	Vec3F pos, nor;
	Vec2F tex;
	if( posIndex < m_pos.size() ) {
		pos = m_pos[ posIndex ];
	} if( texIndex < m_tex.size() ) {
		tex = m_tex[ texIndex ];
	} if( normIndex < m_nor.size() ) {
		nor = m_nor[ normIndex ];
	}

	m_vertices.push_back( Vertex( pos, nor, tex ) );
	return m_vertices.size() - 1;
}

void RdrObj::getLine( std::string& io_line ) {
	std::getline( m_ifs, io_line );
	m_lineCnt++;
}
std::vector< std::string >& RdrObj::split( const std::string& p_s, char p_d, std::vector< std::string >& io_split ) {
	std::stringstream ss( p_s );
	std::string sub;
	while( std::getline( ss, sub, p_d ) ) {
		io_split.push_back( sub );
	}
	return io_split;
}