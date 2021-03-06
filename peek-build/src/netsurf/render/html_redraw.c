/*
 * Copyright 2004-2008 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2004-2007 John M Bell <jmb202@ecs.soton.ac.uk>
 * Copyright 2004-2007 Richard Wilson <info@tinct.net>
 * Copyright 2005-2006 Adrian Lees <adrianl@users.sourceforge.net>
 * Copyright 2006 Rob Kendrick <rjek@netsurf-browser.org>
 * Copyright 2008 Michael Drake <tlsa@netsurf-browser.org>
 * Copyright 2009 Paul Blokus <paul_pl@users.sourceforge.net> 
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file
 * Redraw of a CONTENT_HTML (implementation).
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils/config.h"
#include "content/content_protected.h"
#include "css/css.h"
#include "css/utils.h"
#include "desktop/gui.h"
#include "desktop/plotters.h"
#include "desktop/knockout.h"
#include "desktop/selection.h"
#include "desktop/textinput.h"
#include "desktop/options.h"
#include "desktop/print.h"
#include "desktop/search.h"
#include "desktop/scroll.h"
#include "image/bitmap.h"
#include "render/box.h"
#include "render/font.h"
#include "render/form.h"
#include "render/layout.h"
#include "utils/log.h"
#include "utils/messages.h"
#include "utils/utils.h"


static bool html_redraw_box(struct box *box, int x, int y, struct rect clip,
		float scale, colour current_background_color);
static bool html_redraw_box_children(struct box *box,
		int x_parent, int y_parent, struct rect clip,
		float scale, colour current_background_color);
static bool html_redraw_text_box(struct box *box, int x, int y,
		struct rect clip, float scale,
		colour current_background_color);
static bool html_redraw_caret(struct caret *caret,
		colour current_background_color, float scale);
static bool html_redraw_borders(struct box *box, int x_parent, int y_parent,
		int p_width, int p_height, struct rect clip, float scale);
static bool html_redraw_inline_borders(struct box *box, struct rect b,
		struct rect clip, float scale, bool first, bool last);
static bool html_redraw_border_plot(const int side, const int *p, colour c,
		enum css_border_style_e style, int thickness, bool rectangular,
		struct rect clip);
static bool html_redraw_checkbox(int x, int y, int width, int height,
		bool selected);
static bool html_redraw_radio(int x, int y, int width, int height,
		bool selected);
static bool html_redraw_file(int x, int y, int width, int height,
		struct box *box, float scale, colour background_colour);
static bool html_redraw_background(int x, int y, struct box *box, float scale,
		struct rect clip, colour *background_colour,
		struct box *background);
static bool html_redraw_inline_background(int x, int y, struct box *box,
		float scale, struct rect clip, struct rect b,
		bool first, bool last, colour *background_colour);
static bool html_redraw_text_decoration(struct box *box,
		int x_parent, int y_parent, float scale,
		colour background_colour);
static bool html_redraw_text_decoration_inline(struct box *box, int x, int y,
		float scale, colour colour, float ratio);
static bool html_redraw_text_decoration_block(struct box *box, int x, int y,
		float scale, colour colour, float ratio);

bool html_redraw_debug = false;

/**
 * Draw a CONTENT_HTML using the current set of plotters (plot).
 *
 * \param  c		     content of type CONTENT_HTML
 * \param  x		     coordinate for top-left of redraw
 * \param  y		     coordinate for top-left of redraw
 * \param  width	     available width (not used for HTML redraw)
 * \param  height	     available height (not used for HTML redraw)
 * \param  clip_x0	     clip rectangle
 * \param  clip_y0	     clip rectangle
 * \param  clip_x1	     clip rectangle
 * \param  clip_y1	     clip rectangle
 * \param  scale	     scale for redraw
 * \param  background_colour the background colour
 * \return true if successful, false otherwise
 *
 * x, y, clip_[xy][01] are in target coordinates.
 */

bool html_redraw(struct content *c, int x, int y,
		int width, int height,
		int clip_x0, int clip_y0, int clip_x1, int clip_y1,
		float scale, colour background_colour)
{
	struct box *box;
	bool result = true, want_knockout;
	bool select, select_only;
	plot_style_t pstyle_fill_bg = {
                0,
                0,
                0,
                PLOT_OP_TYPE_SOLID,
                0
	};

	pstyle_fill_bg.fill_colour = background_colour;
	box = c->data.html.layout;
	assert(box);

	want_knockout = plot.option_knockout;
	if (want_knockout)
		knockout_plot_start(&plot);

	/* The select menu needs special treating because, when opened, it
	 * reaches beyond its layout box.
	 */
	select = false;
	select_only = false;
	if (current_redraw_browser != NULL &&
			current_redraw_browser->visible_select_menu != NULL) {
		struct form_control *control =
				current_redraw_browser->visible_select_menu;
		select = true;
		/* check if the redraw rectangle is completely inside of the
		   select menu */
		select_only = form_clip_inside_select_menu(control, scale,
				clip_x0, clip_y0, clip_x1, clip_y1);
	}
	
	if (!select_only) {
		struct rect clip;
		clip.x0 = clip_x0;
		clip.y0 = clip_y0;
		clip.x1 = clip_x1;
		clip.y1 = clip_y1;
		/* clear to background colour */
		result = plot.clip(clip_x0, clip_y0, clip_x1, clip_y1);
	
		if (c->data.html.background_colour != NS_TRANSPARENT)
			pstyle_fill_bg.fill_colour =
					c->data.html.background_colour;
	
		result &= plot.rectangle(clip_x0, clip_y0, clip_x1, clip_y1,
				&pstyle_fill_bg);
	
		result &= html_redraw_box(box, x, y, clip,
				scale, pstyle_fill_bg.fill_colour);
	}

	if (select) {
		int menu_x, menu_y;
		box = current_redraw_browser->visible_select_menu->box;
		box_coords(box, &menu_x, &menu_y);
	
		menu_x -= box->border[LEFT].width;
		menu_y += box->height + box->border[BOTTOM].width +
				box->padding[BOTTOM] + box->padding[TOP];
		result &= form_redraw_select_menu(
				current_redraw_browser->visible_select_menu,
				x + menu_x, y + menu_y,
    				current_redraw_browser->scale,
				clip_x0, clip_y0, clip_x1, clip_y1);
	}
	
	if (want_knockout)
		knockout_plot_end();

	return result;

}


/**
 * Recursively draw a box.
 *
 * \param  box	     box to draw
 * \param  x_parent  coordinate of parent box
 * \param  y_parent  coordinate of parent box
 * \param  clip      clip rectangle
 * \param  scale     scale for redraw
 * \param  current_background_color  background colour under this box
 * \param  inline_depth  depth of nested inlines inside an inline container
 * \return true if successful, false otherwise
 *
 * x, y, clip_[xy][01] are in target coordinates.
 */

bool html_redraw_box(struct box *box, int x_parent, int y_parent,
		struct rect clip, float scale, colour current_background_color)
{
	int x, y;
	int width, height;
	int padding_left, padding_top, padding_width, padding_height;
	int border_left, border_top, border_right, border_bottom;
	struct rect r;
	int x_scrolled, y_scrolled;
	struct box *bg_box = NULL;
	css_color bgcol = 0;
	bool has_x_scroll, has_y_scroll;

	if (html_redraw_printing && box->printed)
		return true;

	/* avoid trivial FP maths */
	if (scale == 1.0) {
		x = x_parent + box->x;
		y = y_parent + box->y;
		width = box->width;
		height = box->height;
		padding_left = box->padding[LEFT];
		padding_top = box->padding[TOP];
		padding_width = padding_left + box->width + box->padding[RIGHT];
		padding_height = padding_top + box->height +
				box->padding[BOTTOM];
		border_left = box->border[LEFT].width;
		border_top = box->border[TOP].width;
		border_right = box->border[RIGHT].width;
		border_bottom = box->border[BOTTOM].width;
	} else {
		x = (x_parent + box->x) * scale;
		y = (y_parent + box->y) * scale;
		width = box->width * scale;
		height = box->height * scale;
		/* left and top padding values are normally zero,
		 * so avoid trivial FP maths */
		padding_left = box->padding[LEFT] ? box->padding[LEFT] * scale
				: 0;
		padding_top = box->padding[TOP] ? box->padding[TOP] * scale
				: 0;
		padding_width = (box->padding[LEFT] + box->width +
				box->padding[RIGHT]) * scale;
		padding_height = (box->padding[TOP] + box->height +
				box->padding[BOTTOM]) * scale;
		border_left = box->border[LEFT].width * scale;
		border_top = box->border[TOP].width * scale;
		border_right = box->border[RIGHT].width * scale;
		border_bottom = box->border[BOTTOM].width * scale;
	}

	/* calculate rectangle covering this box and descendants */
	if (box->style && css_computed_overflow(box->style) != 
			CSS_OVERFLOW_VISIBLE) {
		/* box contents clipped to box size */
		r.x0 = x - border_left;
		r.y0 = y - border_top;
		r.x1 = x + padding_width + border_right;
		r.y1 = y + padding_height + border_bottom;
	} else {
		/* box contents can hang out of the box; use descendant box */
		if (scale == 1.0) {
			r.x0 = x + box->descendant_x0;
			r.y0 = y + box->descendant_y0;
			r.x1 = x + box->descendant_x1 + 1;
			r.y1 = y + box->descendant_y1 + 1;
		} else {
			r.x0 = x + box->descendant_x0 * scale;
			r.y0 = y + box->descendant_y0 * scale;
			r.x1 = x + box->descendant_x1 * scale + 1;
			r.y1 = y + box->descendant_y1 * scale + 1;
		}
		if (!box->parent) {
			/* root element */
			int margin_left, margin_right;
			int margin_top, margin_bottom;
			if (scale == 1.0) {
				margin_left = box->margin[LEFT];
				margin_top = box->margin[TOP];
				margin_right = box->margin[RIGHT];
				margin_bottom = box->margin[BOTTOM];
			} else {
				margin_left = box->margin[LEFT] * scale;
				margin_top = box->margin[TOP] * scale;
				margin_right = box->margin[RIGHT] * scale;
				margin_bottom = box->margin[BOTTOM] * scale;
			}
			r.x0 = x - border_left - margin_left < r.x0 ?
					x - border_left - margin_left : r.x0;
			r.y0 = y - border_top - margin_top < r.y0 ?
					y - border_top - margin_top : r.y0;
			r.x1 = x + padding_width + border_right +
					margin_right > r.x1 ?
					x + padding_width + border_right +
					margin_right : r.x1;
			r.y1 = y + padding_height + border_bottom +
					margin_bottom > r.y1 ?
					y + padding_height + border_bottom +
					margin_bottom : r.y1;
		}
	}

