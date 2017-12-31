#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

#include "ModulePlayer.h"

#define SIZE 3
#define SPACE_PART_CIRCUIT 4
#define RENDER_DISTANCE 30

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	Plane p2(0,1,0,0);
	p = p2;
	LOG("Loading Intro assets");
	bool ret = true;
	createMap1();
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	p2List_item<Primitive*>* rec = primitiveListMesh.getFirst();
	while (rec != nullptr) {
		int xx, yy;
		rec->data->getXY(xx, yy);
		int xFinal = xx - (int)App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX();
		if (xFinal < 0)
			xFinal *= -1;
		int yFinal = yy - (int)App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ();// Sha de pillar la Z perque Y es al�ada
		if (yFinal < 0)
			yFinal *= -1;
		if (xFinal < RENDER_DISTANCE && yFinal < RENDER_DISTANCE)
			rec->data->Render();
		rec = rec->next;
	}
	p.Render();


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::createMap1() {
	mapXML = new pugi::xml_document;
	mapXML->load_file("mapa.tmx");
	pugi::xml_node layer = mapXML->child("map").child("layer");
	std::string nullLayerString = "";
	std::string elements = "elements";
	while (layer.attribute("name").as_string() != nullLayerString) {
		pugi::xml_node layer_data = layer.child("data");
		pugi::xml_node tile = layer_data.child("tile");

		for (int y = 0; y < mapXML->child("map").attribute("height").as_uint(); ++y)
		{
			for (int x = 0; x < mapXML->child("map").attribute("width").as_int(); ++x)
			{
				switch (tile.attribute("gid").as_int() - 1) {
				case 0:
					break;
				case 1:

					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					addPrimitiveToMap(CUBE, x * SIZE, 0, y * SIZE, SIZE, SIZE, SIZE);
					break;
				case 6:
					addPrimitiveToMap(SPHERE, x * SIZE, 0, y * SIZE, SIZE, SIZE, SIZE);
					break;
				case 7:
					break;
				case 8:
					break;
				case 9:
					addPrimitiveToMap(CIRCUIT_RECTA, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 10:
					addPrimitiveToMap(CIRCUIT_ROTA_FRONT_RIGHT, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 11:
					addPrimitiveToMap(CIRCUIT_ROTA_FRONT_LEFT, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 12:
					addPrimitiveToMap(CIRCUIT_ROTA_BACK_RIGHT, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 13:
					addPrimitiveToMap(CIRCUIT_ROTA_BACK_LEFT, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 14:
					addPrimitiveToMap(CIRCUIT_RECTA_LATERAL, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				}
				tile = tile.next_sibling("tile");
			}
		}
		layer = layer.next_sibling("layer");
	}
}

void ModuleSceneIntro::addPrimitiveToMap(primitiveTypes type, int x, int y, int z, int radOrX, int Y = 1, int Z = 1) {
	switch (type) {
	case CUBE: {
		Cube cube(radOrX, Y, Z);
		cube.SetPos(x, y, z);
		App->physics->AddBody(cube, 0);
		break;
	}
	case SPHERE: {
		Sphere sphere(radOrX);
		sphere.SetPos(x, y, z);
		App->physics->AddBody(sphere, 0);
		break;
	}
	case CIRCUIT_RECTA: {
		Cube cube(radOrX, Y , Z * SPACE_PART_CIRCUIT);
		Cube cube2(radOrX, Y , Z * SPACE_PART_CIRCUIT);
		cube.SetPos(x + SIZE * SPACE_PART_CIRCUIT/2, y, z );
		addMeshToMap(Primitive_Custom, "meshes/balla2.obj", x + SIZE * SPACE_PART_CIRCUIT / 2.3f, 0, z, 0);
		cube2.SetPos(x - SIZE * SPACE_PART_CIRCUIT/2, y, z );
		addMeshToMap(Primitive_Custom, "meshes/balla2.obj", x - SIZE * SPACE_PART_CIRCUIT / 2.3f, 0, z, 0);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
		break;
	}
	case CIRCUIT_ROTA_FRONT_RIGHT: {
		Cube cube(radOrX, Y, Z * SPACE_PART_CIRCUIT / 4 * 3);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x - SIZE * SPACE_PART_CIRCUIT / 2, y, z);
		cube2.SetPos(x, y, z - SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/esquina.obj", x - SIZE * SPACE_PART_CIRCUIT / 2.3f, 0, z, 0);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
	}
		break;
	case CIRCUIT_ROTA_FRONT_LEFT: {
		Cube cube(radOrX, Y, Z * SPACE_PART_CIRCUIT / 4 * 3);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x + SIZE * SPACE_PART_CIRCUIT / 2, y, z);
		cube2.SetPos(x, y, z - SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/esquina.obj", x , 0, z - SIZE * SPACE_PART_CIRCUIT / 2.3f, 270);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
	}
		break;
	case CIRCUIT_ROTA_BACK_RIGHT: {
		Cube cube(radOrX, Y, Z * SPACE_PART_CIRCUIT / 4 * 3);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x - SIZE * SPACE_PART_CIRCUIT / 2, y, z);
		cube2.SetPos(x, y, z + SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/esquina.obj", x, 0, z + SIZE * SPACE_PART_CIRCUIT / 2.3f, 90);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
	}
		break;
	case CIRCUIT_ROTA_BACK_LEFT: {
		Cube cube(radOrX, Y, Z * SPACE_PART_CIRCUIT / 4 * 3);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x + SIZE * SPACE_PART_CIRCUIT / 2, y, z);
		cube2.SetPos(x, y, z + SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/esquina.obj", x + SIZE * SPACE_PART_CIRCUIT / 2.3f, 0, z , 180);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
		
	}
		break;
	case CIRCUIT_RECTA_LATERAL:
		Cube cube(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x, y, z + SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/balla2.obj", x, 0, z + SIZE * SPACE_PART_CIRCUIT / 2.3f, 90);
		cube2.SetPos(x, y, z - SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/balla2.obj", x, 0, z - SIZE * SPACE_PART_CIRCUIT / 2.3f, 90);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
		break;
	}
}

void ModuleSceneIntro::addMeshToMap(PrimitiveTypes type, char* path, int x, int y, int z, int degToRotate = 0) {
	switch (type) {
	case	Primitive_Plane:
	case	Primitive_Cube:
	case	Primitive_Sphere:
	case	Primitive_Cylinder:
	case	Primitive_Custom: {
		Primitive* item = new Custom(path,x,y,z,degToRotate);
		primitiveListMesh.add(item);
	}
	}
}

void ModuleSceneIntro::CleanMeshes() {
	p2List_item<Primitive*>* rec = primitiveListMesh.getFirst();
	while (rec != nullptr) {
		p2List_item<Primitive*>* aux = rec;
		rec = rec->next;
		primitiveListMesh.del(aux);
	}
}