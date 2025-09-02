#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent{
    protected:
        static int nextId;
};

template <typename TComponent>
class Component: public IComponent{
    static int GetId(){
        static auto id = nextId++;
        return id;
    }
};

class Entity{
    private:
        int id;

    public:
        Entity(int _id) : id(_id){};
        int GetId() const;

        Entity& operator =(const Entity& other) = default;
        bool operator ==(const Entity& otherEntity) const{ return id == otherEntity.id; }
        bool operator !=(const Entity& otherEntity) const{ return id != otherEntity.id; }
};

class System{
    private:
        Signature componentSignature;
        std::vector<Entity> entities;

    public:
        System() = default;
        ~System() = default;

        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        std::vector<Entity> GetSystemEntities() const;
        const Signature& GetComponentSignature() const;

        template <typename TComponent> void RequireComponent();
};

class IPool{
    public:
        virtual ~IPool(){}
};

/// Pool - Vector of contiguous data of objects of type
template <typename T>
class Pool : public IPool{
    private:
        std::vector<T> data;

    public:
        Pool(int size = 100){
            data.resize(size);
        }

        ~Pool() = default;

        bool isEmpty() const { return data.empty();}
        int GetSize() const { return data.size();}
        void Resize(int n) { data.resize(n);}
        void Clear() { data.clear();}

        void Add(T object){ data.push_back(object);}
        void Set(int index, T object){ data[index] = object; }
        T& Get(int index){ return static_cast<T&>(data[index]); }

        T& operator [](unsigned int index){ return data[index]; }
};


///  
/// Manages the creation and destruction of the entitites, add systems and components. 
/// Game manager in a way
///

class Registry{
    private: 
    int numEntities = 0;

    // Pool index is entity ID
    std::vector<IPool*> componentPools;

    // It lets us know which component is turned on for an entity
    // Vector index is entity ID
    std::vector<Signature> entityComponentSignature;

    std::unordered_map<std::type_index, System*> system;

    // Set of entities that will be added or removed on the next registry update
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;

    public:
        Registry() = default;
        void Update();
        // TO DO 
        // Create an entity
        Entity CreateEntity();
        void AddEntityToSystem(Entity entity);

        template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
        template <typename T> void RemoveComponent(Entity entity);
        template <typename T> bool HasComponent(Entity entity) const;
        // Kill an entity
        // Add components to entities
        // Remove Components from entities
        // Has certain component
        // Get Component from entity
        // Add system
        // Remove system
        // Get System
        // Has System
};

template <typename TComponent, typename ...TArgs> 
void Registry::AddComponent(Entity entity, TArgs&& ...args)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if(componentId >= componentPools.size()){
        componentPools.resize(componentId + 1, nullptr);
    }

    if(!componentPools[componentId])
    {
        Pool<TComponent>* newComponentPool = new Pool<TComponent>();
        componentPools[componentId] = newComponentPool;
    }

    Pool<TComponent> componentPool = componentPools[componentId];

    if(entityId >= componentPool->GetSize()){
        componentPool->Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);
    entityComponentSignature[entityId].set(componentId);
}

template <typename T>
void Registry::RemoveComponent(Entity entity)
{
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    entityComponentSignature[entityId].set(componentId, false);
}

template <typename T> 
bool Registry::HasComponent(Entity entity) const{
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    return entityComponentSignature[entityId].test(componentId);
}

template <typename TComponent>
void System::RequireComponent(){
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}
#endif