	/* return if the rectangle is completely outside the clip rectangle */
	if (clip.y1 < r.y0 || r.y1 < clip.y0 ||
			clip.x1 < r.x0 || r.x1 < clip.x0)
		return true;

	/*if the rectangle is under the page bottom but it can fit in a page,
	don't print it now*/
	if (html_redraw_printing) {
		if (r.y1 > html_redraw_printing_border) {
			if (r.y1 - r.y0 <= html_redraw_printing_border &&
					(box->type == BOX_TEXT ||
					box->type == BOX_TABLE_CELL
					|| box->object || box->gadget)) {
				/*remember the highest of all points from the
				not printed elements*/
				if (r.y0 < html_redraw_printing_top_cropped)
					html_redraw_printing_top_cropped = r.y0;
				return true;
			}
		}
		else box->printed = true;/*it won't be printed anymore*/
	}

	/* if visibility is hidden render children only */
	if (box->style && css_computed_visibility(box->style) == 
			CSS_VISIBILITY_HIDDEN) {
		if ((plot.group_start) && (!plot.group_start("hidden box")))
			return false;
		if (!html_redraw_box_children(box, x_parent, y_parent,
				r, scale, current_background_color))
			return false;
		return ((!plot.group_end) || (plot.group_end()));
	}

	if ((plot.group_start) && (!plot.group_start("vis box")))
		return false;

	if (box->type == BOX_BLOCK || box->type == BOX_INLINE_BLOCK ||
			box->type == BOX_TABLE_CELL || box->object) {
		/* find intersection of clip rectangle and box */
		if (r.x0 < clip.x0) r.x0 = clip.x0;
		if (r.y0 < clip.y0) r.y0 = clip.y0;
		if (clip.x1 < r.x1) r.x1 = clip.x1;
		if (clip.y1 < r.y1) r.y1 = clip.y1;
		/* no point trying to draw 0-width/height boxes */
		if (r.x0 == r.x1 || r.y0 == r.y1)
			/* not an error */
			return ((!plot.group_end) || (plot.group_end()));
		/* clip to it */
		if (!plot.clip(r.x0, r.y0, r.x1, r.y1))
			return false;
	} else {
		/* clip box unchanged */
		r.x0 = clip.x0;
		r.y0 = clip.y0;
		r.x1 = clip.x1;
		r.y1 = clip.y1;
	}

	/* background colour and image for block level content and replaced
	 * inlines */

	/* Thanks to backwards compatibility, CSS defines the following:
	 *
	 * + If the box is for the root element and it has a background,
	 *   use that (and then process the body box with no special case)
	 * + If the box is for the root element and it has no background,
	 *   then use the background (if any) from the body element as if
	 *   it were specified on the root. Then, when the box for the body
	 *   element is processed, ignore the background.
	 * + For any other box, just use its own styling.
	 */
	if (!box->parent) {
		/* Root box */
		if (box->style && (css_computed_background_color(box->style,
				&bgcol) != CSS_BACKGROUND_COLOR_TRANSPARENT ||
				box->background)) {
			/* With its own background */
			bg_box = box;
		} else if (!box->style ||
				(css_computed_background_color(box->style,
				&bgcol) == CSS_BACKGROUND_COLOR_TRANSPARENT &&
				!box->background)) {
			/* Without its own background */
			if (box->children && box->children->style &&
					(css_computed_background_color(
					box->children->style, 
					&bgcol) != 
					CSS_BACKGROUND_COLOR_TRANSPARENT ||
					box->children->background)) {
				/* But body has one, so use that */
				bg_box = box->children;
			}
		}
	} else if (box->parent && !box->parent->parent) {
		/* Body box */
		if (box->style && (css_computed_background_color(box->style, 
				&bgcol) != CSS_BACKGROUND_COLOR_TRANSPARENT ||
				box->background)) {
			/* With a background */
			if (box->parent->style &&
				(css_computed_background_color(
					box->parent->style, &bgcol) !=
					CSS_BACKGROUND_COLOR_TRANSPARENT ||
					box->parent->background)) {
				/* Root has own background; process normally */
				bg_box = box;
			}
		}
	} else {
		/* Any other box */
		bg_box = box;
	}

	/* bg_box == NULL implies that this box should not have
	* its background rendered. Otherwise filter out linebreaks,
	* optimize away non-differing inlines, only plot background
	* for BOX_TEXT it's in an inline and ensure the bg_box
	* has something worth rendering */
	if (bg_box && bg_box->style &&
			bg_box->type != BOX_BR &&
			bg_box->type != BOX_TEXT &&
			bg_box->type != BOX_INLINE_END &&
			(bg_box->type != BOX_INLINE || bg_box->object) &&
			(css_computed_background_color(bg_box->style, 
			&bgcol) != CSS_BACKGROUND_COLOR_TRANSPARENT ||
			bg_box->background)) {
		/* find intersection of clip box and border edge */
		struct rect p;
		p.x0 = x - border_left < r.x0 ? r.x0 : x - border_left;
		p.y0 = y - border_top < r.y0 ? r.y0 : y - border_top;
		p.x1 = x + padding_width + border_right < r.x1 ?
				x + padding_width + border_right : r.x1;
		p.y1 = y + padding_height + border_bottom < r.y1 ?
				y + padding_height + border_bottom : r.y1;
		if (!box->parent) {
			/* Root element, special case:
			 * background covers margins too */
			int m_left, m_top, m_right, m_bottom;
			if (scale == 1.0) {
				m_left = box->margin[LEFT];
				m_top = box->margin[TOP];
				m_right = box->margin[RIGHT];
				m_bottom = box->margin[BOTTOM];
			} else {
				m_left = box->margin[LEFT] * scale;
				m_top = box->margin[TOP] * scale;
				m_right = box->margin[RIGHT] * scale;
				m_bottom = box->margin[BOTTOM] * scale;
			}
			p.x0 = p.x0 - m_left < r.x0 ? r.x0 : p.x0 - m_left;
			p.y0 = p.y0 - m_top < r.y0 ? r.y0 : p.y0 - m_top;
			p.x1 = p.x1 + m_right < r.x1 ? p.x1 + m_right : r.x1;
			p.y1 = p.y1 + m_bottom < r.y1 ? p.y1 + m_bottom : r.y1;
		}
		/* valid clipping rectangles only */
		if ((p.x0 < p.x1) && (p.y0 < p.y1)) {
			/* plot background */
			if (!html_redraw_background(x, y, box, scale, p,
					&current_background_color, bg_box))
				return false;
			/* restore previous graphics window */
			if (!plot.clip(r.x0, r.y0, r.x1, r.y1))
				return false;
		}
	}

	/* borders for block level content and replaced inlines */
	if (box->style && box->type != BOX_TEXT &&
			box->type != BOX_INLINE_END &&
			(box->type != BOX_INLINE || box->object) &&
			(border_top || border_right ||
			 border_bottom || border_left)) {
		if (!html_redraw_borders(box, x_parent, y_parent,
				padding_width, padding_height, r,
				scale))
			return false;
	}

	/* backgrounds and borders for non-replaced inlines */
	if (box->style && box->type == BOX_INLINE && box->inline_end &&
			(css_computed_background_color(box->style, &bgcol) != 
			CSS_BACKGROUND_COLOR_TRANSPARENT ||
			box->background || border_top || border_right ||
			border_bottom || border_left)) {
		/* inline backgrounds and borders span other boxes and may
		 * wrap onto separate lines */
		struct box *ib;
		struct rect b; /* border edge rectangle */
		struct rect p; /* clipped rect */
		bool first = true;
		int ib_x;
		int ib_y = y;
		int ib_p_width;
		int ib_b_left, ib_b_right;

		b.x0 = x - border_left;
		b.x1 = x + padding_width + border_right;
		b.y0 = y - border_top;
		b.y1 = y + padding_height + border_bottom;

		p.x0 = b.x0 < r.x0 ? r.x0 : b.x0;
		p.x1 = b.x1 < r.x1 ? b.x1 : r.x1;
		p.y0 = b.y0 < r.y0 ? r.y0 : b.y0;
		p.y1 = b.y1 < r.y1 ? b.y1 : r.y1;
		for (ib = box; ib; ib = ib->next) {
			/* to get extents of rectangle(s) associated with
			 * inline, cycle though all boxes in inline, skipping
			 * over floats */
			if (ib->type == BOX_FLOAT_LEFT ||
					ib->type == BOX_FLOAT_RIGHT)
				continue;
			if (scale == 1.0) {
				ib_x = x_parent + ib->x;
				ib_y = y_parent + ib->y;
				ib_p_width = ib->padding[LEFT] + ib->width +
						ib->padding[RIGHT];
				ib_b_left = ib->border[LEFT].width;
				ib_b_right = ib->border[RIGHT].width;
			} else {
				ib_x = (x_parent + ib->x) * scale;
				ib_y = (y_parent + ib->y) * scale;
				ib_p_width = (ib->padding[LEFT] + ib->width +
						ib->padding[RIGHT]) * scale;
				ib_b_left = ib->border[LEFT].width * scale;
				ib_b_right = ib->border[RIGHT].width * scale;
			}

			if (ib->inline_new_line && ib != box) {
				/* inline element has wrapped, plot background
				 * and borders */
				if (!html_redraw_inline_background(
						x, y, box, scale, p, b,
						first, false,
						&current_background_color))
					return false;
				/* restore previous graphics window */
				if (!plot.clip(r.x0, r.y0, r.x1, r.y1))
					return false;
				if (!html_redraw_inline_borders(box, b, r,
						scale, first, false))
					return false;
				/* reset coords */
				b.x0 = ib_x - ib_b_left;
				b.y0 = ib_y - border_top - padding_top;
				b.y1 = ib_y + padding_height - padding_top +
						border_bottom;

				p.x0 = b.x0 < r.x0 ? r.x0 : b.x0;
				p.y0 = b.y0 < r.y0 ? r.y0 : b.y0;
				p.y1 = b.y1 < r.y1 ? b.y1 : r.y1;

				first = false;
			}

			/* increase width for current box */
			b.x1 = ib_x + ib_p_width + ib_b_right;
			p.x1 = b.x1 < r.x1 ? b.x1 : r.x1;

			if (ib == box->inline_end)
				/* reached end of BOX_INLINE span */
				break;
		}
		/* plot background and borders for last rectangle of
		 * the inline */
		if (!html_redraw_inline_background(x, ib_y, box, scale, p, b,
				first, true, &current_background_color))
			return false;
		/* restore previous graphics window */
		if (!plot.clip(r.x0, r.y0, r.x1, r.y1))
			return false;
		if (!html_redraw_inline_borders(box, b, r, scale, first, true))
			return false;

	}

