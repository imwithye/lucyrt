// Copyright 2019
#include "Model.h"

#include <assimp/postprocess.h>  //NOLINT
#include <assimp/scene.h>
#include <sqlite3.h>
#include <stdlib.h>

#include <filesystem>
#include <fstream>  //NOLINT
#include <iostream>

#include <assimp/Importer.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <nlohmann/json.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "TransformationMatrix.h"
#include "resource/rs.h"

using lucyrt::graphic::Context;
using lucyrt::graphic::Mesh;
using lucyrt::graphic::MeshPtr;
using lucyrt::graphic::Model;
using lucyrt::graphic::ModelPtr;
using lucyrt::graphic::Shader;
using lucyrt::graphic::ShaderPtr;
using lucyrt::graphic::Texture;
using lucyrt::graphic::TexturePtr;
using lucyrt::graphic::TransformationMatrix;
using lucyrt::graphic::Vertex;

static std::vector<TexturePtr> AssimpLoadMaterialTextures(
    aiMaterial *mat, aiTextureType type, const std::string &directory) {
  std::vector<TexturePtr> textures;
  std::filesystem::path dirpath(directory);
  dirpath = std::filesystem::canonical(dirpath);
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    std::string filepath(str.C_Str());
    filepath = (dirpath / filepath).string();
    TexturePtr tex = Texture::LoadFromFile(filepath);
    textures.push_back(tex);
  }
  return textures;
}

static void AssimpProcessMesh(ModelPtr m, aiMesh *aMesh, const aiScene *aScene,
                              const std::string &directory) {
  const MeshPtr mesh = Mesh::New(aMesh->mName.C_Str());

  std::vector<Vertex> vertices;
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
    vertices.push_back(vertex);
  }
  mesh->SetVertices(vertices);

  std::vector<GLuint> indices;
  for (size_t i = 0; i < aMesh->mNumFaces; i++) {
    aiFace face = aMesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  mesh->SetSubmeshCount(1);
  mesh->SetIndices(0, indices);

  aiMaterial *aMaterial = aScene->mMaterials[aMesh->mMaterialIndex];
  aiColor3D aColor(0.f, 0.f, 0.f);
  aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aColor);
  std::vector<ShaderPtr> shaders(1);
  shaders[0] = Shader::Compile(
      "shader", reinterpret_cast<const char *>(Shaders_blinn_phong_vert),
      reinterpret_cast<const char *>(Shaders_blinn_phong_frag));
  shaders[0]->diffuse = glm::vec4(aColor.r, aColor.g, aColor.b, 1.0f);
  std::vector<TexturePtr> diffuse_maps =
      AssimpLoadMaterialTextures(aMaterial, aiTextureType_DIFFUSE, directory);
  if (diffuse_maps.size() > 0) {
    shaders[0]->diffuse_texture = diffuse_maps[0];
  }
  mesh->SetShaders(shaders);
  TransformationMatrix mat;
  mesh->transforms.push_back(mat);

  m->meshes.push_back(mesh);
}

static void AssimpProcessNode(ModelPtr m, aiNode *aNode, const aiScene *aScene,
                              const std::string &directory) {
  for (unsigned int i = 0; i < aNode->mNumMeshes; i++) {
    aiMesh *mesh = aScene->mMeshes[aNode->mMeshes[i]];
    AssimpProcessMesh(m, mesh, aScene, directory);
  }
  for (unsigned int i = 0; i < aNode->mNumChildren; i++) {
    AssimpProcessNode(m, aNode->mChildren[i], aScene, directory);
  }
}

