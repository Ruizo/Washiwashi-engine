#include "Globals.h"
#include "Primitive.h"
#include "glew.h"

#include <vector>
#include <gl/GL.h>
#include <gl/GLU.h>
// ------------------------------------------------------------
namespace Primitive
{
	Primitive::Primitive() : transform(float4x4::identity), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point), id(0), index(0)
	{}

	// ------------------------------------------------------------
	PrimitiveTypes Primitive::GetType() const
	{
		return type;
	}

	// ------------------------------------------------------------
	void Primitive::Render() const
	{
		glPushMatrix();
		glMultMatrixf(*transform.v);

		if (axis == true)
		{
			glLineWidth(2.0f);

			glBegin(GL_LINES);

			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

			glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
			glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

			glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
			glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
			glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
			glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

			glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
			glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
			glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
			glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

			glEnd();

			glLineWidth(1.0f);
		}

		glColor3f(color.r, color.g, color.b);

		if (wire)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		InnerRender();

		glPopMatrix();
	}

	// ------------------------------------------------------------
	void Primitive::InnerRender() const
	{
		glPointSize(5.0f);

		glBegin(GL_POINTS);

		glVertex3f(0.0f, 0.0f, 0.0f);

		glEnd();

		glPointSize(1.0f);
	}

	// ------------------------------------------------------------
	void Primitive::SetPos(float x, float y, float z)
	{
		transform.Translate(x, y, z);
	}

	// ------------------------------------------------------------
	void Primitive::SetRotation(float angle, const float3& u)
	{
		transform.RotateAxisAngle(u, angle);
	}

	// ------------------------------------------------------------
	void Primitive::Scale(float x, float y, float z)
	{
		transform.Scale(x, y, z);
	}

	// ============================================
	Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
	{
		type = PrimitiveTypes::Primitive_Cube;
	}

	Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
	{
		type = PrimitiveTypes::Primitive_Cube;
	}

	void Cube::InnerRender() const
	{
		glGenBuffers(1, (GLuint*)&(id));
		glBindBuffer(GL_ARRAY_BUFFER, id);

		float sx = size.x * 0.5f;
		float sy = size.y * 0.5f;
		float sz = size.z * 0.5f;

		std::vector<float> vertices =
		{
			sx,sy,sz,
			sx,sy,-sz,
			sx,-sy,sz,
			sx,-sy,-sz,
			-sx,sy,sz,
			-sx,sy,-sz,
			-sx,-sy,sz,
			-sx,-sy,-sz,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(index));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		std::vector<uint> indices =
		{
			2,1,0,
			2,3,1,
			4,5,6,
			5,7,6,
			6,3,2,
			6,7,3,
			0,5,4,
			0,1,5,
			3,5,1,
			3,7,5,
			6,0,4,
			6,2,0,
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size() * 3, &indices[0], GL_STATIC_DRAW);

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		// ... bind and use other buffers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	// SPHERE ============================================
	Sphere::Sphere() : Primitive(), radius(1.0f), stackCount(2), sectorCount(3), smooth(false)
	{
		type = PrimitiveTypes::Primitive_Sphere;
		if (smooth)
			buildVerticesSmooth();
		else
			buildVerticesFlat();
	}

	Sphere::Sphere(float radius, unsigned int rings, unsigned int sectors, bool smooth) : Primitive(), radius(radius), stackCount(rings), sectorCount(sectors), smooth(smooth)
	{
		type = PrimitiveTypes::Primitive_Sphere;
		if (smooth)
			buildVerticesSmooth();
		else
			buildVerticesFlat();
	}

	void Sphere::clearArrays()
	{
		std::vector<float>().swap(vertices);
		std::vector<float>().swap(normals);
		std::vector<float>().swap(texCoords);
		std::vector<unsigned int>().swap(indices);
		std::vector<unsigned int>().swap(lineIndices);
	}

	void Sphere::buildVerticesSmooth()
	{
		const float PI = acos(-1);

		clearArrays();

		float x, y, z, xy;
		float nx, ny, nz, lengthInv = 1.0f / radius;
		float s, t;

		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = PI / 2 - i * stackStep;
			xy = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);

			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;

				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);
				addVertex(x, y, z);

				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				addNormal(nx, ny, nz);

				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				addTexCoord(s, t);
			}
		}

		unsigned int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);
			k2 = k1 + sectorCount + 1;

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				if (i != 0)
				{
					addIndices(k1, k2, k1 + 1);
				}

				if (i != (stackCount - 1))
				{
					addIndices(k1 + 1, k2, k2 + 1);
				}

				lineIndices.push_back(k1);
				lineIndices.push_back(k2);
				if (i != 0)
				{
					lineIndices.push_back(k1);
					lineIndices.push_back(k1 + 1);
				}
			}
		}

		buildInterleavedVertices();
	}

	void Sphere::buildVerticesFlat()
	{
		const float PI = acos(-1);

		struct Vertex
		{
			float x, y, z, s, t;
		};
		std::vector<Vertex> tmpVertices;

		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = PI / 2 - i * stackStep;
			float xy = radius * cosf(stackAngle);
			float z = radius * sinf(stackAngle);

			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;

				Vertex vertex;
				vertex.x = xy * cosf(sectorAngle);
				vertex.y = xy * sinf(sectorAngle);
				vertex.z = z;
				vertex.s = (float)j / sectorCount;
				vertex.t = (float)i / stackCount;
				tmpVertices.push_back(vertex);
			}
		}

		clearArrays();

		Vertex v1, v2, v3, v4;
		std::vector<float> n;

		int i, j, k, vi1, vi2;
		int index = 0;
		for (i = 0; i < stackCount; ++i)
		{
			vi1 = i * (sectorCount + 1);
			vi2 = (i + 1) * (sectorCount + 1);

			for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
			{
				v1 = tmpVertices[vi1];
				v2 = tmpVertices[vi2];
				v3 = tmpVertices[vi1 + 1];
				v4 = tmpVertices[vi2 + 1];

				if (i == 0)
				{
					addVertex(v1.x, v1.y, v1.z);
					addVertex(v2.x, v2.y, v2.z);
					addVertex(v4.x, v4.y, v4.z);

					addTexCoord(v1.s, v1.t);
					addTexCoord(v2.s, v2.t);
					addTexCoord(v4.s, v4.t);

					n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
					for (k = 0; k < 3; ++k)
					{
						addNormal(n[0], n[1], n[2]);
					}

					addIndices(index, index + 1, index + 2);

					lineIndices.push_back(index);
					lineIndices.push_back(index + 1);

					index += 3;
				}
				else if (i == (stackCount - 1))
				{
					addVertex(v1.x, v1.y, v1.z);
					addVertex(v2.x, v2.y, v2.z);
					addVertex(v3.x, v3.y, v3.z);

					addTexCoord(v1.s, v1.t);
					addTexCoord(v2.s, v2.t);
					addTexCoord(v3.s, v3.t);

					n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
					for (k = 0; k < 3; ++k)
					{
						addNormal(n[0], n[1], n[2]);
					}

					addIndices(index, index + 1, index + 2);

					lineIndices.push_back(index);
					lineIndices.push_back(index + 1);
					lineIndices.push_back(index);
					lineIndices.push_back(index + 2);

					index += 3;
				}
				else
				{
					addVertex(v1.x, v1.y, v1.z);
					addVertex(v2.x, v2.y, v2.z);
					addVertex(v3.x, v3.y, v3.z);
					addVertex(v4.x, v4.y, v4.z);

					addTexCoord(v1.s, v1.t);
					addTexCoord(v2.s, v2.t);
					addTexCoord(v3.s, v3.t);
					addTexCoord(v4.s, v4.t);

					n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
					for (k = 0; k < 4; ++k)
					{
						addNormal(n[0], n[1], n[2]);
					}

					addIndices(index, index + 1, index + 2);
					addIndices(index + 2, index + 1, index + 3);

					lineIndices.push_back(index);
					lineIndices.push_back(index + 1);
					lineIndices.push_back(index);
					lineIndices.push_back(index + 2);

					index += 4;
				}
			}
		}
		buildInterleavedVertices();
	}

	void Sphere::buildInterleavedVertices()
	{
		std::vector<float>().swap(interleavedVertices);

		std::size_t i, j;
		std::size_t count = vertices.size();
		for (i = 0, j = 0; i < count; i += 3, j += 2)
		{
			interleavedVertices.push_back(vertices[i]);
			interleavedVertices.push_back(vertices[i + 1]);
			interleavedVertices.push_back(vertices[i + 2]);

			interleavedVertices.push_back(normals[i]);
			interleavedVertices.push_back(normals[i + 1]);
			interleavedVertices.push_back(normals[i + 2]);

			interleavedVertices.push_back(texCoords[j]);
			interleavedVertices.push_back(texCoords[j + 1]);
		}
	}

	void Sphere::addVertex(float x, float y, float z)
	{
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}

	void Sphere::addNormal(float nx, float ny, float nz)
	{
		normals.push_back(nx);
		normals.push_back(ny);
		normals.push_back(nz);
	}

	void Sphere::addTexCoord(float s, float t)
	{
		texCoords.push_back(s);
		texCoords.push_back(t);
	}

	void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
	{
		indices.push_back(i1);
		indices.push_back(i2);
		indices.push_back(i3);
	}

	std::vector<float> Sphere::computeFaceNormal(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3)
	{
		const float EPSILON = 0.000001f;

		std::vector<float> normal(3, 0.0f);
		float nx, ny, nz;

		float ex1 = x2 - x1;
		float ey1 = y2 - y1;
		float ez1 = z2 - z1;
		float ex2 = x3 - x1;
		float ey2 = y3 - y1;
		float ez2 = z3 - z1;

		nx = ey1 * ez2 - ez1 * ey2;
		ny = ez1 * ex2 - ex1 * ez2;
		nz = ex1 * ey2 - ey1 * ex2;

		float length = sqrtf(nx * nx + ny * ny + nz * nz);
		if (length > EPSILON)
		{
			float lengthInv = 1.0f / length;
			normal[0] = nx * lengthInv;
			normal[1] = ny * lengthInv;
			normal[2] = nz * lengthInv;
		}

		return normal;
	}

	void Sphere::InnerRender() const
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0]);
		glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[3]);
		glTexCoordPointer(2, GL_FLOAT, interleavedStride, &interleavedVertices[6]);

		glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, indices.data());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	// CYLINDER ============================================
	Cylinder::Cylinder() : Primitive(), baseRadius(1.0f), topRadius(1.0f), height(1.0f), sectorCount(36), stackCount(1), smooth(true)
	{
		type = PrimitiveTypes::Primitive_Cylinder;
		buildUnitCircleVertices();
		if (smooth)
			buildVerticesSmooth();
		else
			buildVerticesFlat();
	}

	Cylinder::Cylinder(float baseRadius, float topRadius, float height,
		int sectorCount, int stackCount, bool smooth)
		: Primitive(), baseRadius(baseRadius), topRadius(topRadius), height(height), sectorCount(sectorCount), stackCount(stackCount), smooth(smooth)
	{
		type = PrimitiveTypes::Primitive_Cylinder;
		buildUnitCircleVertices();
		if (smooth)
			buildVerticesSmooth();
		else
			buildVerticesFlat();
	}

	void Cylinder::clearArrays()
	{
		std::vector<float>().swap(vertices);
		std::vector<float>().swap(normals);
		std::vector<float>().swap(texCoords);
		std::vector<unsigned int>().swap(indices);
		std::vector<unsigned int>().swap(lineIndices);
	}

	void Cylinder::buildVerticesSmooth()
	{
		clearArrays();

		float x, y, z;
		float radius;

		std::vector<float> sideNormals = getSideNormals();

		for (int i = 0; i <= stackCount; ++i)
		{
			z = -(height * 0.5f) + (float)i / stackCount * height;
			radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);
			float t = 1.0f - (float)i / stackCount;

			for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
			{
				x = unitCircleVertices[k];
				y = unitCircleVertices[k + 1];
				addVertex(x * radius, y * radius, z);
				addNormal(sideNormals[k], sideNormals[k + 1], sideNormals[k + 2]);
				addTexCoord((float)j / sectorCount, t);
			}
		}

		unsigned int baseVertexIndex = (unsigned int)vertices.size() / 3;

		z = -height * 0.5f;
		addVertex(0, 0, z);
		addNormal(0, 0, -1);
		addTexCoord(0.5f, 0.5f);
		for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
		{
			x = unitCircleVertices[j];
			y = unitCircleVertices[j + 1];
			addVertex(x * baseRadius, y * baseRadius, z);
			addNormal(0, 0, -1);
			addTexCoord(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
		}

		unsigned int topVertexIndex = (unsigned int)vertices.size() / 3;

		z = height * 0.5f;
		addVertex(0, 0, z);
		addNormal(0, 0, 1);
		addTexCoord(0.5f, 0.5f);
		for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
		{
			x = unitCircleVertices[j];
			y = unitCircleVertices[j + 1];
			addVertex(x * topRadius, y * topRadius, z);
			addNormal(0, 0, 1);
			addTexCoord(x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
		}

		unsigned int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);
			k2 = k1 + sectorCount + 1;

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				addIndices(k1, k1 + 1, k2);
				addIndices(k2, k1 + 1, k2 + 1);

				lineIndices.push_back(k1);
				lineIndices.push_back(k2);
				lineIndices.push_back(k2);
				lineIndices.push_back(k2 + 1);
				if (i == 0)
				{
					lineIndices.push_back(k1);
					lineIndices.push_back(k1 + 1);
				}
			}
		}

		baseIndex = (unsigned int)indices.size();

		for (int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
		{
			if (i < (sectorCount - 1))
				addIndices(baseVertexIndex, k + 1, k);
			else
				addIndices(baseVertexIndex, baseVertexIndex + 1, k);
		}

		topIndex = (unsigned int)indices.size();

		for (int i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
		{
			if (i < (sectorCount - 1))
				addIndices(topVertexIndex, k, k + 1);
			else
				addIndices(topVertexIndex, k, topVertexIndex + 1);
		}

		buildInterleavedVertices();
	}

	void Cylinder::buildVerticesFlat()
	{
		struct Vertex
		{
			float x, y, z, s, t;
		};
		std::vector<Vertex> tmpVertices;

		int i, j, k;
		float x, y, z, s, t, radius;

		for (i = 0; i <= stackCount; ++i)
		{
			z = -(height * 0.5f) + (float)i / stackCount * height;
			radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);
			t = 1.0f - (float)i / stackCount;

			for (j = 0, k = 0; j <= sectorCount; ++j, k += 3)
			{
				x = unitCircleVertices[k];
				y = unitCircleVertices[k + 1];
				s = (float)j / sectorCount;

				Vertex vertex;
				vertex.x = x * radius;
				vertex.y = y * radius;
				vertex.z = z;
				vertex.s = s;
				vertex.t = t;
				tmpVertices.push_back(vertex);
			}
		}

		clearArrays();

		Vertex v1, v2, v3, v4;
		std::vector<float> n;
		int vi1, vi2;
		int index = 0;

		for (i = 0; i < stackCount; ++i)
		{
			vi1 = i * (sectorCount + 1);
			vi2 = (i + 1) * (sectorCount + 1);

			for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
			{
				v1 = tmpVertices[vi1];
				v2 = tmpVertices[vi2];
				v3 = tmpVertices[vi1 + 1];
				v4 = tmpVertices[vi2 + 1];

				n = computeFaceNormal(v1.x, v1.y, v1.z, v3.x, v3.y, v3.z, v2.x, v2.y, v2.z);

				addVertex(v1.x, v1.y, v1.z);
				addVertex(v2.x, v2.y, v2.z);
				addVertex(v3.x, v3.y, v3.z);
				addVertex(v4.x, v4.y, v4.z);

				addTexCoord(v1.s, v1.t);
				addTexCoord(v2.s, v2.t);
				addTexCoord(v3.s, v3.t);
				addTexCoord(v4.s, v4.t);

				for (k = 0; k < 4; ++k)
				{
					addNormal(n[0], n[1], n[2]);
				}

				addIndices(index, index + 2, index + 1);
				addIndices(index + 1, index + 2, index + 3);

				lineIndices.push_back(index);
				lineIndices.push_back(index + 1);
				lineIndices.push_back(index + 1);
				lineIndices.push_back(index + 3);
				if (i == 0)
				{
					lineIndices.push_back(index);
					lineIndices.push_back(index + 2);
				}

				index += 4;
			}
		}

		baseIndex = (unsigned int)indices.size();
		unsigned int baseVertexIndex = (unsigned int)vertices.size() / 3;

		z = -height * 0.5f;
		addVertex(0, 0, z);
		addNormal(0, 0, -1);
		addTexCoord(0.5f, 0.5f);
		for (i = 0, j = 0; i < sectorCount; ++i, j += 3)
		{
			x = unitCircleVertices[j];
			y = unitCircleVertices[j + 1];
			addVertex(x * baseRadius, y * baseRadius, z);
			addNormal(0, 0, -1);
			addTexCoord(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
		}

		for (i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
		{
			if (i < sectorCount - 1)
				addIndices(baseVertexIndex, k + 1, k);
			else
				addIndices(baseVertexIndex, baseVertexIndex + 1, k);
		}

		topIndex = (unsigned int)indices.size();
		unsigned int topVertexIndex = (unsigned int)vertices.size() / 3;

		z = height * 0.5f;
		addVertex(0, 0, z);
		addNormal(0, 0, 1);
		addTexCoord(0.5f, 0.5f);
		for (i = 0, j = 0; i < sectorCount; ++i, j += 3)
		{
			x = unitCircleVertices[j];
			y = unitCircleVertices[j + 1];
			addVertex(x * topRadius, y * topRadius, z);
			addNormal(0, 0, 1);
			addTexCoord(x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
		}

		for (i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
		{
			if (i < sectorCount - 1)
				addIndices(topVertexIndex, k, k + 1);
			else
				addIndices(topVertexIndex, k, topVertexIndex + 1);
		}
		buildInterleavedVertices();
	}

	void Cylinder::buildInterleavedVertices()
	{
		std::vector<float>().swap(interleavedVertices);

		std::size_t i, j;
		std::size_t count = vertices.size();
		for (i = 0, j = 0; i < count; i += 3, j += 2)
		{
			interleavedVertices.insert(interleavedVertices.end(), &vertices[i], &vertices[i] + 3);

			interleavedVertices.insert(interleavedVertices.end(), &normals[i], &normals[i] + 3);

			interleavedVertices.insert(interleavedVertices.end(), &texCoords[j], &texCoords[j] + 2);
		}
	}

	void Cylinder::buildUnitCircleVertices()
	{
		const float PI = acos(-1);
		float sectorStep = 2 * PI / sectorCount;
		float sectorAngle;

		std::vector<float>().swap(unitCircleVertices);
		for (int i = 0; i <= sectorCount; ++i)
		{
			sectorAngle = i * sectorStep;
			unitCircleVertices.push_back(cos(sectorAngle));
			unitCircleVertices.push_back(sin(sectorAngle));
			unitCircleVertices.push_back(0);
		}
	}

	void Cylinder::addVertex(float x, float y, float z)
	{
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}

	void Cylinder::addNormal(float nx, float ny, float nz)
	{
		normals.push_back(nx);
		normals.push_back(ny);
		normals.push_back(nz);
	}

	void Cylinder::addTexCoord(float s, float t)
	{
		texCoords.push_back(s);
		texCoords.push_back(t);
	}

	void Cylinder::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
	{
		indices.push_back(i1);
		indices.push_back(i2);
		indices.push_back(i3);
	}

	std::vector<float> Cylinder::getSideNormals()
	{
		const float PI = acos(-1);
		float sectorStep = 2 * PI / sectorCount;
		float sectorAngle;

		float zAngle = atan2(baseRadius - topRadius, height);
		float x0 = cos(zAngle);
		float y0 = 0;
		float z0 = sin(zAngle);

		std::vector<float> normals;
		for (int i = 0; i <= sectorCount; ++i)
		{
			sectorAngle = i * sectorStep;
			normals.push_back(cos(sectorAngle) * x0 - sin(sectorAngle) * y0);   // nx
			normals.push_back(sin(sectorAngle) * x0 + cos(sectorAngle) * y0);   // ny
			normals.push_back(z0);  // nz
		}

		return normals;
	}

	std::vector<float> Cylinder::computeFaceNormal(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3)
	{
		const float EPSILON = 0.000001f;

		std::vector<float> normal(3, 0.0f);
		float nx, ny, nz;

		float ex1 = x2 - x1;
		float ey1 = y2 - y1;
		float ez1 = z2 - z1;
		float ex2 = x3 - x1;
		float ey2 = y3 - y1;
		float ez2 = z3 - z1;

		nx = ey1 * ez2 - ez1 * ey2;
		ny = ez1 * ex2 - ex1 * ez2;
		nz = ex1 * ey2 - ey1 * ex2;

		float length = sqrtf(nx * nx + ny * ny + nz * nz);
		if (length > EPSILON)
		{
			float lengthInv = 1.0f / length;
			normal[0] = nx * lengthInv;
			normal[1] = ny * lengthInv;
			normal[2] = nz * lengthInv;
		}

		return normal;
	}


	void Cylinder::InnerRender() const
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0]);
		glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[3]);
		glTexCoordPointer(2, GL_FLOAT, interleavedStride, &interleavedVertices[6]);

		glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, indices.data());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	// PYRAMID ==================================================
	Pyramid::Pyramid() : Primitive(), height(1), base(1)
	{
		type = PrimitiveTypes::Primitive_Line;
	}

	Pyramid::Pyramid(float height, float base) : Primitive(), height(height), base(base)
	{
		type = PrimitiveTypes::Primitive_Line;
	}

	void Pyramid::InnerRender() const
	{
		glGenBuffers(1, (GLuint*)&(id));
		glBindBuffer(GL_ARRAY_BUFFER, id);

		float half = base / 2;

		std::vector<float> vertices =
		{
			-half,0,-half,
			half,0,-half,
			half,0,half,
			-half,0,half,
			0,height,0,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(index));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		std::vector<uint> indices =
		{
			0,1,2,
			2,3,0,
			4,1,0,
			4,2,1,
			4,3,2,
			4,0,3,
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size() * 3, &indices[0], GL_STATIC_DRAW);

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	// LINE ==================================================
	Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
	{
		type = PrimitiveTypes::Primitive_Line;
	}

	Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
	{
		type = PrimitiveTypes::Primitive_Line;
	}

	void Line::InnerRender() const
	{
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(destination.x, destination.y, destination.z);

		glEnd();

		glLineWidth(1.0f);
	}

	// PLANE ==================================================
	Plane::Plane() : Primitive(), normal(0, 1, 0), constant(1)
	{
		type = PrimitiveTypes::Primitive_Plane;
	}

	Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
	{
		type = PrimitiveTypes::Primitive_Plane;
	}

	void Plane::InnerRender() const
	{
		glLineWidth(1.0f);

		glBegin(GL_LINES);

		float d = 200.0f;

		for (float i = -d; i <= d; i += 1.0f)
		{
			glVertex3f(i, 0.0f, -d);
			glVertex3f(i, 0.0f, d);
			glVertex3f(-d, 0.0f, i);
			glVertex3f(d, 0.0f, i);
		}

		glEnd();
	}

}