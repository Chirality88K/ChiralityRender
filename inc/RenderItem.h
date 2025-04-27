#ifndef RENDERITEM_H
#define RENDERITEM_H

#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include <memory>
#include <unordered_map>
#include <vector>

class RenderItem {
public:
  RenderItem(std::shared_ptr<Model> model, const glm::mat4x4& matrix);
  //void LinkShader(unsigned int, const std::vector <Shader>&)const;
  void Draw(unsigned int programID)const;
  void UpdateRenderItem(glm::mat4x4 matrix)
  {
	  mModelMatrix = matrix;
  }
  glm::mat4x4 GetNowModelMatrix() const
  {
	  return mModelMatrix;
  }

private:
  std::shared_ptr <Model> mModel;
  glm::mat4x4 mModelMatrix;
};

#endif