	/* Debug outlines */
	if (html_redraw_debug) {
		int margin_left, margin_right;
		int margin_top, margin_bottom;
		if (scale == 1.0) {
			/* avoid trivial fp maths */
			margin_left = box->margin[LEFT];
			margin_top = box->margin[TOP];
			margin_right = box->margin[RIGHT];
			margin_bottom = box->margin[BOTTOM];
		} else {
			margin_left = box->margin[LEFT] * scale;
			margin_top = box->margin[TOP] * scale;
			margin_right = box->margin[RIGHT] * scale;
			margin_bottom = box->margin[BOTTOM] * scale;
		}
		/* Content edge -- blue */
		if (!plot.rectangle(x + padding_left,
				y + padding_top,
				x + padding_left + width,
				y + padding_top + height,
				plot_style_content_edge))
			return false;
		/* Padding edge -- red */
		if (!plot.rectangle(x, y,
				x + padding_width, y + padding_height,
				plot_style_padding_edge))
			return false;
		/* Margin edge -- yellow */
		if (!plot.rectangle(
				x - border_left - margin_left,
				y - border_top - margin_top,
				x + padding_width + border_right +
						margin_right,
				y + padding_height + border_bottom +
						margin_bottom,
				plot_style_margin_edge))
			return false;
	}

	/* clip to the padding edge for objects, or boxes with overflow hidden
	 * or scroll */
	if (box->object || (box->style && css_computed_overflow(box->style) != 
			CSS_OVERFLOW_VISIBLE)) {
		r.x0 = x;
		r.y0 = y;
		r.x1 = x + padding_width;
		r.y1 = y + padding_height;
		if (r.x0 < clip.x0) r.x0 = clip.x0;
		if (r.y0 < clip.y0) r.y0 = clip.y0;
		if (clip.x1 < r.x1) r.x1 = clip.x1;
		if (clip.y1 < r.y1) r.y1 = clip.y1;
		if (r.x1 <= r.x0 || r.y1 <= r.y0)
			return ((!plot.group_end) || (plot.group_end()));
		if (box->type == BOX_BLOCK || box->type == BOX_INLINE_BLOCK ||
				box->type == BOX_TABLE_CELL || box->object) {
			if (!plot.clip(r.x0, r.y0, r.x1, r.y1))
				return false;
		}
	}

	/* text decoration */
	if (box->type != BOX_TEXT && box->style &&
			css_computed_text_decoration(box->style) !=
			CSS_TEXT_DECORATION_NONE)
		if (!html_redraw_text_decoration(box, x_parent, y_parent,
				scale, current_background_color))
			return false;

	if (box->object && width != 0 && height != 0) {
		x_scrolled = x - scroll_get_offset(box->scroll_x) * scale;
		y_scrolled = y - scroll_get_offset(box->scroll_y) * scale;
		if (!content_redraw(box->object,
				x_scrolled + padding_left,
				y_scrolled + padding_top,
				width, height, r.x0, r.y0, r.x1, r.y1, scale,
				current_background_color))
			return false;

	} else if (box->gadget && box->gadget->type == GADGET_CHECKBOX) {
		if (!html_redraw_checkbox(x + padding_left, y + padding_top,
				width, height,
				box->gadget->selected))
			return false;

	} else if (box->gadget && box->gadget->type == GADGET_RADIO) {
		if (!html_redraw_radio(x + padding_left, y + padding_top,
				width, height,
				box->gadget->selected))
			return false;

	} else if (box->gadget && box->gadget->type == GADGET_FILE) {
		if (!html_redraw_file(x + padding_left, y + padding_top,
				width, height, box, scale,
				current_background_color))
			return false;

	} else if (box->text) {
		if (!html_redraw_text_box(box, x, y, r, scale,
				current_background_color))
			return false;

	} else {
		if (!html_redraw_box_children(box, x_parent, y_parent, r,
				scale, current_background_color))
			return false;
	}

	/* list marker */
	if (box->list_marker)
		if (!html_redraw_box(box->list_marker,
				x_parent + box->x -
				scroll_get_offset(box->scroll_x),
				y_parent + box->y -
				scroll_get_offset(box->scroll_y),
				clip, scale, current_background_color))
			return false;

	/* scrollbars */
	if (current_redraw_browser && ((box->style && box->type != BOX_BR &&
			box->type != BOX_TABLE && box->type != BOX_INLINE &&
			(css_computed_overflow(box->style) == 
				CSS_OVERFLOW_SCROLL ||
			css_computed_overflow(box->style) == 
				CSS_OVERFLOW_AUTO)) || (box->object &&
			content_get_type(box->object) == CONTENT_HTML))) {

		has_x_scroll = box_hscrollbar_present(box);
		has_y_scroll = box_vscrollbar_present(box);

		if (!box_handle_scrollbars(current_redraw_browser, box,
				has_x_scroll, has_y_scroll))
			return false;
		
		if (box->scroll_x != NULL)
			scroll_redraw(box->scroll_x,
					x_parent + box->x,
     					y_parent + box->y + box->padding[TOP] +
					box->height + box->padding[BOTTOM] -
					SCROLLBAR_WIDTH,
					clip.x0, clip.y0, clip.x1, clip.y1,
     					scale);
		if (box->scroll_y != NULL)
			scroll_redraw(box->scroll_y,
					x_parent + box->x + box->padding[LEFT] +
					box->width + box->padding[RIGHT] -
					SCROLLBAR_WIDTH,
					y_parent + box->y,
					clip.x0, clip.y0, clip.x1, clip.y1,
     					scale);
	}


	if (box->type == BOX_BLOCK || box->type == BOX_INLINE_BLOCK ||
			box->type == BOX_TABLE_CELL || box->object)
		if (!plot.clip(clip.x0, clip.y0, clip.x1, clip.y1))
			return false;

	return ((!plot.group_end) || (plot.group_end()));
}


/**
 * Draw the various children of a box.
 *
 * \param  box	     box to draw children of
 * \param  x_parent  coordinate of parent box
 * \param  y_parent  coordinate of parent box
 * \param  clip      clip rectangle
 * \param  scale     scale for redraw
 * \param  current_background_color  background colour under this box
 * \return true if successful, false otherwise
 */

bool html_redraw_box_children(struct box *box, int x_parent, int y_parent,
		struct rect clip, float scale,
		colour current_background_color)
{
	struct box *c;

	for (c = box->children; c; c = c->next) {

		if (c->type != BOX_FLOAT_LEFT && c->type != BOX_FLOAT_RIGHT)
			if (!html_redraw_box(c,
					x_parent + box->x -
					scroll_get_offset(box->scroll_x),
					y_parent + box->y -
					scroll_get_offset(box->scroll_y),
					clip, scale, current_background_color))
				return false;
	}
	for (c = box->float_children; c; c = c->next_float)
		if (!html_redraw_box(c,
				x_parent + box->x -
				scroll_get_offset(box->scroll_x),
				y_parent + box->y -
				scroll_get_offset(box->scroll_y),
				clip, scale, current_background_color))
			return false;

	return true;
}


/**
 * Redraw the text content of a box, possibly partially highlighted
 * because the text has been selected, or matches a search operation.
 *
 * \param  box      box with text content
 * \param  x        x co-ord of box
 * \param  y        y co-ord of box
 * \param  clip     current clip rectangle
 * \param  scale    current scale setting (1.0 = 100%)
 * \param  current_background_color
 * \return true iff successful and redraw should proceed
 */

bool html_redraw_text_box(struct box *box, int x, int y,
		struct rect clip, float scale,
		colour current_background_color)
{
	bool excluded = (box->object != NULL);
	plot_font_style_t fstyle;

	font_plot_style_from_css(box->style, &fstyle);
	fstyle.background = current_background_color;

	if (!text_redraw(box->text, box->length, box->byte_offset,
			box->space, &fstyle, x, y,
			&clip, box->height, scale, excluded))
		return false;

	/* does this textbox contain the ghost caret? */
	if (ghost_caret.defined && box == ghost_caret.text_box) {

		if (!html_redraw_caret(&ghost_caret, current_background_color,
				scale))
			return false;
	}
	return true;
}

/**
 * Redraw a short text string, complete with highlighting
 * (for selection/search) and ghost caret
 *
 * \param  utf8_text  pointer to UTF-8 text string
 * \param  utf8_len   length of string, in bytes
 * \param  offset     byte offset within textual representation
 * \param  space      indicates whether string is followed by a space
 * \param  fstyle     text style to use
 * \param  x          x ordinate at which to plot text
 * \param  y          y ordinate at which to plot text
 * \param  clip       pointer to current clip rectangle
 * \param  height     height of text string
 * \param  scale      current display scale (1.0 = 100%)
 * \param  excluded   exclude this text string from the selection
 * \return true iff successful and redraw should proceed
 */

