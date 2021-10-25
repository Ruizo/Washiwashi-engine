#pragma once
#include "MathGeoLib.h"
#include "Color.h"

namespace Primitive 
{
	enum PrimitiveTypes
	{
		Primitive_Point,
		Primitive_Line,
		Primitive_Plane,
		Primitive_Cube,
		Primitive_Sphere,
		Primitive_Cylinder,
	};

	class Primitive
	{
	public:

		Primitive();

		virtual void	Render() const;
		virtual void	InnerRender() const;
		void			SetPos(float x, float y, float z);
		void			SetRotation(float angle, const float3& u);
		void			Scale(float x, float y, float z);
		PrimitiveTypes	GetType() const;

	public:

		Color color;
		float4x4 transform;
		bool axis, wire;
		uint id;
		uint index;

	protected:
		PrimitiveTypes type;
	};

	// ============================================
	class Cube : public Primitive
	{
	public:
		Cube();
		Cube(float sizeX, float sizeY, float sizeZ);
		void InnerRender() const;
	public:
		float3 size;
	};

	// ============================================
	class Sphere : public Primitive
	{
	public:
		Sphere();
		Sphere(float radius, unsigned int rings, unsigned int sectors, bool smooth);
		void InnerRender() const;

		void buildVerticesSmooth();
		void buildVerticesFlat();
		void buildInterleavedVertices();
		void clearArrays();
		void addVertex(float x, float y, float z);
		void addNormal(float x, float y, float z);
		void addTexCoord(float s, float t);
		void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
		std::vector<float> computeFaceNormal(float x1, float y1, float z1,
			float x2, float y2, float z2,
			float x3, float y3, float z3);
	public:
		float radius;
		int sectorCount;                        // longitude, # of slices
		int stackCount;                         // latitude, # of stacks
		bool smooth;
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texCoords;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> lineIndices;

		// interleaved
		std::vector<float> interleavedVertices;
		int interleavedStride = 32;

	};

	// ============================================
	class Cylinder : public Primitive
	{
	public:
		Cylinder();
		Cylinder(float baseRadius, float topRadius, float height,
			int sectorCount, int stackCount, bool smooth);
		void InnerRender() const;
	public:
		void clearArrays();
		void buildVerticesSmooth();
		void buildVerticesFlat();
		void buildInterleavedVertices();
		void buildUnitCircleVertices();
		void addVertex(float x, float y, float z);
		void addNormal(float x, float y, float z);
		void addTexCoord(float s, float t);
		void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
		std::vector<float> getSideNormals();
		std::vector<float> computeFaceNormal(float x1, float y1, float z1,
			float x2, float y2, float z2,
			float x3, float y3, float z3);

		// memeber vars
		float baseRadius;
		float topRadius;
		float height;
		int sectorCount;                        // # of slices
		int stackCount;                         // # of stacks
		unsigned int baseIndex;                 // starting index of base
		unsigned int topIndex;                  // starting index of top
		bool smooth;
		std::vector<float> unitCircleVertices;
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texCoords;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> lineIndices;

		// interleaved
		std::vector<float> interleavedVertices;
		int interleavedStride = 32;
	};

	// ============================================
	class Pyramid : public Primitive
	{
	public:
		Pyramid();
		Pyramid(float height, float base);
		void InnerRender() const;
	public:
		float height;
		float base;
	};

	// ============================================
	class Line : public Primitive
	{
	public:
		Line();
		Line(float x, float y, float z);
		void InnerRender() const;
	public:
		float3 origin;
		float3 destination;
	};

	// ============================================
	class Plane : public Primitive
	{
	public:
		Plane();
		Plane(float x, float y, float z, float d);
		void InnerRender() const;
	public:
		float3 normal;
		float constant;
	};
}