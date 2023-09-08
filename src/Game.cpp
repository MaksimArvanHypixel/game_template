#include "Game.h"

#include <raylib.h>

namespace game
{

Game::Game() : mRenderer(mEcs)
{
	mEcs.set<flecs::Rest>({});
	flecs::log::set_level(0);
	mEcs.import<flecs::monitor>();
}

void Game::Start()
{
	while (mEcs.progress(GetFrameTime()))
	{
	}
}

} // namespace game