bool text_redraw(const char *utf8_text, size_t utf8_len,
		size_t offset, bool space, const plot_font_style_t *fstyle,
		int x, int y, struct rect *clip,
		int height,
		float scale,
		bool excluded)
{
	bool highlighted = false;

	/* is this box part of a selection? */
	if (!excluded && current_redraw_browser) {
		unsigned len = utf8_len + (space ? 1 : 0);
		unsigned start_idx;
		unsigned end_idx;

		/* first try the browser window's current selection */
		if (selection_defined(current_redraw_browser->sel) &&
			selection_highlighted(current_redraw_browser->sel,
				offset, offset + len, &start_idx, &end_idx)) {
			highlighted = true;
		}

		/* what about the current search operation, if any? */
		if (!highlighted && (current_redraw_browser->search_context
				!= NULL) && 
				gui_search_term_highlighted(
						current_redraw_browser->window,
						offset, offset + len,
						&start_idx, &end_idx,
						current_redraw_browser->
						search_context)) {
			highlighted = true;
		}

		/* \todo make search terms visible within selected text */
		if (highlighted) {
			unsigned endtxt_idx = end_idx;
			bool clip_changed = false;
			bool text_visible = true;
			int startx, endx;
			plot_style_t *pstyle_fill_hback = plot_style_fill_white;
			plot_font_style_t fstyle_hback = *fstyle;

			if (end_idx > utf8_len) {
				/* adjust for trailing space, not present in
				 * utf8_text */
				assert(end_idx == utf8_len + 1);
				endtxt_idx = utf8_len;
			}

			if (!nsfont.font_width(fstyle, utf8_text, start_idx,
					&startx))
				startx = 0;

			if (!nsfont.font_width(fstyle, utf8_text, endtxt_idx,
					&endx))
				endx = 0;

			/* is there a trailing space that should be highlighted
			 * as well? */
			if (end_idx > utf8_len) {
				int spc_width;
				/* \todo is there a more elegant/efficient
				 * solution? */
				if (nsfont.font_width(fstyle, " ", 1,
						&spc_width))
					endx += spc_width;
			}

			if (scale != 1.0) {
				startx *= scale;
				endx *= scale;
			}

			/* draw any text preceding highlighted portion */
			if (start_idx > 0 &&
				!plot.text(x, y + (int) (height * 0.75 * scale),
						utf8_text, start_idx,
						fstyle))
				return false;

			/* decide whether highlighted portion is to be
			 * white-on-black or black-on-white */
			if ((fstyle->background & 0x808080) == 0x808080)
				pstyle_fill_hback = plot_style_fill_black;

			/* highlighted portion */
			if (!plot.rectangle(x + startx, y, x + endx,
					y + height * scale,
					pstyle_fill_hback))
				return false;

			if (start_idx > 0) {
				int px0 = max(x + startx, clip->x0);
				int px1 = min(x + endx, clip->x1);

				if (px0 < px1) {
					if (!plot.clip(px0, clip->y0, px1,
							clip->y1))
						return false;
					clip_changed = true;
				} else {
					text_visible = false;
				}
			}

			fstyle_hback.background =
				pstyle_fill_hback->fill_colour;
			fstyle_hback.foreground =
				pstyle_fill_hback->fill_colour ^ 0xffffff;

			if (text_visible &&
				!plot.text(x, y + (int) (height * 0.75 * scale),
						utf8_text, endtxt_idx,
						&fstyle_hback))
				return false;

			/* draw any text succeeding highlighted portion */
			if (endtxt_idx < utf8_len) {
				int px0 = max(x + endx, clip->x0);
				if (px0 < clip->x1) {

					if (!plot.clip(px0, clip->y0,
							clip->x1, clip->y1))
						return false;

					clip_changed = true;

					if (!plot.text(x, y + (int)
						(height * 0.75 * scale),
						utf8_text, utf8_len,
						fstyle))
						return false;
				}
			}

			if (clip_changed &&
				!plot.clip(clip->x0, clip->y0,
						clip->x1, clip->y1))
				return false;
		}
	}

	if (!highlighted) {
		if (!plot.text(x, y + (int) (height * 0.75 * scale),
				utf8_text, utf8_len,
				fstyle))
			return false;
	}
	return true;
}


/**
 * Draw text caret.
 *
 * \param  c	  structure describing text caret
 * \param  current_background_color	background colour under the caret
 * \param  scale  current scale setting (1.0 = 100%)
 * \return true iff successful and redraw should proceed
 */

bool html_redraw_caret(struct caret *c, colour current_background_color,
		float scale)
{
	int xc = c->x, y = c->y;
	int h = c->height - 1;
	int w = (h + 7) / 8;

	return (plot.line(xc * scale, y * scale,
				xc * scale, (y + h) * scale,
				plot_style_caret) &&
			plot.line((xc - w) * scale, y * scale,
				(xc + w) * scale, y * scale,
				plot_style_caret) &&
			plot.line((xc - w) * scale, (y + h) * scale,
				(xc + w) * scale, (y + h) * scale,
				plot_style_caret));
}


/**
 * Draw borders for a box.
 *
 * \param  box		box to draw
 * \param  x_parent	coordinate of left padding edge of parent of box
 * \param  y_parent	coordinate of top padding edge of parent of box
 * \param  p_width	width of padding box
 * \param  p_height	height of padding box
 * \param  scale	scale for redraw
 * \return true if successful, false otherwise
 */

bool html_redraw_borders(struct box *box, int x_parent, int y_parent,
		int p_width, int p_height, struct rect clip, float scale)
{
	unsigned int sides[] = { LEFT, RIGHT, TOP, BOTTOM };
	int top = box->border[TOP].width;
	int right = box->border[RIGHT].width;
	int bottom = box->border[BOTTOM].width;
	int left = box->border[LEFT].width;
	int x, y;
	unsigned int i, side;
	int p[8]; /* Box border vertices */
	int z[8]; /* Border vertices */
	bool square_end_1 = false;
	bool square_end_2 = false;

	x = x_parent + box->x;
	y = y_parent + box->y;

	if (scale != 1.0) {
		top *= scale;
		right *= scale;
		bottom *= scale;
		left *= scale;
		x *= scale;
		y *= scale;
	}

	assert(box->style);

	/* Calculate border vertices
	 *
	 *    A----------------------+
	 *    | \                  / |
	 *    |   B--------------+   |
	 *    |   |              |   |
	 *    |   +--------------C   |
	 *    | /                  \ |
	 *    +----------------------D
	 */
	p[0] = x - left;		p[1] = y - top;			/* A */
	p[2] = x;			p[3] = y;			/* B */
	p[4] = x + p_width;		p[5] = y + p_height;		/* C */
	p[6] = x + p_width + right;	p[7] = y + p_height + bottom;	/* D */

	for (i = 0; i != 4; i++) {
		colour col = 0;
		side = sides[i]; /* plot order */

		if (box->border[side].width == 0 || box->border[side].color ==
				CSS_BORDER_COLOR_TRANSPARENT)
			continue;

		switch (side) {
		case LEFT:
			square_end_1 = (top == 0);
			square_end_2 = (bottom == 0);

			z[0] = p[0];	z[1] = p[7];
			z[2] = p[2];	z[3] = p[5];
			z[4] = p[2];	z[5] = p[3];
			z[6] = p[0];	z[7] = p[1];

			if (box->border[TOP].color !=
					CSS_BORDER_COLOR_TRANSPARENT &&
					box->border[TOP].style !=
					CSS_BORDER_STYLE_DOUBLE) {
				/* make border overhang top corner fully,
				 * if top border is opaque */
				z[5] -= top;
				square_end_1 = true;
			}
			if (box->border[BOTTOM].color !=
					CSS_BORDER_COLOR_TRANSPARENT &&
					box->border[BOTTOM].style !=
					CSS_BORDER_STYLE_DOUBLE) {
				/* make border overhang bottom corner fully,
				 * if bottom border is opaque */
				z[3] += bottom;
				square_end_2 = true;
			}

			col = nscss_color_to_ns(box->border[side].c);

			if (!html_redraw_border_plot(side, z, col,
					box->border[side].style,
					box->border[side].width * scale,
					square_end_1 && square_end_2, clip))
				return false;
			break;
		case RIGHT:
			square_end_1 = (top == 0);
			square_end_2 = (bottom == 0);

			z[0] = p[6];	z[1] = p[1];
			z[2] = p[4];	z[3] = p[3];
			z[4] = p[4];	z[5] = p[5];
			z[6] = p[6];	z[7] = p[7];

			if (box->border[TOP].color !=
					CSS_BORDER_COLOR_TRANSPARENT &&
					box->border[TOP].style !=
					CSS_BORDER_STYLE_DOUBLE) {
				/* make border overhang top corner fully,
				 * if top border is opaque */
				z[3] -= top;
				square_end_1 = true;
			}
			if (box->border[BOTTOM].color !=
					CSS_BORDER_COLOR_TRANSPARENT &&
					box->border[BOTTOM].style !=
					CSS_BORDER_STYLE_DOUBLE) {
				/* make border overhang bottom corner fully,
				 * if bottom border is opaque */
				z[5] += bottom;
				square_end_2 = true;
			}

			col = nscss_color_to_ns(box->border[side].c);

			if (!html_redraw_border_plot(side, z, col,
					box->border[side].style,
					box->border[side].width * scale,
					square_end_1 && square_end_2, clip))
				return false;
			break;
		case TOP:
			if (clip.y0 > p[3])
				/* clip rectangle is below border; nothing to
				 * plot */
				continue;

			square_end_1 = (left == 0);
			square_end_2 = (right == 0);

			z[0] = p[2];	z[1] = p[3];
			z[2] = p[0];	z[3] = p[1];
			z[4] = p[6];	z[5] = p[1];
			z[6] = p[4];	z[7] = p[3];

			if (box->border[TOP].style ==
					CSS_BORDER_STYLE_SOLID &&
					box->border[TOP].c ==
					box->border[LEFT].c) {
				/* don't bother overlapping left corner if
				 * it's the same colour anyway */
				z[2] += left;
				square_end_1 = true;
			}
			if (box->border[TOP].style ==
					CSS_BORDER_STYLE_SOLID &&
					box->border[TOP].c ==
					box->border[RIGHT].c) {
				/* don't bother overlapping right corner if
				 * it's the same colour anyway */
				z[4] -= right;
				square_end_2 = true;
			}

			col = nscss_color_to_ns(box->border[side].c);

			if (!html_redraw_border_plot(side, z, col,
					box->border[side].style,
					box->border[side].width * scale,
					square_end_1 && square_end_2, clip))
				return false;
			break;
		case BOTTOM:
			if (clip.y1 < p[5])
				/* clip rectangle is above border; nothing to
				 * plot */
				continue;

			square_end_1 = (left == 0);
			square_end_2 = (right == 0);

			z[0] = p[4];	z[1] = p[5];
			z[2] = p[6];	z[3] = p[7];
			z[4] = p[0];	z[5] = p[7];
			z[6] = p[2];	z[7] = p[5];

			if (box->border[BOTTOM].style ==
					CSS_BORDER_STYLE_SOLID &&
					box->border[BOTTOM].c ==
					box->border[LEFT].c) {
				/* don't bother overlapping left corner if
				 * it's the same colour anyway */
				z[4] += left;
				square_end_1 = true;
			}
			if (box->border[BOTTOM].style ==
					CSS_BORDER_STYLE_SOLID &&
					box->border[BOTTOM].c ==
					box->border[RIGHT].c) {
				/* don't bother overlapping right corner if
				 * it's the same colour anyway */
				z[2] -= right;
				square_end_2 = true;
			}

			col = nscss_color_to_ns(box->border[side].c);

			if (!html_redraw_border_plot(side, z, col,
					box->border[side].style,
					box->border[side].width * scale,
					square_end_1 && square_end_2, clip))
				return false;
			break;
		default:
			assert(side == TOP || side == BOTTOM ||
					side == LEFT || side == RIGHT);
			break;
		}
	}

	return true;
}


