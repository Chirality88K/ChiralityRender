#include "../inc/Mesh.h"
#include <iostream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "../inc/stb_image.h"
#include "../inc/Log.h"

static int name = 0;

Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<TexturePicture>& textures)
{
    mName = name;
    std::string info;
    mVertices = vertices;
    if (vertices.size() == 0)
    {
        info = "Number of vertices is ZERO!!!!!!";
        LOG_ERROR(info);
    }
    else 
    {
        info = "Load vertices of mesh, number of vertices: ";
        LOG_INFO(info + std::to_string(vertices.size()));
    }
    mIndices = indices;
    if (indices.size() == 0)
    {
        info = "Number of indices is ZERO!!!!!!";
        LOG_ERROR(info);
    }
    else 
    {
        info = "Load indices of mesh, number of indices: ";
        LOG_INFO(info + std::to_string(indices.size()));
    }
    mTexture = textures;
    if (textures.size() == 0)
    {
        info = "No textures loaded";
        LOG_INFO(info);
    }
    else
    {
        info = "Load textures \n";
        for (const auto& t : textures)
        {
            info += t.mType + " " + t.mPath + "\n";
        }
        LOG_INFO(info);
    }
}

void Mesh::BindVertexBuffer()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);
    unsigned int count_v = mVertices.size();
    unsigned int count_i = mIndices.size();
    const Vertex* vertices = mVertices.data();
    const uint32_t* indices = mIndices.data();

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * count_v, vertices, GL_STATIC_DRAW);
    LOG_INFO("Succeed to upload " + std::to_string(count_v) + " vertices");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count_i, indices, GL_STATIC_DRAW);
    LOG_INFO("Succeed to upload " + std::to_string(count_i) + " indices");
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        (void*)0);
    LOG_INFO("Succeed to bind vertices");
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        (void*)(3 * sizeof(float)));
    LOG_INFO("Succeed to bind normal vectors");
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 
        (void*)(6 * sizeof(float)));
    LOG_INFO("Succeed to bind texture coords");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::BindTextureBuffer()
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    std::string info;
    std::string real_path;
    for (auto& t : mTexture)
    {
        //std::filesystem::path currpath = std::filesystem::current_path();
        real_path = "Resources/" + t.mPath;
        unsigned char* data = stbi_load(real_path.c_str(), &width, &height, &nrChannels, 0);
        if (data == nullptr)
        {
            info = "Fail to load texture: ";
            LOG_ERROR(info + t.mPath);
            return;
        }
        glGenTextures(1, &(t.mID));
        glBindTexture(GL_TEXTURE_2D, t.mID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        info = "Load texture: " + t.mPath + "\n" + "TextureID: " + std::to_string(t.mID);
        info += " Width: " + std::to_string(width) + " Height: " + std::to_string(height);
        LOG_INFO(info);
    }
}

void Mesh::Draw(unsigned int program) const
{
    if (mADS_Material != nullptr)
    {
        mADS_Material->SetMaterial(program);
    }
    for (const auto& t : mTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t.mID);
        glUniform1i(glGetUniformLocation(program, "ourTexture"), 0);
    }
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void Model::LoadModel(const std::string& path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    std::string info;
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        info = "ASSIMP:: " + std::string(import.GetErrorString());
        LOG_ERROR(info);
        return;
    }
    mPath = path.substr(0, path.find_last_of('/') + 1);
    mName = path.substr(path.find_last_of('/') + 1);
    processNode(scene->mRootNode, scene);
    info = "Load model: " + mPath + mName;
    LOG_INFO(info);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    if (node == nullptr || scene == nullptr)
    {
        return;
    }
    std::string info;
    std::string name_of_mesh;
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::unique_ptr<Mesh> meshptr = processMesh(mesh, scene);
        name_of_mesh = meshptr->GetName();
        mMeshLists[name_of_mesh] = std::move(meshptr);
        info = "Succeed to add mesh " + name_of_mesh + " to model " + mName;
        LOG_INFO(info);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

std::vector<TexturePicture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<TexturePicture> textures;
    if (mat == nullptr)
    {
        LOG_ERROR("Fail to load texture because of nullptr!!!!!!");
        return textures;
    }
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        TexturePicture texture;
        texture.mID = 0;
        texture.mType = typeName;
        texture.mPath = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}

std::unique_ptr<Mesh> Model:: processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector <Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    std::vector <unsigned int> indices;
    std::vector <TexturePicture> textures;
    ADS_Material adsm("material0");

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z,
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z, 0, 0);
        if (mesh->mTextureCoords[0])
        {
            vertex.SetTexCood(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        vertices.push_back(vertex);
    }
    
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<TexturePicture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<TexturePicture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        aiColor4D color;

        if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS) 
        {
            adsm.mDiffuse = glm::vec4(color.r, color.g, color.b, color.a);
        }
        if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS)
        {
            adsm.mAmbient = glm::vec4(color.r, color.g, color.b, color.a);
        }
        if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS)
        {
            adsm.mSpecular = glm::vec4(color.r, color.g, color.b, color.a);
        }
        if (aiGetMaterialColor(material, AI_MATKEY_SHININESS, &color) == AI_SUCCESS)
        {
            adsm.mShininess = color.r;
        }
    }
    std::unique_ptr<Mesh> mesh_ptr = std::make_unique<Mesh>(mesh->mName.C_Str(), vertices, indices, textures);
    mesh_ptr->BindVertexBuffer();
    mesh_ptr->BindTextureBuffer();
    mesh_ptr->SetMaterial(std::make_shared<ADS_Material>(adsm));
    return mesh_ptr;
}

void Model::Draw(unsigned int program) const
{
    for (const auto& it : mMeshLists)
    {
        it.second->Draw(program);
    }
}
