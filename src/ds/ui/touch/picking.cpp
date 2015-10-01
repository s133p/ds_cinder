#include "picking.h"

namespace ds {

/**
 * \class ds::Picking
 */
Picking::Picking() {
}

Picking::~Picking() {
}

void Picking::setWorldSize(const glm::vec2& ws) {
	mWorldSize = ws;
}

} // namespace ds