/**
 * Draw an inline's borders.
 *
 * \param  box	  BOX_INLINE which created the border
 * \param  b	  coordinates of border edge rectangle
 * \param  scale  scale for redraw
 * \param  first  true if this is the first rectangle associated with the inline
 * \param  last	  true if this is the last rectangle associated with the inline
 * \return true if successful, false otherwise
 */

bool html_redraw_inline_borders(struct box *box, struct rect b,
		struct rect clip, float scale, bool first, bool last)
{
	int top = box->border[TOP].width;
	int right = box->border[RIGHT].width;
	int bottom = box->border[BOTTOM].width;
	int left = box->border[LEFT].width;
	colour col;
	int p[8]; /* Box border vertices */
	int z[8]; /* Border vertices */
	bool square_end_1;
	bool square_end_2;

	if (scale != 1.0) {
		top *= scale;
		right *= scale;
		bottom *= scale;
		left *= scale;
	}

	/* Calculate border vertices
	 *
	 *    A----------------------+
	 *    | \                  / |
	 *    |   B--------------+   |
	 *    |   |              |   |
	 *    |   +--------------C   |
	 *    | /                  \ |
	 *    +----------------------D
	 */
	p[0] = b.x0;				p[1] = b.y0;		/* A */
	p[2] = first ? b.x0 + left : b.x0;	p[3] = b.y0 + top;	/* B */
	p[4] = last ? b.x1 - right : b.x1;	p[5] = b.y1 - bottom;	/* C */
	p[6] = b.x1;				p[7] = b.y1;		/* D */

	assert(box->style);

	/* Left */
	square_end_1 = (top == 0);
	square_end_2 = (bottom == 0);
	if (left != 0 && first && box->border[LEFT].color !=
			CSS_BORDER_COLOR_TRANSPARENT) {
		col = nscss_color_to_ns(box->border[LEFT].c);

		z[0] = p[0];	z[1] = p[7];
		z[2] = p[2];	z[3] = p[5];
		z[4] = p[2];	z[5] = p[3];
		z[6] = p[0];	z[7] = p[1];

		if (box->border[TOP].color !=
				CSS_BORDER_COLOR_TRANSPARENT &&
				box->border[TOP].style !=
				CSS_BORDER_STYLE_DOUBLE) {
			/* make border overhang top corner fully,
			 * if top border is opaque */
			z[5] -= top;
			square_end_1 = true;
		}
		if (box->border[BOTTOM].color !=
				CSS_BORDER_COLOR_TRANSPARENT &&
				box->border[BOTTOM].style !=
				CSS_BORDER_STYLE_DOUBLE) {
			/* make border overhang bottom corner fully,
			 * if bottom border is opaque */
			z[3] += bottom;
			square_end_2 = true;
		}

		if (!html_redraw_border_plot(LEFT, z, col,
				box->border[LEFT].style, 
				left, square_end_1 && square_end_2, clip))
			return false;
	}

	/* Right */
	square_end_1 = (top == 0);
	square_end_2 = (bottom == 0);
	if (right != 0 && last && box->border[RIGHT].color !=
			CSS_BORDER_COLOR_TRANSPARENT) {
		col = nscss_color_to_ns(box->border[RIGHT].c);

		z[0] = p[6];	z[1] = p[1];
		z[2] = p[4];	z[3] = p[3];
		z[4] = p[4];	z[5] = p[5];
		z[6] = p[6];	z[7] = p[7];

		if (box->border[TOP].color !=
				CSS_BORDER_COLOR_TRANSPARENT &&
				box->border[TOP].style !=
				CSS_BORDER_STYLE_DOUBLE) {
			/* make border overhang top corner fully,
			 * if top border is opaque */
			z[3] -= top;
			square_end_1 = true;
		}
		if (box->border[BOTTOM].color !=
				CSS_BORDER_COLOR_TRANSPARENT &&
				box->border[BOTTOM].style !=
				CSS_BORDER_STYLE_DOUBLE) {
			/* make border overhang bottom corner fully,
			 * if bottom border is opaque */
			z[5] += bottom;
			square_end_2 = true;
		}

		if (!html_redraw_border_plot(RIGHT, z, col,
				box->border[RIGHT].style,
				right, square_end_1 && square_end_2, clip))
			return false;
	}

	/* Top */
	square_end_1 = (left == 0);
	square_end_2 = (right == 0);
	if (top != 0 && box->border[TOP].color !=
			CSS_BORDER_COLOR_TRANSPARENT) {
		col = nscss_color_to_ns(box->border[TOP].c);

		z[0] = p[2];	z[1] = p[3];
		z[2] = p[0];	z[3] = p[1];
		z[4] = p[6];	z[5] = p[1];
		z[6] = p[4];	z[7] = p[3];

		if (first && box->border[TOP].style ==
				CSS_BORDER_STYLE_SOLID &&
				box->border[TOP].c ==
				box->border[LEFT].c) {
			/* don't bother overlapping left corner if
			 * it's the same colour anyway */
			z[2] += left;
			square_end_1 = true;
		}
		if (last && box->border[TOP].style ==
				CSS_BORDER_STYLE_SOLID &&
				box->border[TOP].c ==
				box->border[RIGHT].c) {
			/* don't bother overlapping right corner if
			 * it's the same colour anyway */
			z[4] -= right;
			square_end_2 = true;
		}

		if (!html_redraw_border_plot(TOP, z, col,
				box->border[TOP].style, 
				top, square_end_1 && square_end_2, clip))
			return false;
	}

	/* Bottom */
	square_end_1 = (left == 0);
	square_end_2 = (right == 0);
	if (bottom != 0 && box->border[BOTTOM].color !=
			CSS_BORDER_COLOR_TRANSPARENT) {
		col = nscss_color_to_ns(box->border[BOTTOM].c);

		z[0] = p[4];	z[1] = p[5];
		z[2] = p[6];	z[3] = p[7];
		z[4] = p[0];	z[5] = p[7];
		z[6] = p[2];	z[7] = p[5];

		if (first && box->border[BOTTOM].style ==
				CSS_BORDER_STYLE_SOLID &&
				box->border[BOTTOM].c ==
				box->border[LEFT].c) {
			/* don't bother overlapping left corner if
			 * it's the same colour anyway */
			z[4] += left;
			square_end_1 = true;
		}
		if (last && box->border[BOTTOM].style ==
				CSS_BORDER_STYLE_SOLID &&
				box->border[BOTTOM].c ==
				box->border[RIGHT].c) {
			/* don't bother overlapping right corner if
			 * it's the same colour anyway */
			z[2] -= right;
			square_end_2 = true;
		}

		if (!html_redraw_border_plot(BOTTOM, z, col,
				box->border[BOTTOM].style, 
				bottom, square_end_1 && square_end_2, clip))
			return false;
	}

	return true;
}

static plot_style_t plot_style_bdr = {
	PLOT_OP_TYPE_DASH,
	0,
	0,
	0,
	0
};
static plot_style_t plot_style_fillbdr = {
	0,
	0,
	0,
	PLOT_OP_TYPE_SOLID,
	0
};
static plot_style_t plot_style_fillbdr_dark = {
        0,
        0,
        0,
	PLOT_OP_TYPE_SOLID,
	0
};
static plot_style_t plot_style_fillbdr_light = {
        0,
        0,
        0,
	PLOT_OP_TYPE_SOLID,
	0
};
static plot_style_t plot_style_fillbdr_ddark = {
        0,
        0,
        0,
	PLOT_OP_TYPE_SOLID,
	0
};
static plot_style_t plot_style_fillbdr_dlight = {
        0,
        0,
        0,
	PLOT_OP_TYPE_SOLID,
	0
};

/**
 * Draw one border.
 *
 * \param  side         index of border side (TOP, RIGHT, BOTTOM, LEFT)
 * \param  p            array of precomputed border vertices
 * \param  c            colour for border
 * \param  style        border line style
 * \param  thickness    border thickness
 * \param  rectangular  whether border is rectangular
 * \return true if successful, false otherwise
 */

bool html_redraw_border_plot(const int side, const int *p, colour c,
		enum css_border_style_e style, int thickness, bool rectangular,
		struct rect clip)
{
	int z[8]; /* Vertices of border part */
	unsigned int light = side;
	plot_style_t *plot_style_bdr_in;
	plot_style_t *plot_style_bdr_out;

	if (c == NS_TRANSPARENT)
		return true;

	plot_style_bdr.stroke_type = PLOT_OP_TYPE_DASH;
	plot_style_bdr.stroke_colour = c;
	plot_style_bdr.stroke_width = thickness;
	plot_style_fillbdr.fill_colour = c;
	plot_style_fillbdr_dark.fill_colour = darken_colour(c);
	plot_style_fillbdr_light.fill_colour = lighten_colour(c);
	plot_style_fillbdr_ddark.fill_colour = double_darken_colour(c);
	plot_style_fillbdr_dlight.fill_colour = double_lighten_colour(c);

