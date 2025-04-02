#include "QuadMesh.h"
#include "Vertex.h"
#include "../Shader/Shader.h"
#include "Shader/TextureMappingShader.h"

#include "Resource/ShaderLoader.h"
#include "Resource/ModelLoader.h"

#include "Math/Matrix4.h"
#include "Math/Vector3.h"

namespace Blue
{
	QuadMesh::QuadMesh()
	{
		// ¸ðµ¨ ·Îµå
		std::weak_ptr<MeshData> mesh;
		if (ModelLoader::Get().Load("quad.obj", mesh))
		{
			meshes.emplace_back(mesh);
		}
		
	}

	void QuadMesh::Update(float deltaTime)
	{

	}

}