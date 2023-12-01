#include "Game.h"

#include <raylib.h>
#include <iostream>

#include "Rendering/Renderer.h"
#include <Globals.h>
#include <Utils.h>
#include <Vector.h>
#include <string>


namespace game
{

///////////////////////////////////////////////////////////////////////////////////

typedef struct {
	types::Vector position;
	types::Vector heading;
	float speed;
} Orientation;

typedef struct {
	const char* value;
} Name;

typedef struct {
} Player;

typedef struct {
} Enemy;

typedef struct {
} Bullet;

typedef struct {
	float timeout;
	float radius;
} Bomb;

typedef struct {
} Attractor;

typedef struct {
} Targetable;

typedef struct {
	Color color;
	int radius;
} Visual;

typedef struct {
	int amount;
} Health;

///////////////////////////////////////////////////////////////////////////////////

Rectangle viewport = {0.f, 0.f, (float)VIEWPORT_WIDTH, (float)VIEWPORT_HEIGHT};

///////////////////////////////////////////////////////////////////////////////////

void PlayerMoveSystem(flecs::iter& iter, Orientation* orientation) 
{
	float pixelsStep = orientation->speed * iter.delta_time();

	for (auto i : iter)
	{
		bool adjustOrientation = false;
		types::Vector newPosition = orientation[i].position;

		if (IsKeyDown('A'))
		{
			newPosition.x -= pixelsStep;
			adjustOrientation = true;
		}
		if (IsKeyDown('D'))
		{
			newPosition.x += pixelsStep;
			adjustOrientation = true;
		}
		if (IsKeyDown('W'))
		{
			newPosition.y -= pixelsStep;
			adjustOrientation = true;
		}
		if (IsKeyDown('S'))
		{
			newPosition.y += pixelsStep;
			adjustOrientation = true;
		}

		if (adjustOrientation && utils::IsPositionInsideOfRectangle(viewport, newPosition))
		{
			orientation[i].heading = (newPosition - orientation[i].position).normalize();

			orientation[i].position = newPosition;
		}
	}
}

void EnemyMoveSystem(flecs::iter& iter, Orientation* orientation)
{
	flecs::filter atttractorsFilter = iter.world().filter<Attractor, Orientation>();
	flecs::filter playerFilter = iter.world().filter<Player, Orientation, Visual>();

	for (auto i : iter)
	{
		atttractorsFilter.each([i, &orientation](Attractor& a, Orientation& o)
		{
			// Just grab first one, whatever
			if (orientation[i].position.sqDistanceTo(o.position) < utils::SquareOf(ATTRACTION_DISTANCE))
			{
				orientation[i].heading = o.position - orientation[i].position;
				orientation[i].heading.normalize();
			}
		});

		flecs::entity hitEntity = playerFilter.find([i, &orientation](Player& p, Orientation& o, Visual& v) {
			return (orientation[i].position.sqDistanceTo(o.position) < utils::SquareOf(v.radius));
		});

		if (hitEntity.is_alive())
		{
			std::cout << "You're HIT" << std::endl;
		}

		orientation[i].heading.x *= utils::IsPositionInsideOfRectangleX(viewport, orientation[i].position) ? 1.f : -1.f;
		orientation[i].heading.y *= utils::IsPositionInsideOfRectangleY(viewport, orientation[i].position) ? 1.f : -1.f;

		orientation[i].position.x += (orientation[i].heading.x * orientation[i].speed * iter.delta_time());
		orientation[i].position.y += (orientation[i].heading.y * orientation[i].speed * iter.delta_time());
	}
}

void BulletMoveSystem(flecs::iter& iter, Orientation* orientation)
{
	flecs::filter enemyFilter = iter.world().filter<Targetable, Orientation, Visual>();

	for (auto i : iter)
	{
		flecs::entity hitEntity = enemyFilter.find([i, &orientation](Targetable& t, Orientation& o, Visual& v) {
			return (orientation[i].position.sqDistanceTo(o.position) < utils::SquareOf(v.radius));
		});

		if (hitEntity.is_alive())
		{
			iter.entity(i).destruct();
			std::cout << "HIT" << std::endl;
			hitEntity.destruct();
		}

		orientation[i].position.x += (orientation[i].heading.x * orientation[i].speed * iter.delta_time());
		orientation[i].position.y += (orientation[i].heading.y * orientation[i].speed * iter.delta_time());

		if (!utils::IsPositionInsideOfRectangle(viewport, orientation[i].position))
		{
			iter.entity(i).destruct();
		}
	}
}

void PlayerShootSystem(flecs::entity& entity, Orientation& orientation)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		types::Vector shootDirection = (types::Vector{GetMouseX(), GetMouseY()} - orientation.position).normalize();

