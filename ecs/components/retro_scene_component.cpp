#include "retro_scene_component.h"
#include "retro_render_component.h"
#include "../retro_entity.h"

retro_component(C_SceneConstructor)
{
	// object that waits for a render component to be added to the object so that sectoring can be done
protected:
	void OnUpdate( void ) override {
		auto *r = GetObject()->GetComponent<retro3d::RenderComponent>();
		if (r != nullptr && r->GetModel().IsNull() == false) {
			// TODO: Do work
			Destroy(); // NOTE: Remove component.
		}
	}

public:
	C_SceneConstructor( void ) : mtlInherit(this) {}
};

void retro3d::SceneComponent::OnSpawn( void )
{
	auto *r = GetObject()->GetComponent<retro3d::RenderComponent>();
	if (r == nullptr || r->GetModel().IsNull()) {
		GetObject()->AddComponent<C_SceneConstructor>();
	}
}

retro3d::SceneComponent::SceneComponent( void ) : mtlInherit(this)
{}