	switch (style) {
	case CSS_BORDER_STYLE_DOTTED:
		plot_style_bdr.stroke_type = PLOT_OP_TYPE_DOT;
		/* fall through */
	case CSS_BORDER_STYLE_DASHED:
		if (!plot.line((p[0] + p[2]) / 2,
				(p[1] + p[3]) / 2,
				(p[4] + p[6]) / 2,
				(p[5] + p[7]) / 2,
				&plot_style_bdr))
			return false;
		break;

	case CSS_BORDER_STYLE_SOLID:
		/* fall through to default */
	default:
		if (rectangular || thickness == 1) {
			int x0, y0, x1, y1;
			if (side == TOP || side == RIGHT) {
				x0 = p[2];	y0 = p[3];
				x1 = p[6];	y1 = p[7];
				x1 = ((side == TOP) && (p[4] - p[6] != 0)) ?
					x1 + p[4] - p[6] : x1;
			} else {
				x0 = p[6];	y0 = p[7];
				x1 = p[2];	y1 = p[3];
				y1 = ((side == LEFT) && (p[1] - p[3] != 0)) ?
					y1 + p[1] - p[3] : y1;
			}
			/* find intersection of clip rectangle and border */
			x0 = (clip.x0 > x0) ? clip.x0 : x0;
			y0 = (clip.y0 > y0) ? clip.y0 : y0;
			x1 = (clip.x1 < x1) ? clip.x1 : x1;
			y1 = (clip.y1 < y1) ? clip.y1 : y1;
			if ((x0 < x1) && (y0 < y1)) {
				/* valid clip rectangles only */
				if (!plot.rectangle(x0, y0, x1, y1,
						&plot_style_fillbdr))
					return false;
			}
		} else {
			if (!plot.polygon(p, 4, &plot_style_fillbdr))
				return false;
		}
		break;

	case CSS_BORDER_STYLE_DOUBLE:
		z[0] = p[0];
		z[1] = p[1];
		z[2] = (p[0] * 2 + p[2]) / 3;
		z[3] = (p[1] * 2 + p[3]) / 3;
		z[4] = (p[6] * 2 + p[4]) / 3;
		z[5] = (p[7] * 2 + p[5]) / 3;
		z[6] = p[6];
		z[7] = p[7];
		if (!plot.polygon(z, 4, &plot_style_fillbdr))
			return false;
		z[0] = p[2];
		z[1] = p[3];
		z[2] = (p[2] * 2 + p[0]) / 3;
		z[3] = (p[3] * 2 + p[1]) / 3;
		z[4] = (p[4] * 2 + p[6]) / 3;
		z[5] = (p[5] * 2 + p[7]) / 3;
		z[6] = p[4];
		z[7] = p[5];
		if (!plot.polygon(z, 4, &plot_style_fillbdr))
			return false;
		break;

	case CSS_BORDER_STYLE_GROOVE:
		light = 3 - light;
		/* fall through */
	case CSS_BORDER_STYLE_RIDGE:
		/* choose correct colours for each part of the border line */
		if (light <= 1) {
			plot_style_bdr_in = &plot_style_fillbdr_dark;
			plot_style_bdr_out = &plot_style_fillbdr_light;
		} else {
			plot_style_bdr_in = &plot_style_fillbdr_light;
			plot_style_bdr_out = &plot_style_fillbdr_dark;
		}

		/* Render border */
		if ((rectangular || thickness == 2) && thickness != 1) {
			/* Border made up from two parts and can be plotted
			 * with rectangles */
			int x0, y0, x1, y1;

			/* First part */
			if (side == TOP || side == RIGHT) {
				x0 = (p[0] + p[2]) / 2;	y0 = (p[1] + p[3]) / 2;
				x1 = p[6];		y1 = p[7];
			} else {
				x0 = p[6];		y0 = p[7];
				x1 = (p[0] + p[2]) / 2;	y1 = (p[1] + p[3]) / 2;
			}
			/* find intersection of clip rectangle and border */
			x0 = (clip.x0 > x0) ? clip.x0 : x0;
			y0 = (clip.y0 > y0) ? clip.y0 : y0;
			x1 = (clip.x1 < x1) ? clip.x1 : x1;
			y1 = (clip.y1 < y1) ? clip.y1 : y1;
			if ((x0 < x1) && (y0 < y1)) {
				/* valid clip rectangles only */
				if (!plot.rectangle(x0, y0, x1, y1,
						plot_style_bdr_in))
					return false;
			}

			/* Second part */
			if (side == TOP || side == RIGHT) {
				x0 = p[2];		y0 = p[3];
				x1 = (p[6] + p[4]) / 2;	y1 = (p[7] + p[5]) / 2;
			} else {
				x0 = (p[6] + p[4]) / 2;	y0 = (p[7] + p[5]) / 2;
				x1 = p[2];		y1 = p[3];
			}
			/* find intersection of clip rectangle and border */
			x0 = (clip.x0 > x0) ? clip.x0 : x0;
			y0 = (clip.y0 > y0) ? clip.y0 : y0;
			x1 = (clip.x1 < x1) ? clip.x1 : x1;
			y1 = (clip.y1 < y1) ? clip.y1 : y1;
			if ((x0 < x1) && (y0 < y1)) {
				/* valid clip rectangles only */
				if (!plot.rectangle(x0, y0, x1, y1,
						plot_style_bdr_out))
					return false;
			}
		} else if (thickness == 1) {
			/* Border made up from one part which can be plotted
			 * as a rectangle */
			int x0, y0, x1, y1;
			if (side == TOP || side == RIGHT) {
				x0 = p[2];	y0 = p[3];
				x1 = p[6];	y1 = p[7];
				x1 = ((side == TOP) && (p[4] - p[6] != 0)) ?
					x1 + p[4] - p[6] : x1;
				/* find intersection of clip rectangle and
				 * border */
				x0 = (clip.x0 > x0) ? clip.x0 : x0;
				y0 = (clip.y0 > y0) ? clip.y0 : y0;
				x1 = (clip.x1 < x1) ? clip.x1 : x1;
				y1 = (clip.y1 < y1) ? clip.y1 : y1;
				if ((x0 < x1) && (y0 < y1)) {
					/* valid clip rectangles only */
					if (!plot.rectangle(x0, y0, x1, y1,
							plot_style_bdr_in))
						return false;
				}
			} else {
				x0 = p[6];	y0 = p[7];
				x1 = p[2];	y1 = p[3];
				y1 = ((side == LEFT) && (p[1] - p[3] != 0)) ?
					y1 + p[1] - p[3] : y1;
				/* find intersection of clip rectangle and
				 * border */
				x0 = (clip.x0 > x0) ? clip.x0 : x0;
				y0 = (clip.y0 > y0) ? clip.y0 : y0;
				x1 = (clip.x1 < x1) ? clip.x1 : x1;
				y1 = (clip.y1 < y1) ? clip.y1 : y1;
				if ((x0 < x1) && (y0 < y1)) {
					/* valid clip rectangles only */
					if (!plot.rectangle(x0, y0, x1, y1,
							plot_style_bdr_out))
						return false;
				}
			}
		} else {
			/* Border made up from two parts and can't be plotted
			 * with rectangles */
			z[0] = p[0];
			z[1] = p[1];
			z[2] = (p[0] + p[2]) / 2;
			z[3] = (p[1] + p[3]) / 2;
			z[4] = (p[6] + p[4]) / 2;
			z[5] = (p[7] + p[5]) / 2;
			z[6] = p[6];
			z[7] = p[7];
			if (!plot.polygon(z, 4, plot_style_bdr_in))
				return false;
			z[0] = p[2];
			z[1] = p[3];
			z[6] = p[4];
			z[7] = p[5];
			if (!plot.polygon(z, 4, plot_style_bdr_out))
				return false;
		}
		break;

	case CSS_BORDER_STYLE_INSET:
		light = (light + 2) % 4;
		/* fall through */
	case CSS_BORDER_STYLE_OUTSET:
		/* choose correct colours for each part of the border line */
		switch (light) {
		case 0:
			plot_style_bdr_in = &plot_style_fillbdr_light;
			plot_style_bdr_out = &plot_style_fillbdr_dlight;
			break;
		case 1:
			plot_style_bdr_in = &plot_style_fillbdr_ddark;
			plot_style_bdr_out = &plot_style_fillbdr_dark;
			break;
		case 2:
			plot_style_bdr_in = &plot_style_fillbdr_dark;
			plot_style_bdr_out = &plot_style_fillbdr_ddark;
			break;
		case 3:
			plot_style_bdr_in = &plot_style_fillbdr_dlight;
			plot_style_bdr_out = &plot_style_fillbdr_light;
			break;
		default:
			plot_style_bdr_in = &plot_style_fillbdr;
			plot_style_bdr_out = &plot_style_fillbdr;
			break;
		}

		/* Render border */
		if ((rectangular || thickness == 2) && thickness != 1) {
			/* Border made up from two parts and can be plotted
			 * with rectangles */
			int x0, y0, x1, y1;

			/* First part */
			if (side == TOP || side == RIGHT) {
				x0 = (p[0] + p[2]) / 2;	y0 = (p[1] + p[3]) / 2;
				x1 = p[6];		y1 = p[7];
			} else {
				x0 = p[6];		y0 = p[7];
				x1 = (p[0] + p[2]) / 2;	y1 = (p[1] + p[3]) / 2;
			}
			/* find intersection of clip rectangle and border */
			x0 = (clip.x0 > x0) ? clip.x0 : x0;
			y0 = (clip.y0 > y0) ? clip.y0 : y0;
			x1 = (clip.x1 < x1) ? clip.x1 : x1;
			y1 = (clip.y1 < y1) ? clip.y1 : y1;
			if ((x0 < x1) && (y0 < y1)) {
				/* valid clip rectangles only */
				if (!plot.rectangle(x0, y0, x1, y1,
						plot_style_bdr_in))
					return false;
			}

			/* Second part */
			if (side == TOP || side == RIGHT) {
				x0 = p[2];		y0 = p[3];
				x1 = (p[6] + p[4]) / 2;	y1 = (p[7] + p[5]) / 2;
			} else {
				x0 = (p[6] + p[4]) / 2;	y0 = (p[7] + p[5]) / 2;
				x1 = p[2];		y1 = p[3];
			}
			/* find intersection of clip rectangle and border */
			x0 = (clip.x0 > x0) ? clip.x0 : x0;
			y0 = (clip.y0 > y0) ? clip.y0 : y0;
			x1 = (clip.x1 < x1) ? clip.x1 : x1;
			y1 = (clip.y1 < y1) ? clip.y1 : y1;
			if ((x0 < x1) && (y0 < y1)) {
				/* valid clip rectangles only */
				if (!plot.rectangle(x0, y0, x1, y1,
						plot_style_bdr_out))
					return false;
			}
		} else if (thickness == 1) {
			/* Border made up from one part which can be plotted
			 * as a rectangle */
			int x0, y0, x1, y1;
			if (side == TOP || side == RIGHT) {
				x0 = p[2];	y0 = p[3];
				x1 = p[6];	y1 = p[7];
				x1 = ((side == TOP) && (p[4] - p[6] != 0)) ?
					x1 + p[4] - p[6] : x1;
				/* find intersection of clip rectangle and
				 * border */
				x0 = (clip.x0 > x0) ? clip.x0 : x0;
				y0 = (clip.y0 > y0) ? clip.y0 : y0;
				x1 = (clip.x1 < x1) ? clip.x1 : x1;
				y1 = (clip.y1 < y1) ? clip.y1 : y1;
				if ((x0 < x1) && (y0 < y1)) {
					/* valid clip rectangles only */
					if (!plot.rectangle(x0, y0, x1, y1,
							plot_style_bdr_in))
						return false;
				}
			} else {
				x0 = p[6];	y0 = p[7];
				x1 = p[2];	y1 = p[3];
				y1 = ((side == LEFT) && (p[1] - p[3] != 0)) ?
					y1 + p[1] - p[3] : y1;
				/* find intersection of clip rectangle and
				 * border */
				x0 = (clip.x0 > x0) ? clip.x0 : x0;
				y0 = (clip.y0 > y0) ? clip.y0 : y0;
				x1 = (clip.x1 < x1) ? clip.x1 : x1;
				y1 = (clip.y1 < y1) ? clip.y1 : y1;
				if ((x0 < x1) && (y0 < y1)) {
					/* valid clip rectangles only */
					if (!plot.rectangle(x0, y0, x1, y1,
							plot_style_bdr_out))
						return false;
				}
			}
		} else {
			/* Border made up from two parts and can't be plotted
			 * with rectangles */
			z[0] = p[0];
			z[1] = p[1];
			z[2] = (p[0] + p[2]) / 2;
			z[3] = (p[1] + p[3]) / 2;
			z[4] = (p[6] + p[4]) / 2;
			z[5] = (p[7] + p[5]) / 2;
			z[6] = p[6];
			z[7] = p[7];
			if (!plot.polygon(z, 4, plot_style_bdr_in))
				return false;
			z[0] = p[2];
			z[1] = p[3];
			z[6] = p[4];
			z[7] = p[5];
			if (!plot.polygon(z, 4, plot_style_bdr_out))
				return false;
		}
		break;
	}

