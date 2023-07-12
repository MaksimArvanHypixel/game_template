#include "Renderer.h"

#include <raylib.h>
#include <rlImGui.h>

#include "DebugWindow.h"
#include "Globals.h"

namespace game::Rendering
{

Renderer::Renderer(const flecs::world& ecs)
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(FPS);

	rlImGuiSetup(true);

	ecs.system("Renderer::PreDraw")
			.kind(flecs::PreStore)
			.each(PreDraw);

	ecs.system("Renderer::DrawDebug")
			.kind(flecs::PreStore)
			.each(DrawDebug);

	ecs.system("Renderer::PostDraw")
			.kind(flecs::PreStore)
			.each(PostDraw);
}

void Renderer::PreDraw(const flecs::iter& iter, int index)
{
	BeginDrawing();
	rlImGuiBegin();

	ClearBackground(WHITE);
}

void Renderer::DrawDebug(const flecs::iter& iter, int index)
{
	DrawFPS(0, 0);

	flecs::world ecs = iter.world();

	DebugWindow::Draw();

	// Keeping this commented to have it at hand easily when we need to look at the demo window.
//	ImGui::ShowDemoWindow();
}

void Renderer::PostDraw(const flecs::iter& iter, int index)
{
	rlImGuiEnd();
	EndDrawing();
}

} // namespace game::Rendering