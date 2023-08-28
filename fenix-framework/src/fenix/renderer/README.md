# Renderer

- Scene
  - Environment
    - Point lights
    - Directional lights
    - Cube map
  - Camera
    - Projection: Perspective or Orthogonal
    - Type: FPS camera, Fly camera, Editor camera...
  - Scene Objects
    - Object
      - Geometry (Mesh): vertices, faces, normals...
      - Material: metalic, plastic, wood...
      - Transform
        - Translation
        - Rotation
        - Scale

## End goal API

```c++
auto material = std::make_shared<Material>(m_Shader);
auto material_i = std::make_shared<MaterialInstance>(material); // For more specific data (degradation texture...)
material_i->SetValue("u_color", Color::Red);
material_i->SetTexture("u_albedo_map", texture) // How to create an albedo map with gimp?
mesh->SetMaterial(material_i);
```
