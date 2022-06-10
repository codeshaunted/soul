#include "gui.hh"

namespace soul {

GUI GUI::create() {
	return GUI();
}

struct Rect {
	float x, y; // top left
	float width, height;
};

void leavesToRectsRec(std::vector<Rect>& out, Rect& current, GNode& node);

std::pair<std::vector<Vertex>, std::vector<unsigned int>>
GUI::toGeometry(unsigned int window_width, unsigned int window_height) {
	Rect root {0.f, 0.f, static_cast<float>(window_width), static_cast<float>(window_height)};
	std::vector<Rect> rects;
	leavesToRectsRec(rects, root, *this->tree);

	std::vector<Vertex> verts_ret;
	std::vector<unsigned int> idx_ret;

	// todo: convert rects to verts

	return {verts_ret, idx_ret};
}

void leavesToRectsRec(std::vector<Rect>& out, Rect& current, GNode& node) {
	if (node.shouldBeDrawn()) {
		out.push_back(current);
	} else {
		// calculate Rects for each child, and recurse them.
		GNonLeaf& node_nl = static_cast<GNonLeaf&>(node);
		Rect r1, r2;
		if (node_nl.vertical) {
			r1.width = current.width;
			r2.width = current.width;
			r1.x = current.x;
			r2.x = current.x;
			r1.y = current.y;
			float height1 = node_nl.split_point * current.height;
			r1.height = height1;
			r2.height = current.height - height1;
			r2.y = current.y + height1;
			leavesToRectsRec(out, r1, *node_nl.first);
			leavesToRectsRec(out, r2, *node_nl.second);
		} else {
			r1.height = current.height;
			r2.height = current.height;
			r1.y = current.y;
			r2.y = current.y;
			r1.x = current.x;
			float width1 = node_nl.split_point * current.height;
			r1.width = width1;
			r2.width = current.width - width1;
			r2.x = current.x + width1;
			leavesToRectsRec(out, r1, *node_nl.first);
			leavesToRectsRec(out, r2, *node_nl.second);
		}
	}
}

}