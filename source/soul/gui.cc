#include "gui.hh"
#include <cstdint>
#include <optional>

namespace soul {

GUI GUI::create() {
	return GUI();
}

struct Rect {
	float x, y; // top left
	float width, height;
	uint32_t color;
};

void leavesToRectsRec(std::vector<Rect>& out, Rect& current, GNode& node);

uint16_t addVert(std::vector<Vertex>& vec, Vertex to_add) {
	// TODO: don't add duplicate verts?
	uint16_t idx = vec.size();
	vec.push_back(to_add);
	return idx;
}

std::optional<GNonLeaf*> GNonLeaf::create(bool horizontal, float split, GNode *first, GNode *second) {
	if (
		split >= 1 || 
		split <= 0 ||
		!first ||
		!second
	) return std::nullopt;
	return new GNonLeaf(horizontal, split, first, second);
}

GNonLeaf::GNonLeaf(bool split_horizontal, float split, GNode* first, GNode* second) {
	this->horizontal = split_horizontal;
	this->split_point = split;
	this->first = first;
	this->second = second;
}

std::pair<std::vector<Vertex>, std::vector<uint16_t>>
GUI::toGeometry(unsigned int window_width, unsigned int window_height) {
	Rect root {0.f, 0.f, static_cast<float>(window_width), static_cast<float>(window_height), 0xff000000};
	std::vector<Rect> rects;
	leavesToRectsRec(rects, root, *this->tree);

	std::vector<Vertex> verts_ret;
	std::vector<uint16_t> idx_ret;

	for (auto r : rects) {
		auto tl = addVert(verts_ret, Vertex{r.x, r.y, 0.f, r.color});
		auto tr = addVert(verts_ret, Vertex{r.x + r.width, r.y, 0.f, r.color});
		auto bl = addVert(verts_ret, Vertex{r.x, r.y + r.height, 0.f, r.color});
		auto br = addVert(verts_ret, Vertex{r.x + r.width, r.y + r.height, 0.f, r.color});
		idx_ret.push_back(bl);
		idx_ret.push_back(tr);
		idx_ret.push_back(tl);
		idx_ret.push_back(bl);
		idx_ret.push_back(br);
		idx_ret.push_back(tr);
	}

	return {verts_ret, idx_ret};
}

void leavesToRectsRec(std::vector<Rect>& out, Rect& current, GNode& node) {
	if (node.shouldBeDrawn()) {
		GLeaf& node_leaf = static_cast<GLeaf&>(node);
		current.color = node_leaf.color;
		out.push_back(current);
	} else {
		// calculate Rects for each child, and recurse them.
		GNonLeaf& node_nl = static_cast<GNonLeaf&>(node);
		Rect r1, r2;
		if (node_nl.horizontal) {
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