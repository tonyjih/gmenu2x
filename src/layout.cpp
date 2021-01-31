// SPDX-License-Identifier: GPL-2.0

#define LAY_IMPLEMENTATION
#include "layout.h"
#include "surface.h"

#include <algorithm>
#include <memory>

Layout::Layout()
{
	lay_init_context(&ctx);

	top_item = std::make_shared<LayoutItem>();
}

Layout::~Layout()
{
	lay_destroy_context(&ctx);
}

void Layout::run()
{
	top_item->updateAll();

	lay_reset_context(&ctx);
	top_item->populate(this);
	lay_run_context(&ctx);
	top_item->readSettings(this);
}

void Layout::render(Surface& s) const
{
	top_item->renderAll(s);
}

std::shared_ptr<LayoutItem> Layout::topItem() const
{
	return top_item;
}

LayoutItem::LayoutItem()
{
	size = (lay_vec2){0,0};
	rect = (lay_vec4){0,0,0,0};
	margins = (lay_vec4){0,0,0,0};
	c_flags = 0;
	b_flags = 0;
	visible = true;
}

LayoutItem::~LayoutItem()
{
	setSize(0, 0);
}

void LayoutItem::populate(Layout *lay)
{
	lay_id previous = LAY_INVALID_ID;

	id = lay_item(&lay->ctx);

	for (auto it: items) {
#if 0
		if (!it->visible)
			continue;
#endif

		it->populate(lay);

		if (previous == LAY_INVALID_ID)
			lay_insert(&lay->ctx, id, it->id);
		else
			lay_append(&lay->ctx, previous, it->id);

		previous = it->id;
	}

	lay_set_size(&lay->ctx, id, size);
	lay_set_margins(&lay->ctx, id, margins);
	lay_set_contain(&lay->ctx, id, c_flags);
	lay_set_behave(&lay->ctx, id, b_flags);
}

void LayoutItem::readSettings(Layout *lay)
{
	if (1 || visible) {
		rect = lay_get_rect(&lay->ctx, id);

		for (auto it: items)
			it->readSettings(lay);
	}
}

void LayoutItem::updateAll()
{
	update();

	for (auto it: items)
		it->updateAll();
}

void LayoutItem::renderAll(Surface& s) const
{
	if (visible) {
		render(s);

		for (auto it: items)
			it->renderAll(s);
	}
}

void LayoutItem::addChild(std::shared_ptr<LayoutItem> child)
{
	items.push_back(child);
}

void LayoutItem::addSibling(std::shared_ptr<LayoutItem> item,
			    std::shared_ptr<LayoutItem> newSibling)
{
	auto it = std::find(items.begin(), items.end(), item);

	items.insert(++it, newSibling);
}

void LayoutItem::removeChild(std::shared_ptr<LayoutItem> child)
{
	auto it = std::find(items.begin(), items.end(), child);

	items.erase(it);
}

void LayoutItem::removeChildren()
{
	items.clear();
}

LayoutSurface::LayoutSurface(std::shared_ptr<OffscreenSurface> s)
	: surface(s)
{
	if (s)
		setSize(s->width(), s->height());
	setBehave(LAY_CENTER);
}

LayoutSurface::LayoutSurface(OffscreenSurface *s)
{
	LayoutSurface(std::shared_ptr<OffscreenSurface>(s));
}

LayoutSurface::~LayoutSurface()
{
}

void LayoutSurface::render(Surface& s) const
{
	lay_vec4 r = getRect();

	if (surface)
		surface->blit(s, r[0], r[1], r[2], r[3]);
}
