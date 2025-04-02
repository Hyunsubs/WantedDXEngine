#include "Core/Engine.h"
#include "Level/DemoLevel.h"
#include <iostream>

//#include "Math/Vector2.h"

using namespace Blue;

#include "Core/Common.h"
#include <typeinfo>
#include "Shader/Shader.h"
#include "Shader/TextureMappingShader.h"

// Todo List
// Input Controller (Window Message Process)
// Actor(Entity/GameObject) / Component / Scene(Level)
// FBX Loader (Assimp / FBX SDK -> Submesh)
// Camera (View / Projection(Perspective/Orthographic))
// Screen Coordinate System(NDC - Normalized Device Coordinates)
// Shade (Light / Brightness / Darkness)
// Ambient(Static) / Diffuse(Lambert .../Roughness) / Specular (Highlight, Phong / Bliin-Phong / Metalic)
// Render Target(Draw to texture)
// Shadow Mapping (Shadow Map)
// Normal Mapping
// Environment Mapping (Skybox, SkySphere)
// Skeletal Mesh (Skinned Mesh) - Skeleton
// Tone Mapping(For Extra Study)
// Study (Physics)

// IA -> VS -> (Optional) HS -> GS -> RS -> PS -> OM
// VS (정점 변환 Object->World->View->Projection)
// PS (조명 처리) PBR / Diffuse / Specular
// 엔진 개발자 (렌더러 개발자)

// Window Render Without WinMain
int main()
{
	Engine engine(1280, 800, TEXT("Engine Demo"), GetModuleHandle(nullptr));
	engine.SetLevel(std::make_shared<DemoLevel>());
	engine.Run();
}