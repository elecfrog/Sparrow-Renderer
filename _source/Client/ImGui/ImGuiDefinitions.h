#pragma once

#include "Base/BaseDefinition.h"

namespace Sparrow
{
	using ComponentCallback = std::function<void()>;

	enum class UIComponentType
	{
		Panel,
		Dockspace,
		MenuBar,
		ProfilingPanel,
		HierarchyPanel,
		InspectorPanel
	};

	enum class GameOjbectComponentType
	{
		Transform,
		Camera,
		Light,
		Renderer,
		// ... Add more component types.
	};



	template <GameOjbectComponentType T>
	struct ComponentData{ };

	template <>
	struct ComponentData<GameOjbectComponentType::Transform>
	{
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;
		ComponentData() = default;
		ComponentData(Vector3f position, Vector3f rotation, Vector3f scale)
			: position(std::move(position)), rotation(std::move(rotation)), scale(std::move(scale))
		{ }
	};

	template <>
	struct ComponentData<GameOjbectComponentType::Light>
	{
		Vector3f position;
		// TODO Add more attributes, and move position to transform
		ComponentData() = default;
		ComponentData(Vector3f position)
			: position(std::move(position))
		{ }

	};

	template <>
	struct ComponentData<GameOjbectComponentType::Camera>
	{
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;
	};

	template <>
	struct ComponentData<GameOjbectComponentType::Renderer>
	{
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;
	};

	// Continue with other component types

	using ComponentVariant = std::variant<
		ComponentData<GameOjbectComponentType::Transform>,
		ComponentData<GameOjbectComponentType::Camera>,
		ComponentData<GameOjbectComponentType::Light>,
		ComponentData<GameOjbectComponentType::Renderer>
		// ... Add more component types.
	>;

	struct GameOjbectComponent
	{
		GameOjbectComponentType     type;
		ComponentVariant            data; // which is point to the data of the component of the game object.
		ComponentCallback           callback;
	};


	struct GameObject
	{
		std::string name;
		std::vector<GameOjbectComponent> components;
		// Add more object properties if needed.

		GameObject() = default;
		GameObject(const std::string& name, const std::vector<GameOjbectComponent>& components)
			: name(name), components(components)
		{ }
	};

}