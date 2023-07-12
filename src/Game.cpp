#include "Game.h"

#include <raylib.h>

namespace game
{

Game::Game() : mRenderer(mEcs)
{
}

void Game::Start()
{
	while (mEcs.progress(GetFrameTime()))
	{
	}
}

} // namespace game