#pragma once

#include <flecs.h>

namespace game
{

class Game
{
public:
	explicit Game();

	void Init();
	void Update();

private:
	flecs::world mEcsWorld{};
};

} // namespace game