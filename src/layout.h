/* SPDX-License-Identifier: GPL-2.0 */

#ifndef LAYOUT_H
#define LAYOUT_H

#include <memory>
#include <list>

#include "layout/layout.h"

class Layout;
class OffscreenSurface;
class Surface;

class LayoutItem {
	friend class Layout;

public:
	LayoutItem();
	~LayoutItem();

	void addChild(std::shared_ptr<LayoutItem> child);
	void addSibling(std::shared_ptr<LayoutItem> item,
			std::shared_ptr<LayoutItem> newSibling);

	void removeChild(std::shared_ptr<LayoutItem> child);
	void removeChildren();

	void setSize(lay_vec2 s) { size = s; }
	void setSize(short width, short height) {
		size = (lay_vec2){width, height};
	}
	void setMargins(lay_vec4 m) { margins = m; }
	void setMargins(short left, short top, short right, short bottom) {
		margins = (lay_vec4){left, top, right, bottom};
	}
	void setContainer(uint32_t flags) { c_flags = flags; }
	void setBehave(uint32_t flags) { b_flags = flags; }

	void setVisible(bool v) { visible = v; }
	void show() { visible = true; }
	void hide() { visible = false; }

	bool empty() const { return items.empty(); }

	/* Only valid after layout->run() has been called. */
	lay_vec4 getRect() const { return rect; }

protected:
	/* Ask this layout item to render itself to its coordinates */
	virtual void render(Surface& s) const {}

	std::list<std::shared_ptr<LayoutItem> > items;

private:
	lay_id id;

	lay_vec2 size;
	lay_vec4 rect;
	lay_vec4 margins;
	uint32_t c_flags;
	uint32_t b_flags;
	bool visible;

	void populate(Layout *lay);
	void readSettings(Layout *lay);

	void updateAll();
	void renderAll(Surface& s) const;

	/* Called before layout->run(). */
	virtual void update() {}
};

class LayoutSurface : public LayoutItem {
public:

	LayoutSurface(std::shared_ptr<OffscreenSurface> s);
	LayoutSurface(OffscreenSurface *s);
	~LayoutSurface();

	std::shared_ptr<OffscreenSurface> getSurface() const { return surface; }

protected:
	void render(Surface& s) const;

private:
	std::shared_ptr<OffscreenSurface> surface;
};

class Layout {
	friend class LayoutItem;

public:
	Layout();
	~Layout();

	std::shared_ptr<LayoutItem> topItem() const;

	void run();
	void render(Surface& s) const;

private:
	lay_context ctx;
	std::shared_ptr<LayoutItem> top_item;
	unsigned int nb_items;
};

#endif
