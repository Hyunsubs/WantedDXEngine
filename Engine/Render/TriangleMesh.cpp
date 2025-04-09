#include "TriangleMesh.h"""
#include "Vertex.h"
#include "Shader/DefaultShader.h"
#include "Resource/ShaderLoader.h"

namespace Blue
{
	TriangleMesh::TriangleMesh()
	{
		// 정점 배열.
		std::vector<Vertex> vertices =
		{
			Vertex(Vector3(0.0f, 0.5f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.5f, 0.0f), Vector3(0.f, 0.f, -1.f)),
			Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector3(0.f, 0.f, -1.f)),
			Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.f, 0.f, -1.f)),
		};

		// 인덱스 배열.
		std::vector<uint32> indices = { 0, 1, 2 };
		
		meshes.emplace_back(std::make_shared<MeshData>(vertices, indices));
	}
}