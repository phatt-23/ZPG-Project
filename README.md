# ZPG Project

To build and run the project:

```sh
mkdir build
cd build
cmake ..
cmake --build . --target=run -j10
```

To run from the root directory:

```sh
mkdir build
pushd build; cmake --build . --target=run -j10; popd
```

# Deferred Shading notes

- u_gPosMap seems to be working correctly
- writing to u_gNormalMap values in range [-1,1] will map them into the range [0,1]

```
decode v :: 2.0 * v - 1.0
encode v :: 0.5 * v - 0.5

encode 1 == 0.5 * 1 - 0.5 == 0
encode 0 = 0.5 * 0 - .5 == -0.5
```

# New Renderer Design

```
enum class Mobility : u32
{
    Static,
    Dynamic,
}

// dohromady, jinak by mesh drzel jenom vao
class Mesh
{
    VertexArray* vao;
    Material* mat;
}

class RenderBatches
{
    umap<BatchKey, BatchInstance> batches;

    void clear()
    {
        batches.clear();
    }
}

class Renderer
{
    void ClearStaticBatches()
    {
        static_batches.clear();
    }

    RenderBatches static_batches;
    RenderBatches dynamic_batches;
}

class Material
{
    implementation std::hash();
}

class BatchKey
{
    VertexArray* vao;
    Material* material;

    u64 BatchKey(VertexArray vao, Material material)
    {
        this.vao = vao;
        this.material = material;
        return hash(vao) + hash(material);
    }
}

class BatchInstance
{
    int[] entityIDs;
    m4[] models;

    void addInstance(int entityID, m4 model)
    {
        entityIDs.push_back(entityID);
        models.push_back(model);
    }
}

// The time complexity is O(1).
// There is no need to sort after.
// There is small cost of creating a new batch instance in case of
Renderer::Submit(Mobility mobility, int entityID, const Mesh& mesh, const m4& transform)
{
    BatchKey batch_key{mesh.GetVertexArray(), mesh.GetMaterial()};

    RenderBatches* batches = nullptr;

    switch (mobility)
    {
        case Mobility::Static:
            batches = &static_batches;
            break;
        case Mobility::Dynamic:
            batches = &dynamic_batches;
            break;
        default:
            ZPG_UNREACHABLE();
    }

    if (!batches.contains(batch_key))
    {
        batches[batch_key] = new BatchInstance();
    }

    batches[batch_key].addInstance(entityID, transform);
}

Renderer::SubmitStatic(int entityID, const Mesh& mesh, const m4& transform)
{
    Submit(Mobility::Static, entityId, mesh, transform);
}

Renderer::SubmitDynamic(int entityID, const Mesh& mesh, const m4& transform)
{
    Submit(Mobility::Dynamic, entityId, mesh, transform);
}

Renderer::Flush()
{
    RenderBatch[] batches = {
        dynamic_batches,
        static_batches
    };

    for ([batch_key, batch_instance] : batches)
    {
        VertexArray* vao = batch_key.vao;
        Material* material = batch_key.material;

        render_context.material_ssbo.setMaterial(material);

        m4[] models = batch_instance.models;
        int[] entityIDs = batch_instance.entityIDs;

        int batch_size = batch_instance.models.size();

        render_context.model_ssbo.SetModels(models);
        render_context.entity_ssbo.SetEntityIDs(entityIDs);

        vao.Bind();

        RenderCommand::DrawInstanced(vao.GetIndexBuffer().GetIndexCount(), batch_size);
    }

    dynamic_batches.clear();
}

void Scene::OnAttach()
{
    // add static and dynamic entities
    // both can be updated:
    //     - static - will lookup the meshes tagged with the entity id
    //      - dynamic - doesn't cause lookup, because the entity is submitted every frame and isn't cached
    //
    // updating static is expensive, because of the lookup O(n)
    // updating dynamic is cheap O(1) but has to be submitted every frame


    m_EntityManager().AddEntity(Mobility::Static, new Entity(...));
    m_EntityManager().AddEntity(Mobility::Dynamic, new Entity(...));
    ...
    m_EntityManager().AddEntity(Mobility::Static, new Entity(...));
    m_EntityManager().AddEntity(Mobility::Dynamic, new Entity(...));

    Renderer::ClearStaticBatches();

    for (auto& staticEntity : m_EntityManager().GetEntities(Mobility::Static))
    {
        vec<Mesh> meshes = staticEntity.GetModel().GetMehes();

        for (auto& mesh : meshes)
        {
            m4 transform = staticEntity.GetTransfromMatrix() * mesh.GetLocalTransform();
            Renderer::SubmitStatic(staticEntity.GetEntityID(), mesh, transform);
        }
    }

}

void Scene::OnUpdate(timestep ts)
{
    for (entity : m_entityManager.GetEntities())
    {
        entity.OnUpdate(ts);
    }
}

class Entity : public Observable
{
    Transfrom* transform;

    void OnUpdate(timestep ts)
    {
        if (transform.Update(ts))
            mark_dirty();
    }
}

void Scene::OnRender()
{

    Renderer::BeginRender(m_Camera);

    for (auto& dynamic_entity : m_EntityManager.GetEntities(Mobility::Dynamic))
    {
        for (auto& mesh : entity.GetModel().GetMeshes())
        {
            m4 transform = entity.GetTransformMatrix() * mesh.GetLocalTransform();
            Renderer::SubmitDynamic(dynamic_entity.GetEntityID(), mesh, transform);
        }
    }


    Renderer::EndRender();
}
```


# Known Problems

## The VRAM is not cleared between scene switches

- This means that the resources are not being freed on the GPU.
- If the VRAM's capacity is exceeded, then the geometries and textures will get corrupted.
- One solution is to clear the VRAM before switching scenes. 
- But this will cause long loading times between scene switches.


