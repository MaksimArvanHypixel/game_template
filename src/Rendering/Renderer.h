#pragma once

#include <flecs.h>

namespace game::Rendering
{

class Renderer
{
public:
	explicit Renderer(const flecs::world& ecs);

private:
	static void PreDraw(const flecs::iter& iter, int index);
	static void DrawDebug(const flecs::iter& iter, int index);
	static void PostDraw(const flecs::iter& iter, int index);
};

} // namespace game::Rendering