		entity.world().entity()
			.set<Orientation>({orientation.position, shootDirection, BULLET_SPEED})
			.set<Name>({"Bullet"})
			.set<Visual>({RED, 2})
			.add<Bullet>();
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		entity.world().entity()
			.set<Orientation>({orientation.position})
			.set<Name>({"Bomb"})
			.set<Visual>({BLUE, 6})
			.set<Bomb>({3, 200});
	}
}

void BombSystem(flecs::iter& iter, Bomb* bomb, Orientation* orientation)
{
	for (auto i : iter)
	{
		if ((bomb[i].timeout -= iter.delta_time()) <= 0.f)
		{
			std::cout << "Ka BOOM" << std::endl;

			flecs::filter bombTargetFilter = iter.world().filter<Targetable, Orientation>();

			bombTargetFilter.each([&bomb, i, &orientation]( flecs::entity entity, Targetable& t, Orientation& o) 
			{
				if (orientation[i].position.sqDistanceTo(o.position) < utils::SquareOf(bomb[i].radius))
				{
					entity.destruct();
				}
			});
				
			iter.entity(i).destruct();
		}
	}
}

void DrawEntitySystem(flecs::entity& entity, const Orientation& orientation, const Visual& visual)
{
	DrawCircle(orientation.position.x, orientation.position.y, visual.radius, visual.color);

	// Shouldn't be here
	if (const Bomb* bomb = entity.get<Bomb>())
	{
		DrawCircleLines(orientation.position.x, orientation.position.y, bomb->radius, visual.color);
	}
}

void HealthUISystem(flecs::entity& entity)
{
	int posX = 0;
	int posY = 20;

	Color color = LIME;

	const Health* health = entity.get<Health>();

	DrawText(TextFormat("Health: %2i", health->amount), posX, posY, 20, color);
}

Game::Game()
{
	mEcsWorld.set<flecs::Rest>({});
	flecs::log::set_level(0);

	mEcsWorld.import <flecs::monitor>();

	game::Rendering::Init(mEcsWorld);

	//////////////////////////////////////

	mEcsWorld.component<Orientation>();
	mEcsWorld.component<Name>();
	mEcsWorld.component<Visual>();
	mEcsWorld.component<Targetable>();
	mEcsWorld.component<Health>();
	mEcsWorld.component<Attractor>();
	mEcsWorld.component<Bomb>();
	mEcsWorld.component<Bullet>();
	mEcsWorld.component<Player>();
	mEcsWorld.component<Enemy>();

	static Rectangle viewPort{0.f, 0.f, (float)VIEWPORT_WIDTH, (float)VIEWPORT_HEIGHT};

	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		mEcsWorld.entity()
			.set<Orientation>({utils::GetRandomPositionInRectangle(viewPort), utils::GetRandomDirection(), ENEMY_SPEED})
			.set<Name>({("Enemy" + std::to_string(i)).c_str()})
			.set<Health>({ENEMY_HEALTH_AMOUNT})
			.set<Visual>({ORANGE, 10})
			.add<Targetable>()
			.add<Enemy>();
	}

	flecs::entity playerEntity = mEcsWorld.entity()
		.set<Orientation>({utils::GetRandomPositionInRectangle(viewPort), {1, 0}, PLAYER_SPEED})
		.set<Name>({"Player"})
		.emplace<Visual>(BLACK, 10)
		.set<Health>({PLAYER_HEALTH_AMOUNT})
		.add<Attractor>()
		.add<Player>();

	flecs::entity weaponEntity = mEcsWorld.entity().child_of(playerEntity)
		.set<Name>({"Weapon"});

	mEcsWorld.system<Orientation>("PlayerMoveSystem")
		.with<Player>()
		.without<Enemy>()
		.kind(flecs::OnUpdate)
		.iter(PlayerMoveSystem);

	mEcsWorld.system<Orientation>("EnemyMoveSystem")
		.with<Enemy>()
		.without<Player>()
		.kind(flecs::OnUpdate)
		.iter(EnemyMoveSystem);

	mEcsWorld.system<Orientation>("BulletMoveSystem")
		.with<Bullet>()
		.kind(flecs::OnUpdate)
		.iter(BulletMoveSystem);

	mEcsWorld.system<Orientation>("PlayerShootSystem")
		.with<Player>()
		.without<Enemy>()
		.kind(flecs::OnUpdate)
		.each(PlayerShootSystem);

	mEcsWorld.system("HealthUISystem")
		.with<Player>()
		.without<Enemy>()
		.read<Health>()
		.kind(flecs::OnUpdate)
		.each(HealthUISystem);

	mEcsWorld.system<Bomb, Orientation>("BombSystem")
		.kind(flecs::OnUpdate)
		.iter(BombSystem);

	mEcsWorld.system<const Orientation, const Visual>("DrawEntity")
		.kind(flecs::OnUpdate)
		.each(DrawEntitySystem);

	mEcsWorld.app().enable_rest();
}

void Game::Update()
{
	while (mEcsWorld.progress(GetFrameTime()))
	{
		if (WindowShouldClose())
			mEcsWorld.quit();
	}
}

}// namespace game 
