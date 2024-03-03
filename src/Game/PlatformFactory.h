#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../ECS.h"

// A platform will simply contain a position and size
struct Platform {
	glm::vec2 position;
	glm::vec2 size;

	Platform(glm::vec2 position, glm::vec2 size) : position(position), size(size) { }
};

// Segment is a collection of platforms that share the same x coord, but differ in y coordinates.
using Segment = std::vector<Platform>;

class PlatformFactory {
public:

	PlatformFactory(float platformWidth, float platformHeight);

	Segment GetNextSegmentInSequence();

	glm::vec2 GetPlatformSize();

private:

	int m_sequenceIndex = 0;
	std::vector<Segment> m_sequence;

	glm::vec2 m_platformSize;

};