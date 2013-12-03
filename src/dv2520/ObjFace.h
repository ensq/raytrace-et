#ifndef DV2520_OBJFACE_H
#define DV2520_OBJFACE_H

// Describes a face read from .obj.
// Used to filter duplicate faces and create lists of indices without such duplicates.
class ObjFace {
public:
	ObjFace();
	ObjFace( unsigned p_v, unsigned p_t, unsigned p_n, unsigned p_index );
	~ObjFace();

	friend bool operator==( ObjFace& p_l, ObjFace& p_r ) {
		return ( 
			p_l.m_v==p_r.m_v &&
			p_l.m_t==p_r.m_t &&
			p_l.m_n==p_r.m_n );
	}
	friend bool operator!=( ObjFace& p_l, ObjFace& p_r ) {
		return !( p_l==p_r );
	}

	unsigned getV() const;	
	unsigned getT() const;	
	unsigned getN() const;
	unsigned getIndex() const;
	void setIndex( unsigned p_index );
protected:
private:
	unsigned m_v; // Index to Vertex.
	unsigned m_t; // Index to Texture-coordinates.
	unsigned m_n; // Index to Normal.
	
	unsigned m_index; // Index to Face.
};

#endif // DV2520_OBJFACE_H