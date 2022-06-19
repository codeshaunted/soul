#include "renderer.hh"

#include <iostream>

using namespace soul;

void DrawCmd::Any::debugLog() {
	std::cout << "[Any]" << std::endl;
}

void DrawCmd::Rect::debugLog() {
	std::cout
		<< "[Rect (" << this->x << ", " << this->y << ") w "
		<< this->width << " h " << this->height << " c "
		<< std::hex << this->color << "]" << std::endl;
}

void DrawCmd::Text::debugLog() {
	std::cout
		<< "[Text <todo>]" << std::endl;
}