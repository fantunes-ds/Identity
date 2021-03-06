#pragma once
#include <Export.h>
#include <Components/Transform.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Containers/ComponentContainer.h>
#include <Components/IComponent.h>
#include <Components/ModelComponent.h>
#include <Scene/SceneGraph/SceneNode.h>
#include "Managers/ResourceManager.h"

namespace Engine::Objects
{
	class API_ENGINE GameObject : public IObject
	{
	public:
		GameObject();
		GameObject(const std::string& p_name);
		~GameObject();

		//TODO: works when modifying parent's transform, but doesn't work when modifying child's transform
		void SetParentObject(std::shared_ptr<GameObject> p_parent);

		/**
		 * @brief Deletes component.
		 * @return returns true Component has been successfully deleted from memory.
		 */
		bool RemoveComponent(int32_t p_id);
		bool RemoveAllComponents();

		template <class T>
		bool RemoveComponent()
		{
			for (auto component : m_components)
			{
				if (std::shared_ptr<T> foundComp = std::dynamic_pointer_cast<T>(Containers::ComponentContainer::FindComponent(component)))
				{
					for (size_t i = 0; i < m_components.size(); ++i)
					{
						if (m_components[i] == foundComp->GetID())
						{
							if (foundComp->RemoveComponent())
							{
							    m_components.erase(m_components.begin() + i);
								return true;
							}
							return false;
						}
					}
				}
			}

			return false;
		}


		/**
		 * @brief Creates a new Component and adds it to the GameObject and the ComponentContainer.
		 * @return ID of the newly created Component.
		 */
		template <class T, typename ...Args>
		int32_t AddComponent(Args& ... p_args)
		{
			if (std::is_same_v<T, Components::ModelComponent> && FindComponentOfType<Components::ModelComponent>())
			{
				std::string error("Tried to add a ModelComponent on object " + m_name + " while it already has one. Second ModelComponent has not been added.");
				MessageBox(nullptr, error.c_str(), "Warning", MB_OK | MB_ICONWARNING);
				return -1;
			}

			int32_t id = Containers::ComponentContainer::AddComponent<T>(this, p_args...);

            if (std::is_same_v<T, Components::ModelComponent>)
            {
                //TODO change this to use the resource manager
                std::shared_ptr<Components::ModelComponent> modelComp = std::dynamic_pointer_cast<Components::ModelComponent>(Containers::ComponentContainer::FindComponent(id));
            }

			if (id > 0)
			{
				m_components.emplace_back(id);
			}

			return id;
		}

		/**
		 * @return The first instance of the desired component type
		 */
		std::shared_ptr<Components::IComponent> FindComponent(int32_t p_id) const
		{
			return Containers::ComponentContainer::FindComponent(p_id);
		}

		/**
		 * @brief Finds the first component of the type T attached to this GameObject. If this GameObjects contains more than 1
		 * of the desired type of Component, you should use FindAllComponentsOfType<T>().
		 */
		template <class T>
		std::shared_ptr<T> FindComponentOfType() const
		{
			for (auto component : m_components)
			{
				if (std::shared_ptr<T> foundComp = std::dynamic_pointer_cast<T>(Containers::ComponentContainer::FindComponent(component)))
					return foundComp;
			}

			return nullptr;
		}

		/**
		 * @brief Finds all instances of Components of type T.
		 * @return All Components of type T.
		 */
		template <class T>
		std::vector<std::shared_ptr<T>> FindAllComponentsOfType() const
		{
			std::vector<std::shared_ptr<T>> foundComps;

			for (auto component : m_components)
			{
				if (std::shared_ptr<T> foundComp = std::dynamic_pointer_cast<T>(Containers::ComponentContainer::FindComponent(component)))
					foundComps.push_back(foundComp);
			}

			return foundComps;
		}

		void DeleteFromMemory();
		void Serialize(std::ostream& p_stream);
		void Deserialize(std::vector<std::string>& p_strings);

		[[nodiscard]] std::shared_ptr<Components::Transform> GetTransform() const;
		[[nodiscard]] inline uint32_t GetTransformID() const { return m_transform; }
		[[nodiscard]] std::shared_ptr<ObjectElements::Model> GetModel() const;
		[[nodiscard]] std::shared_ptr<Scene::SceneNode> GetSceneNode() const { return m_sceneNode; }
		inline std::vector<int32_t>& GetAllComponents() { return m_components; }
		
		inline void SetTransform(int32_t p_transform) { m_transform = p_transform; }
		inline void SetSceneNode(std::shared_ptr<Scene::SceneNode> p_node) { m_sceneNode = p_node; }
		inline void SetActive(bool p_active);
		inline bool IsActive() { return m_isActive; }

		bool operator==(GameObject& p_other) const;

	private:
		bool m_isActive = true;
		int32_t m_transform = -1;
		std::vector<int32_t> m_components;
		std::shared_ptr<Scene::SceneNode> m_sceneNode = nullptr;
	};
}
