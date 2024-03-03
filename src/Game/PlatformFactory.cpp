#include "PlatformFactory.h"
#include "../Logging.h"

PlatformFactory::PlatformFactory(float platformWidth, float platformHeight): 
	m_platformSize(platformWidth, platformHeight)
{
	// The platforms spawned in game will follow a fixed sequence,
	// which is initialized here.
	// Do note that this can certainly be extended to support as many
	// sequences as you want for variety though, just need to store a pointer to
	// each sequence in another vector, and select whichever one you want.

	Segment segment1;
	segment1.push_back({ { 0, 0 }, { platformWidth, platformHeight } });

	Segment segment2; // empty semgnet adds gaps between platforms

	Segment segment3;
	segment3.push_back({ { 0, 2.5f }, { platformWidth, platformHeight } });
	segment3.push_back({ { 0, -2.5f }, { platformWidth, platformHeight } });

	Segment segment4;

	Segment segment5;
	segment5.push_back({ { 0, 5.0f }, { platformWidth, platformHeight } });
	segment5.push_back({ { 0, -5.0f }, { platformWidth, platformHeight } });

	Segment segment6;

	Segment segment7;
	segment7.push_back({ { 0, 2.5f }, { platformWidth, platformHeight } });
	segment7.push_back({ { 0, -2.5f }, { platformWidth, platformHeight } });

	Segment segment8;

	m_sequence.push_back(segment1);
	m_sequence.push_back(segment2);
	m_sequence.push_back(segment3);
	m_sequence.push_back(segment4);
	m_sequence.push_back(segment5);
	m_sequence.push_back(segment6);
	m_sequence.push_back(segment7);
	m_sequence.push_back(segment8);
}

glm::vec2 PlatformFactory::GetPlatformSize() {
	return m_platformSize;
}

Segment PlatformFactory::GetNextSegmentInSequence() {
	LOG_ASSERT(!m_sequence.empty(), "Platform sequence array is empty!");
	LOG_ASSERT(m_sequenceIndex > -1 && m_sequenceIndex < m_sequence.size(), "Platform sequence index is out of range!");

	Segment segment = m_sequence[m_sequenceIndex];

	// Circular index to restart sequence
	m_sequenceIndex = (m_sequenceIndex + 1) % m_sequence.size();

	return segment;
}