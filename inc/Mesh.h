#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include "GL/glew.h"
#include "../glm/glm.hpp"
#include "Material.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Box.h"

class Vertex
{
public:
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t)
	{
		mPosition = p;
		mNormal = n;
		mTexCood = t;
	}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float tx, float ty)
	{
		mPosition = glm::vec3(x, y, z);
		mNormal = glm::vec3(nx, ny, nz);
		mTexCood = glm::vec2(tx, ty);
	}
	Vertex() = default;
	void SetTexCood(float s, float t)
	{
		mTexCood = glm::vec2(s, t);
	}

public:
	glm::vec3 mPosition;
	glm::vec3 mNormal;
	glm::vec2 mTexCood;
};

class Mesh
{
public:
	Mesh()
	{
		mName = "test_cube";
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
		mVertices.reserve(36);
		for (int i = 0; i < 6 * 6 * 8; i = i + 8)
		{
			mVertices.push_back(Vertex(vertices[i], vertices[i + 1], vertices[i + 2],
									   vertices[i + 3], vertices[i + 4], vertices[i + 5],
									   vertices[i + 6], vertices[i + 7]));
		}
		std::ofstream file("cube.txt");
		for (const Vertex &v : mVertices)
		{
			file << "v " << v.mPosition.x << " " << v.mPosition.y << " " << v.mPosition.z << std::endl;
		}
		for (const Vertex &v : mVertices)
		{
			file << "vn " << v.mNormal.x << " " << v.mNormal.y << " " << v.mNormal.z << std::endl;
		}
		for (const Vertex &v : mVertices)
		{
			file << "vt " << v.mTexCood.x << " " << v.mTexCood.y << std::endl;
		}
		mIndices.reserve(36);
		for (int i = 0; i < 36; ++i)
		{
			mIndices.push_back(i);
		}
		for (int i = 0; i < 12; ++i)
		{
			file << "f " << i * 3 + 1 << "/" << i * 3 + 1 << "/" << i * 3 + 1 << " "
				 << i * 3 + 2 << "/" << i * 3 + 2 << "/" << i * 3 + 2 << " "
				 << i * 3 + 3 << "/" << i * 3 + 3 << "/" << i * 3 + 3
				 << std::endl;
		}
		file.close();
		BindVertexBuffer();
	}
	Mesh(const std::string &name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<TexturePicture> &textures);
	void BindVertexBuffer();
	void BindTextureBuffer();
	void Draw(unsigned int program) const;
	std::string GetName() const { return mName; }
	void SetPath(const std::string &s) { mPath = s; }
	std::string GetPath() const { return mPath; }
	Box GetBox() const { return mBox; }
	void SetMaterial(std::shared_ptr<ADS_Material> p)
	{
		mADS_Material = p;
	}
	const Vertex *GetVerticesPointer() const
	{
		return mVertices.data();
	}
	const uint32_t *GetIndicesPointer() const
	{
		return mIndices.data();
	}
	unsigned int VerticesCount() const
	{
		return mVertices.size();
	}
	unsigned int IndicesCount() const
	{
		return mIndices.size();
	}

public:
	enum class TopoloyType
	{
		Triangles,
		TriangleFans,
		TriangleStrips,
		Quads
	};

private:
	std::string mName;
	std::string mPath;
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;
	TopoloyType mType = TopoloyType::Triangles;
	unsigned int mVAO = 0;
	unsigned int mVBO = 0;
	unsigned int mEBO = 0;
	std::shared_ptr<ADS_Material> mADS_Material;
	std::vector<TexturePicture> mTexture;
	Box mBox;
};

class Model
{
public:
	Model() {}
	void LoadModel(const std::string &path);
	void AddMesh(const std::string &name, std::unique_ptr<Mesh> &mesh)
	{
		if (mMeshLists.count(name) == 0)
		{
			mMeshLists[name] = std::move(mesh);
		}
		else
		{
			mMeshLists[name].reset(nullptr);
			mMeshLists[name] = std::move(mesh);
		}
	}
	void Draw(unsigned int program) const;
	std::string GetPath() const { return mPath; }
	Box GetBox() const { return mBox; }

private:
	void processNode(aiNode *node, const aiScene *scene);
	std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);

private:
	std::string mName;
	std::unordered_map<std::string, std::unique_ptr<Mesh>> mMeshLists;
	// std::vector <TexturePicture> mAllTexture;
	std::string mPath;
	Box mBox;
};
#endif // !MESH_H