	return true;
}


/**
 * Plot a checkbox.
 *
 * \param  x	     left coordinate
 * \param  y	     top coordinate
 * \param  width     dimensions of checkbox
 * \param  height    dimensions of checkbox
 * \param  selected  the checkbox is selected
 * \return true if successful, false otherwise
 */

bool html_redraw_checkbox(int x, int y, int width, int height,
		bool selected)
{
	double z = width * 0.15;
	if (z == 0)
		z = 1;

	if (!(plot.rectangle(x, y, x + width, y + height,
			plot_style_fill_wbasec) &&
		plot.line(x, y, x + width, y, plot_style_stroke_darkwbasec) &&
		plot.line(x, y, x, y + height, plot_style_stroke_darkwbasec) &&
		plot.line(x + width, y, x + width, y + height,
				plot_style_stroke_lightwbasec) &&
		plot.line(x, y + height, x + width, y + height,
				plot_style_stroke_lightwbasec)))
		return false;

	if (selected) {
		if (width < 12 || height < 12) {
			/* render a solid box instead of a tick */
			if (!plot.rectangle(x + z + z, y + z + z,
					x + width - z, y + height - z,
					plot_style_fill_wblobc))
				return false;
		} else {
			/* render a tick, as it'll fit comfortably */
			if (!(plot.line(x + width - z,
					y + z,
					x + (z * 3),
					y + height - z,
					plot_style_stroke_wblobc) &&

				plot.line(x + (z * 3),
					y + height - z,
					x + z + z,
					y + (height / 2),
					plot_style_stroke_wblobc)))
				return false;
		}
	}
	return true;
}


/**
 * Plot a radio icon.
 *
 * \param  x	     left coordinate
 * \param  y	     top coordinate
 * \param  width     dimensions of radio icon
 * \param  height    dimensions of radio icon
 * \param  selected  the radio icon is selected
 * \return true if successful, false otherwise
 */
bool html_redraw_radio(int x, int y, int width, int height,
		bool selected)
{
	/* plot background of radio button */
	if (!plot.disc(x + width * 0.5,
			y + height * 0.5,
			width * 0.5 - 1,
			plot_style_fill_wbasec))
		return false;

	/* plot dark arc */
	if (!plot.arc(x + width * 0.5,
			y + height * 0.5,
			width * 0.5 - 1,
			45,
			225,
			plot_style_fill_darkwbasec))
		return false;

	/* plot light arc */
	if (!plot.arc(x + width * 0.5,
			y + height * 0.5,
			width * 0.5 - 1,
			225,
			45,
			plot_style_fill_lightwbasec))
		return false;

	if (selected) {
		/* plot selection blob */
		if (!plot.disc(x + width * 0.5,
				y + height * 0.5,
				width * 0.3 - 1,
				plot_style_fill_wblobc))
			return false;
	}

	return true;
}


/**
 * Plot a file upload input.
 *
 * \param  x	     left coordinate
 * \param  y	     top coordinate
 * \param  width     dimensions of input
 * \param  height    dimensions of input
 * \param  box	     box of input
 * \param  scale     scale for redraw
 * \param  background_colour  current background colour
 * \return true if successful, false otherwise
 */

bool html_redraw_file(int x, int y, int width, int height,
		struct box *box, float scale, colour background_colour)
{
	int text_width;
	const char *text;
	size_t length;
	plot_font_style_t fstyle;

	font_plot_style_from_css(box->style, &fstyle);
	fstyle.background = background_colour;

	if (box->gadget->value)
		text = box->gadget->value;
	else
		text = messages_get("Form_Drop");
	length = strlen(text);

	if (!nsfont.font_width(&fstyle, text, length, &text_width))
		return false;
	text_width *= scale;
	if (width < text_width + 8)
		x = x + width - text_width - 4;
	else
		x = x + 4;

	return plot.text(x, y + height * 0.75, text, length, &fstyle);
}


/**
 * Plot background images.
 *
 * \param  x	  coordinate of box
 * \param  y	  coordinate of box
 * \param  box	  box to draw background image of
 * \param  scale  scale for redraw
 * \param  clip   current clip rectangle
 * \param  background_colour  current background colour
 * \param  background  box containing background details (usually ::box)
 * \return true if successful, false otherwise
 *
 * The reason for the presence of ::background is the backwards compatibility
 * mess that is backgrounds on <body>. The background will be drawn relative
 * to ::box, using the background information contained within ::background.
 */

bool html_redraw_background(int x, int y, struct box *box, float scale,
		struct rect clip, colour *background_colour,
		struct box *background)
{
	bool repeat_x = false;
	bool repeat_y = false;
	bool plot_colour = true;
	bool plot_content;
	bool clip_to_children = false;
	struct box *clip_box = box;
	int px0 = clip.x0, py0 = clip.y0, px1 = clip.x1, py1 = clip.y1;
	int ox = x, oy = y;
	int width, height;
	css_fixed hpos = 0, vpos = 0;
	css_unit hunit = CSS_UNIT_PX, vunit = CSS_UNIT_PX;
	struct box *parent;
	css_color bgcol;
	plot_style_t pstyle_fill_bg = {
		0,
		0,
		0,
		PLOT_OP_TYPE_SOLID,
		0,
	};

	pstyle_fill_bg.fill_colour = *background_colour;
	if (html_redraw_printing && option_remove_backgrounds)
		return true;

	plot_content = (background->background != NULL);

	if (plot_content) {
		if (!box->parent) {
			/* Root element, special case:
			 * background origin calc. is based on margin box */
			x -= box->margin[LEFT] * scale;
			y -= box->margin[TOP] * scale;
			width = box->margin[LEFT] + box->padding[LEFT] +
					box->width + box->padding[RIGHT] +
					box->margin[RIGHT];
			height = box->margin[TOP] + box->padding[TOP] +
					box->height + box->padding[BOTTOM] +
					box->margin[BOTTOM];
		} else {
			width = box->padding[LEFT] + box->width +
					box->padding[RIGHT];
			height = box->padding[TOP] + box->height +
					box->padding[BOTTOM];
		}
		/* handle background-repeat */
		switch (css_computed_background_repeat(background->style)) {
		case CSS_BACKGROUND_REPEAT_REPEAT:
		{
			struct bitmap *bmp = content_get_bitmap(
					background->background);
			repeat_x = repeat_y = true;
			/* optimisation: only plot the colour if
			 * bitmap is not opaque */
			if (bmp != NULL)
				plot_colour = !bitmap_get_opaque(bmp);
		}
			break;
		case CSS_BACKGROUND_REPEAT_REPEAT_X:
			repeat_x = true;
			break;
		case CSS_BACKGROUND_REPEAT_REPEAT_Y:
			repeat_y = true;
			break;
		case CSS_BACKGROUND_REPEAT_NO_REPEAT:
			break;
		default:
			break;
		}

		/* handle background-position */
		css_computed_background_position(background->style,
				&hpos, &hunit, &vpos, &vunit);
		if (hunit == CSS_UNIT_PCT) {
			x += (width - 
				content_get_width(background->background)) *
				scale * FIXTOFLT(hpos) / 100.;
		} else {
			x += (int) (FIXTOFLT(nscss_len2px(hpos, hunit, 
					background->style)) * scale);
		}

		if (vunit == CSS_UNIT_PCT) {
			y += (height - 
				content_get_height(background->background)) *
				scale * FIXTOFLT(vpos) / 100.;
		} else {
			y += (int) (FIXTOFLT(nscss_len2px(vpos, vunit,
					background->style)) * scale);
		}
	}

	/* special case for table rows as their background needs
	 * to be clipped to all the cells */
	if (box->type == BOX_TABLE_ROW) {
		css_fixed h = 0, v = 0;
		css_unit hu = CSS_UNIT_PX, vu = CSS_UNIT_PX;

		for (parent = box->parent;
			((parent) && (parent->type != BOX_TABLE));
				parent = parent->parent);
		assert(parent && (parent->style));

		css_computed_border_spacing(parent->style, &h, &hu, &v, &vu);

		clip_to_children = (h > 0) || (v > 0);

		if (clip_to_children)
			clip_box = box->children;
	}