ModelPtr Model::LoadWithAssimp(const std::string &name,
                               const std::string &filepath) {
  ModelPtr ptr = std::shared_ptr<Model>(new Model(name), Delete);

  std::filesystem::path dirpath(filepath);
  dirpath = dirpath.parent_path();
  spdlog::info("{} starts loading {}, directory: {}", *ptr, filepath,
               dirpath.string());

  Assimp::Importer importer;
  const aiScene *aScene =
      importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs |
                                      aiProcess_CalcTangentSpace);
  if (!aScene || aScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !aScene->mRootNode) {
    spdlog::error("{} load from {} failed", *ptr, filepath);
    return ptr;
  }

  AssimpProcessNode(ptr, aScene->mRootNode, aScene, dirpath.string());
  spdlog::info("{} loaded from {}", *ptr, filepath);
  return ptr;
}

ModelPtr Model::LoadWithVRcollab(const std::string &name,
                                 const std::string &directory) {
  ModelPtr ptr = std::shared_ptr<Model>(new Model(name), Delete);

  std::filesystem::path dirpath(directory);
  dirpath = std::filesystem::canonical(dirpath);
  spdlog::info("{} starts loading, directory: {}", *ptr, dirpath.string());

  class GeometryReader {
   public:
    std::filesystem::path dirpath;
    std::ifstream handle;
    explicit GeometryReader(const std::filesystem::path &dirpath)
        : dirpath(dirpath),
          handle(dirpath / "geometry.vrc", std::ifstream::binary) {}
    int ReadInt() {
      int out;
      handle.read(reinterpret_cast<char *>(&out), sizeof(out));
      return out;
    }
    float ReadFloat() {
      float out;
      handle.read(reinterpret_cast<char *>(&out), sizeof(out));
      return out;
    }
    std::string ReadString() {
      int len = ReadInt();
      std::vector<char> data;
      data.resize(len + 1);
      handle.read(reinterpret_cast<char *>(data.data()), sizeof(char) * len);
      data[len] = '\0';
      return std::string(data.data());
    }
    ~GeometryReader() {
      if (handle.is_open()) {
        handle.close();
      }
    }
  };
  GeometryReader g_reader(dirpath);

  class MaterialReader {
   public:
    std::filesystem::path dirpath;
    sqlite3 *db = nullptr;

    explicit MaterialReader(const std::filesystem::path &dirpath)
        : dirpath(dirpath), db(nullptr) {
      sqlite3_open(
          reinterpret_cast<const char *>((dirpath / "data.vrc").c_str()), &db);
    }

    glm::vec4 ReadDiffuse(int id) {
      glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
      sqlite3_stmt *stmt;
      std::string query = "select StandardData from MaterialTable where Id = ";
      query += std::to_string(id);
      sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
      int step = sqlite3_step(stmt);
      if (step != SQLITE_DONE) {
        const unsigned char *data = sqlite3_column_text(stmt, 0);
        if (data != nullptr) {
          nlohmann::json json_data = nlohmann::json::parse(data);
          float r = json_data["ConceptualColor"]["r"];
          float g = json_data["ConceptualColor"]["g"];
          float b = json_data["ConceptualColor"]["b"];
          float a = json_data["Opacity"];
          diffuse = glm::vec4(r, g, b, a);
        }
      }
      sqlite3_finalize(stmt);
      return diffuse;
    }

    TexturePtr ReadTexture(int id) {
      TexturePtr ptr = nullptr;

      sqlite3_stmt *stmt;
      std::string query = "select StandardData from MaterialTable where Id = ";
      query += std::to_string(id);
      sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
      int step = sqlite3_step(stmt);
      if (step == SQLITE_DONE) {
        return nullptr;
      }
      const unsigned char *data = sqlite3_column_text(stmt, 0);
      if (data == nullptr) {
        return nullptr;
      }
      nlohmann::json json_data = nlohmann::json::parse(data);
      nlohmann::json main_tex = json_data["MainTex"];
      if (main_tex == nullptr) {
        return nullptr;
      }
      std::string texture_name = main_tex["TextureName"];
      TexturePtr texture =
          Texture::LoadFromFile((dirpath / "assets" / texture_name).string());
      sqlite3_finalize(stmt);
      return texture;
    }

    ~MaterialReader() {
      if (db) {
        sqlite3_close(db);
      }
    }
  };
  MaterialReader m_reader(dirpath);

  int number_of_meshes = g_reader.ReadInt();
  for (int m = 0; m < number_of_meshes; m++) {
    std::string name = g_reader.ReadString();  // Use mesh category as mesh name

    int number_of_submeshes = g_reader.ReadInt();
    std::vector<ShaderPtr> shaders;
    std::vector<std::vector<GLuint>> submeshes;
    for (int s = 0; s < number_of_submeshes; s++) {
      int material_id = g_reader.ReadInt();  // Material ID
      ShaderPtr shader = Shader::Compile(
          "shader", reinterpret_cast<const char *>(Shaders_blinn_phong_vert),
          reinterpret_cast<const char *>(Shaders_blinn_phong_frag));
      shader->diffuse = m_reader.ReadDiffuse(material_id);
      shader->diffuse_texture = m_reader.ReadTexture(material_id);
      shaders.push_back(shader);

      std::vector<GLuint> indices;
      int number_of_faces = g_reader.ReadInt();
      for (int face_idx = 0; face_idx < number_of_faces; face_idx++) {
        // Read Triangle Vertex Index
        int idx0 = g_reader.ReadInt();
        int idx1 = g_reader.ReadInt();
        int idx2 = g_reader.ReadInt();
        indices.push_back(idx0);
        indices.push_back(idx2);
        indices.push_back(idx1);
      }
      submeshes.push_back(indices);
    }

    int number_of_vertices = g_reader.ReadInt();
    std::vector<Vertex> vertices;
    for (int v = 0; v < number_of_vertices; v++) {
      float px = g_reader.ReadFloat(), py = g_reader.ReadFloat(),
            pz = g_reader.ReadFloat();
      float nx = g_reader.ReadFloat(), ny = g_reader.ReadFloat(),
            nz = g_reader.ReadFloat();
      float ux = g_reader.ReadFloat(), uy = g_reader.ReadFloat();
      Vertex vertex{
          glm::vec3{-px, py, pz},  // pos, flip x
          glm::vec3{-nx, ny, nz},  // normal, flip x
          glm::vec2{ux, uy},       // uv
      };
      vertices.push_back(vertex);
    }

    MeshPtr mesh = Mesh::New(name);
    mesh->SetVertices(vertices);
    mesh->SetSubmeshCount(number_of_submeshes);
    for (int i = 0; i < number_of_submeshes; i++) {
      mesh->SetIndices(i, submeshes[i]);
    }
    mesh->SetShaders(shaders);
    int number_of_instances = g_reader.ReadInt();
    for (int i = 0; i < number_of_instances; i++) {
      g_reader.ReadInt();  // Element ID
      glm::mat4 matrix;
      for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
          // flip x
          matrix[col][row] = (row * col == 0) && (row + col != 0)
                                 ? -g_reader.ReadFloat()
                                 : g_reader.ReadFloat();
        }
      }
      TransformationMatrix mat(matrix);
      mesh->transforms.push_back(mat);
    }
    if (mesh->transforms.size() > 0) {
      ptr->meshes.push_back(mesh);
    }
  }

  return ptr;
}

void Model::Delete(Model *model) { delete model; }

Model::Model(const std::string &name) : name(name) {}

Model::~Model() {}

bool Model::PrepareToGPU() {
  for (const MeshPtr mesh : meshes) {
    mesh->PrepareToGPU();
  }
  spdlog::trace("{} loaded to GPU", *this);
  return true;
}

void Model::RemoveFromGPU() {
  for (const std::shared_ptr<Mesh> mesh : meshes) {
    mesh->RemoveFromGPU();
  }
  spdlog::trace("{} removed from GPU", *this);
}

void Model::Draw(Context *ctx) {
  for (const MeshPtr mesh : meshes) {
    mesh->Draw(ctx);
  }
}
