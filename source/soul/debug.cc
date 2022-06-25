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
		<< "[Text (" << this->x << "," << this->y << ") s" << this->scale;
	if (!this->first) {
		std::cout << " {empty}";
	} else {
		auto cur = this->first;
		while (cur) {
			std::cout << std::endl << "  "
				<< std::hex << cur->color_abgr << " \""
				<< cur->text << "\"";
			cur = cur->next;
		}
	}
	std::cout << std::endl << "]" << std::endl;
}