	for (; clip_box; clip_box = clip_box->next) {
		/* clip to child boxes if needed */
		if (clip_to_children) {
			struct bitmap *bmp = NULL;

			assert(clip_box->type == BOX_TABLE_CELL);

			/* update clip.* to the child cell */
			clip.x0 = ox + (clip_box->x * scale);
			clip.y0 = oy + (clip_box->y * scale);
			clip.x1 = clip.x0 + (clip_box->padding[LEFT] +
					clip_box->width +
					clip_box->padding[RIGHT]) * scale;
			clip.y1 = clip.y0 + (clip_box->padding[TOP] +
					clip_box->height +
					clip_box->padding[BOTTOM]) * scale;

			if (clip.x0 < px0) clip.x0 = px0;
			if (clip.y0 < py0) clip.y0 = py0;
			if (clip.x1 > px1) clip.x1 = px1;
			if (clip.y1 > py1) clip.y1 = py1;

			if (clip_box->background != NULL)
				bmp = content_get_bitmap(clip_box->background);

			/* <td> attributes override <tr> */
			if ((clip.x0 >= clip.x1) || (clip.y0 >= clip.y1) ||
					(css_computed_background_color(
						clip_box->style, &bgcol) !=
					CSS_BACKGROUND_COLOR_TRANSPARENT) ||
					(bmp != NULL && bitmap_get_opaque(bmp)))
				continue;
		}

		/* plot the background colour */
		if (css_computed_background_color(background->style, &bgcol) != 
				CSS_BACKGROUND_COLOR_TRANSPARENT) {
			*background_colour = nscss_color_to_ns(bgcol);
			pstyle_fill_bg.fill_colour = *background_colour;
			if (plot_colour)
				if (!plot.rectangle(clip.x0, clip.y0,
						clip.x1, clip.y1,
						&pstyle_fill_bg))
					return false;
		}
		/* and plot the image */
		if (plot_content) {
			width = content_get_width(background->background);
			height = content_get_height(background->background);

			if (!repeat_x) {
				if (clip.x0 < x)
					clip.x0 = x;
				if (clip.x1 > x + width * scale)
					clip.x1 = x + width * scale;
			}
			if (!repeat_y) {
				if (clip.y0 < y)
					clip.y0 = y;
				if (clip.y1 > y + height * scale)
					clip.y1 = y + height * scale;
			}
			/* valid clipping rectangles only */
			if ((clip.x0 < clip.x1) && (clip.y0 < clip.y1)) {
				if (!plot.clip(clip.x0, clip.y0,
						clip.x1, clip.y1))
					return false;
				if (!content_redraw_tiled(
						background->background, x, y,
						ceilf(width * scale),
						ceilf(height * scale),
						clip.x0, clip.y0,
						clip.x1, clip.y1,
						scale, *background_colour,
						repeat_x, repeat_y))
					return false;
			}
		}

		/* only <tr> rows being clipped to child boxes loop */
		if (!clip_to_children)
			return true;
	}
	return true;
}


/**
 * Plot an inline's background and/or background image.
 *
 * \param  x	  coordinate of box
 * \param  y	  coordinate of box
 * \param  box	  BOX_INLINE which created the background
 * \param  scale  scale for redraw
 * \param  clip	  coordinates of clip rectangle
 * \param  b	  coordinates of border edge rectangle
 * \param  first  true if this is the first rectangle associated with the inline
 * \param  last   true if this is the last rectangle associated with the inline
 * \param  background_colour  updated to current background colour if plotted
 * \return true if successful, false otherwise
 */

bool html_redraw_inline_background(int x, int y, struct box *box, float scale,
		struct rect clip, struct rect b, bool first, bool last,
		colour *background_colour)
{
	bool repeat_x = false;
	bool repeat_y = false;
	bool plot_colour = true;
	bool plot_content;
	css_fixed hpos = 0, vpos = 0;
	css_unit hunit = CSS_UNIT_PX, vunit = CSS_UNIT_PX;
	css_color bgcol;
	plot_style_t pstyle_fill_bg = {
		0,
		0,
		0,
		PLOT_OP_TYPE_SOLID,
		0
	};

	pstyle_fill_bg.fill_colour = *background_colour;

	plot_content = (box->background != NULL);

	if (html_redraw_printing && option_remove_backgrounds)
		return true;

	if (plot_content) {
		/* handle background-repeat */
		switch (css_computed_background_repeat(box->style)) {
		case CSS_BACKGROUND_REPEAT_REPEAT:
		{
			struct bitmap *bmp = 
					content_get_bitmap(box->background);
			repeat_x = repeat_y = true;
			/* optimisation: only plot the colour if
			 * bitmap is not opaque */
			if (bmp != NULL)
				plot_colour = !bitmap_get_opaque(bmp);
		}
			break;
		case CSS_BACKGROUND_REPEAT_REPEAT_X:
			repeat_x = true;
			break;
		case CSS_BACKGROUND_REPEAT_REPEAT_Y:
			repeat_y = true;
			break;
		case CSS_BACKGROUND_REPEAT_NO_REPEAT:
			break;
		default:
			break;
		}

		/* handle background-position */
		css_computed_background_position(box->style,
				&hpos, &hunit, &vpos, &vunit);
		if (hunit == CSS_UNIT_PCT) {
			x += (b.x1 - b.x0 -
					content_get_width(box->background) * 
					scale) * FIXTOFLT(hpos) / 100.;

			if (!repeat_x && ((hpos < 2 && !first) || 
					(hpos > 98 && !last))){
				plot_content = false;
			}
		} else {
			x += (int) (FIXTOFLT(nscss_len2px(hpos, hunit, 
					box->style)) * scale);
		}

		if (vunit == CSS_UNIT_PCT) {
			y += (b.y1 - b.y0 -
					content_get_height(box->background) *
					scale) * FIXTOFLT(vpos) / 100.;
		} else {
			y += (int) (FIXTOFLT(nscss_len2px(vpos, vunit, 
					box->style)) * scale);
		}
	}

	/* plot the background colour */
	if (css_computed_background_color(box->style, &bgcol) != 
			CSS_BACKGROUND_COLOR_TRANSPARENT) {
		*background_colour = nscss_color_to_ns(bgcol);
		pstyle_fill_bg.fill_colour = *background_colour;

		if (plot_colour)
			if (!plot.rectangle(clip.x0, clip.y0,
					clip.x1, clip.y1,
					&pstyle_fill_bg))
				return false;
	}
	/* and plot the image */
	if (plot_content) {
		int width = content_get_width(box->background);
		int height = content_get_height(box->background);

		if (!repeat_x) {
			if (clip.x0 < x)
				clip.x0 = x;
			if (clip.x1 > x + width * scale)
				clip.x1 = x + width * scale;
		}
		if (!repeat_y) {
			if (clip.y0 < y)
				clip.y0 = y;
			if (clip.y1 > y + height * scale)
				clip.y1 = y + height * scale;
		}
		/* valid clipping rectangles only */
		if ((clip.x0 < clip.x1) && (clip.y0 < clip.y1)) {
			if (!plot.clip(clip.x0, clip.y0,
					clip.x1, clip.y1))
				return false;
			if (!content_redraw_tiled(box->background, x, y,
					ceilf(width * scale),
					ceilf(height * scale),
					clip.x0, clip.y0,
					clip.x1, clip.y1,
					scale, *background_colour,
					repeat_x, repeat_y))
				return false;
		}
	}

	return true;
}


/**
 * Plot text decoration for a box.
 *
 * \param  box       box to plot decorations for
 * \param  x_parent  x coordinate of parent of box
 * \param  y_parent  y coordinate of parent of box
 * \param  scale     scale for redraw
 * \param  background_colour  current background colour
 * \return true if successful, false otherwise
 */

bool html_redraw_text_decoration(struct box *box,
		int x_parent, int y_parent, float scale,
		colour background_colour)
{
	static const enum css_text_decoration_e decoration[] = {
		CSS_TEXT_DECORATION_UNDERLINE, CSS_TEXT_DECORATION_OVERLINE,
		CSS_TEXT_DECORATION_LINE_THROUGH };
	static const float line_ratio[] = { 0.9, 0.1, 0.5 };
	colour fgcol;
	unsigned int i;
	css_color col;

	css_computed_color(box->style, &col);
	fgcol = nscss_color_to_ns(col);

	/* antialias colour for under/overline */
	if (html_redraw_printing == false)
		fgcol = blend_colour(background_colour, fgcol);

	if (box->type == BOX_INLINE) {
		if (!box->inline_end)
			return true;
		for (i = 0; i != NOF_ELEMENTS(decoration); i++)
			if (css_computed_text_decoration(box->style) & 
					decoration[i])
				if (!html_redraw_text_decoration_inline(box,
						x_parent, y_parent, scale,
						fgcol, line_ratio[i]))
					return false;
	} else {
		for (i = 0; i != NOF_ELEMENTS(decoration); i++)
			if (css_computed_text_decoration(box->style) & 
					decoration[i])
				if (!html_redraw_text_decoration_block(box,
						x_parent + box->x,
						y_parent + box->y,
						scale,
						fgcol, line_ratio[i]))
					return false;
	}

	return true;
}


/**
 * Plot text decoration for an inline box.
 *
 * \param  box     box to plot decorations for, of type BOX_INLINE
 * \param  x       x coordinate of parent of box
 * \param  y       y coordinate of parent of box
 * \param  scale   scale for redraw
 * \param  colour  colour for decorations
 * \param  ratio   position of line as a ratio of line height
 * \return true if successful, false otherwise
 */

bool html_redraw_text_decoration_inline(struct box *box, int x, int y,
		float scale, colour colour, float ratio)
{
	struct box *c;
	plot_style_t plot_style_box = {
		PLOT_OP_TYPE_SOLID,
		0,
		0,
		0,
		0
	};
	plot_style_box.stroke_colour = colour;

	for (c = box->next;
			c && c != box->inline_end;
			c = c->next) {
		if (c->type != BOX_TEXT)
			continue;
		if (!plot.line((x + c->x) * scale,
				(y + c->y + c->height * ratio) * scale,
				(x + c->x + c->width) * scale,
				(y + c->y + c->height * ratio) * scale,
				&plot_style_box))
			return false;
	}
	return true;
}


/**
 * Plot text decoration for an non-inline box.
 *
 * \param  box     box to plot decorations for, of type other than BOX_INLINE
 * \param  x       x coordinate of box
 * \param  y       y coordinate of box
 * \param  scale   scale for redraw
 * \param  colour  colour for decorations
 * \param  ratio   position of line as a ratio of line height
 * \return true if successful, false otherwise
 */

bool html_redraw_text_decoration_block(struct box *box, int x, int y,
		float scale, colour colour, float ratio)
{
	struct box *c;
	plot_style_t plot_style_box = {
		PLOT_OP_TYPE_SOLID,
		0,
		0,
		0,
		0
	};

	plot_style_box.stroke_colour = colour;

	/* draw through text descendants */
	for (c = box->children; c; c = c->next) {
		if (c->type == BOX_TEXT) {
			if (!plot.line((x + c->x) * scale,
					(y + c->y + c->height * ratio) * scale,
					(x + c->x + c->width) * scale,
					(y + c->y + c->height * ratio) * scale,
					&plot_style_box))
				return false;
		} else if (c->type == BOX_INLINE_CONTAINER ||
				c->type == BOX_BLOCK) {
			if (!html_redraw_text_decoration_block(c,
					x + c->x, y + c->y,
					scale, colour, ratio))
				return false;
		}
	}
	return true;
}
