#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

// TODO 1: Include Box 2 header and library


#define SCALE 1
#define PIXEL_TO_METERS(x) x/SCALE
#define METERS_TO_PIXELS(x) x*SCALE


ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
	
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	b2Vec2 gravity(0.0f, 10.0f);

	world = new b2World(gravity);
	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it


	// TODO 4: Create a a big static circle as "ground"

	//Define a body with position, damping, etc.
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody; // or b2_dynamicBody
	groundBodyDef.position.Set(PIXEL_TO_METERS(SCREEN_WIDTH / 2), PIXEL_TO_METERS(SCREEN_HEIGHT / 2));

	//Use the world object to create the body
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	//Define fixtures with a shape, friction, density, etc.

	b2CircleShape circleShape;
	circleShape.m_radius = PIXEL_TO_METERS(300);

	//Create fixtures on the body.
	groundBody->CreateFixture(&circleShape, 0.0f);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)

	world->Step(1.0f / 60.0f, 8, 3);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{

		b2Vec2 pos(App->input->GetMouseX(), App->input->GetMouseY());
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(pos.x, pos.y);

		b2Body* body = world->CreateBody(&bodyDef);

		b2CircleShape dynamicCircle;
		dynamicCircle.m_radius = 30;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicCircle;

		body->CreateFixture(&fixtureDef);
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete(world);

	return true;
}
