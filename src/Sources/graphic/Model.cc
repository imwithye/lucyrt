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
using lucyrt::graphic::MeshRef;
using lucyrt::graphic::Model;
using lucyrt::graphic::ModelRef;
using lucyrt::graphic::Texture;
using lucyrt::graphic::Vertex;

static std::vector<std::shared_ptr<Texture>> loadMaterialTextures(
    aiMaterial *mat, aiTextureType type, const std::string &typeName) {
  std::vector<std::shared_ptr<Texture>> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    std::string filepath(str.C_Str());
    std::shared_ptr<Texture> tex =
        std::make_shared<Texture>("../examples/" + filepath);
    textures.push_back(tex);
  }
  return textures;
}

static void ProcessMesh(ModelRef m, aiMesh *aMesh, const aiScene *aScene) {
  MeshRef mesh = Mesh::New(aMesh->mName.C_Str());

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
  std::vector<std::shared_ptr<Texture>> diffuse_maps =
      loadMaterialTextures(aMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
  if (diffuse_maps.size() > 0) {
    mesh->shader->diffuse_texture = diffuse_maps[0];
  }
  m->meshes.push_back(mesh);
}

static void ProcessNode(ModelRef m, aiNode *aNode, const aiScene *aScene) {
  for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
    aiMesh *mesh = aScene->mMeshes[aNode->mMeshes[i]];
    ProcessMesh(m, mesh, aScene);
  }
  for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
    ProcessNode(m, aNode->mChildren[i], aScene);
  }
}

ModelRef Model::New(const std::string &name, const std::string &filepath) {
  (void)filepath;
  ModelRef ref(new Model(name));

  Assimp::Importer importer;
  const aiScene *aScene =
      importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs |
                                      aiProcess_CalcTangentSpace);
  if (!aScene || aScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !aScene->mRootNode) {
    spdlog::error("Model '{}' load from {} failed", name, filepath);
    return ref;
  }
  ProcessNode(ref, aScene->mRootNode, aScene);
  spdlog::info("Model '{}(meshes: {})' loaded from {}", name,
               ref->meshes.size(), filepath);
  return ref;
}

bool Model::Initialize() {
  for (MeshRef mesh : meshes) {
    mesh->Initialize();
  }
  spdlog::trace("Model '{}(meshes: {})' initialized", name_, meshes.size());
  return true;
}

void Model::Delete() {
  for (MeshRef mesh : meshes) {
    mesh->Delete();
  }
  spdlog::trace("Model '{}(meshes: {})' deleted", name_, meshes.size());
}

void Model::Draw() {
  for (MeshRef mesh : meshes) {
    mesh->Draw();
  }
}

Model::~Model() {}

Model::Model(const std::string &name) : name_(name) {}
