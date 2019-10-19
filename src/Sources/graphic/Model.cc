// Copyright 2019
#include "Model.h"

#include <assimp/postprocess.h>  //NOLINT
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Mesh.h"
#include "Texture.h"

using lucyrt::graphic::Mesh;
using lucyrt::graphic::MeshPtr;
using lucyrt::graphic::Model;
using lucyrt::graphic::ModelPtr;
using lucyrt::graphic::Texture;
using lucyrt::graphic::TexturePtr;
using lucyrt::graphic::Vertex;

static std::vector<TexturePtr> AssimpLoadMaterialTextures(
    aiMaterial *mat, aiTextureType type, const std::string &typeName) {
  std::vector<TexturePtr> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    std::string filepath(str.C_Str());
    TexturePtr tex = Texture::Load("../examples/" + filepath);
    textures.push_back(tex);
  }
  return textures;
}

static void AssimpProcessMesh(ModelPtr m, aiMesh *aMesh,
                              const aiScene *aScene) {
  const MeshPtr mesh = Mesh::New(aMesh->mName.C_Str());

  for (unsigned int i = 0; i < aMesh->mNumVertices; i++) {
    Vertex vertex;
    // positions
    vertex.pos.x = aMesh->mVertices[i].x;
    vertex.pos.y = aMesh->mVertices[i].y;
    vertex.pos.z = aMesh->mVertices[i].z;
    // normals
    vertex.normal.x = aMesh->mNormals[i].x;
    vertex.normal.y = aMesh->mNormals[i].y;
    vertex.normal.z = aMesh->mNormals[i].z;
    // uvs
    if (aMesh->mTextureCoords[0]) {
      vertex.uv.x = aMesh->mTextureCoords[0][i].x;
      vertex.uv.y = aMesh->mTextureCoords[0][i].y;
    }
    mesh->vertices.push_back(vertex);
  }
  for (size_t i = 0; i < aMesh->mNumFaces; i++) {
    aiFace face = aMesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++)
      mesh->indices.push_back(face.mIndices[j]);
  }

  aiMaterial *aMaterial = aScene->mMaterials[aMesh->mMaterialIndex];
  aiColor3D aColor(0.f, 0.f, 0.f);
  aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aColor);
  mesh->shader->diffuse = glm::vec4(aColor.r, aColor.g, aColor.b, 1.0f);
  std::vector<TexturePtr> diffuse_maps = AssimpLoadMaterialTextures(
      aMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
  if (diffuse_maps.size() > 0) {
    mesh->shader->diffuse_texture = diffuse_maps[0];
  }
  m->meshes.push_back(mesh);
}

static void AssimpProcessNode(ModelPtr m, aiNode *aNode,
                              const aiScene *aScene) {
  for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
    aiMesh *mesh = aScene->mMeshes[aNode->mMeshes[i]];
    AssimpProcessMesh(m, mesh, aScene);
  }
  for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
    AssimpProcessNode(m, aNode->mChildren[i], aScene);
  }
}

ModelPtr Model::LoadWithAssimp(const std::string &name,
                               const std::string &filepath) {
  ModelPtr ptr = std::shared_ptr<Model>(new Model(name), Delete);
  Assimp::Importer importer;
  const aiScene *aScene =
      importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs |
                                      aiProcess_CalcTangentSpace);
  if (!aScene || aScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !aScene->mRootNode) {
    spdlog::error("{} load from {} failed", ptr.get(), filepath);
    return ptr;
  }
  AssimpProcessNode(ptr, aScene->mRootNode, aScene);
  spdlog::info("{} loaded from {}", ptr.get(), filepath);
  return ptr;
}

ModelPtr Model::LoadWithVRcollab(const std::string &name,
                                 const std::string &dirpath) {
  (void)dirpath;
  ModelPtr ptr = std::shared_ptr<Model>(new Model(name), Delete);
  return ptr;
}

void Model::Delete(Model *model) { delete model; }

Model::Model(const std::string &name) : name(name) {}

Model::~Model() {}

bool Model::PrepareToGPU() {
  for (const MeshPtr mesh : meshes) {
    mesh->PrepareToGPU();
  }
  spdlog::trace("{} loaded to GPU", this);
  return true;
}

void Model::RemoveFromGPU() {
  for (const std::shared_ptr<Mesh> mesh : meshes) {
    mesh->RemoveFromGPU();
  }
  spdlog::trace("{} removed from GPU", this);
}

void Model::Draw(Context *ctx) {
  for (const MeshPtr mesh : meshes) {
    mesh->Draw(ctx);
  }
}
