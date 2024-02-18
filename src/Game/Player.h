#pragma once

class Player {



};
/*
Entity player = ECS.CreateEntity();

ECS.AddComponent<Transform>(player, Transform{});

// ECS

map<ID, Transform> transform_components;

// system

doMovement(int id) {
	if (ECS.hasTransform(id) && ECS.hasPhysics(id)) {
		Transform& t = ECS.GetComponent<Transform>(id);
		Physics& p = ECS.GetComponent<Physics>(id);
		t.pos += p.vel;
	}
}

// update

for (ECS.Ids(PLAYER_GROUPING)) {
	doMovement(id);
}

*/