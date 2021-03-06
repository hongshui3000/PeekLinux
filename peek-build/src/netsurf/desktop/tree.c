/*
 * Copyright 2004 Richard Wilson <not_ginger_matt@users.sourceforge.net>
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
 * Generic tree handling (implementation).
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "content/content.h"
#include "content/hlcache.h"
#include "desktop/browser.h"
#include "desktop/textarea.h"
#include "desktop/textinput.h"
#include "desktop/tree.h"
#include "desktop/options.h"
#include "desktop/plotters.h"
#include "render/font.h"
#include "utils/log.h"
#include "utils/messages.h"
#include "utils/utils.h"
#include "utils/url.h"

typedef enum {
	TREE_NO_DRAG = 0,
	TREE_SELECT_DRAG,
	TREE_MOVE_DRAG
} tree_drag_type;


#define MAXIMUM_URL_LENGTH 1024

#define TREE_ICON_SIZE 16
#define NODE_INSTEP 20
#define TREE_TEXT_HEIGHT 20
#define FURNITURE_COLOUR 0x888888

static plot_font_style_t plot_fstyle = {
	PLOT_FONT_FAMILY_SANS_SERIF,
	10240,
	400,
	FONTF_NONE,
	0xFFFFFF,
	0x000000
};

static plot_font_style_t plot_fstyle_selected = {
	PLOT_FONT_FAMILY_SANS_SERIF,
	10240,
	400,
	FONTF_NONE,
 	0x000000,
	0xEEEEEE
};

struct node;
struct tree;

struct node_element_box {
	int x;				/**< X offset from origin */
	int y;				/**< Y offset from origin */
	int width;			/**< Element width */
	int height;			/**< Element height */
};

struct node_element {
	struct node *parent;		/**< Parent node */
	node_element_type type;		/**< Element type */
	struct node_element_box box;	/**< Element bounding box */
	const char *text;		/**< Text for the element */
	void *bitmap;			/**< Bitmap for the element */
	struct node_element *next;	/**< Next node element */
	unsigned int flag;		/**< Client specified flag for data
					   being represented */
	bool editable;			/**< Whether the node text can be
					 * modified, editable text is deleted
					 * without noticing the tree user
					 */
};

struct node {
	bool selected;			/**< Whether the node is selected */
	bool expanded;			/**< Whether the node is expanded */
	bool folder;			/**< Whether the node is a folder */
	bool retain_in_memory;		/**< Whether the node remains
					   in memory after deletion */
	bool deleted;			/**< Whether the node is currently
					   deleted */
	bool processing;		/**< Internal flag used when moving */
	struct node_element_box box;	/**< Bounding box of all elements */
	struct node_element data;	/**< Data to display */
	struct node *parent;		/**< Parent entry (NULL for root) */
	struct node *child;		/**< First child */
	struct node *last_child;	/**< Last child */
	struct node *previous;		/**< Previous child of the parent */
	struct node *next;		/**< Next child of the parent */

	/** Sorting function for	the node (for folder nodes only) */
	int (*sort) (struct node *, struct node *);
	/** Gets called for each deleted node_element and on node launch */
	tree_node_user_callback user_callback;
	/** User data to be passed to delete_callback */
	void *callback_data;
};

struct tree {
	/* These coordinates are only added to the coordinates passed to the
	   plotters. This means they are invisible to the tree, what has to be
	   taken into account i.e in keyboard/mouse event passing */
	struct node *root;		/* Tree root element */
	int width;			/* Tree width */
	int height;			/* Tree height */
	unsigned int flags;		/* Tree flags */
	struct text_area *textarea;	/* Handle for UTF-8 textarea */
	bool textarea_drag_start;	/* whether the start of a mouse drag
					   was in the textarea */
	struct node_element *editing;	/* Node element being edited */

	bool redraw;			/* Flag indicating whether the tree
					   should be redrawn on layout
					   changes */
	tree_drag_type drag;
	const struct treeview_table *callbacks;
	void *client_data;		/* User assigned data for the
					   callbacks */
};


/**
 * Creates and initialises a new tree.
 *
 * \param flags		Flag word for flags to create the new tree with
 * \param callbacks	Callback functions to support the tree in the frontend.
 * \param client_data	Data to be passed to start_redraw and end_redraw
 * \return		The newly created tree, or NULL on memory exhaustion
 */
struct tree *tree_create(unsigned int flags,
		const struct treeview_table *callbacks, void *client_data)
{
	struct tree *tree;
	char *title;

	tree = calloc(sizeof(struct tree), 1);
	if (tree == NULL) {
		LOG(("calloc failed"));
		warn_user("NoMemory", 0);
		return NULL;
	}

	title = strdup("Root");
	if (title == NULL) {
		LOG(("malloc failed"));
		warn_user("NoMemory", 0);
		free(tree);
		return NULL;
	}
	tree->root = tree_create_folder_node(NULL, NULL, title,
					     false, false, false);
	if (tree->root == NULL) {
		free(title);
		free(tree);
		return NULL;
	}
	tree->root->expanded = true;

	tree->width = 0;
	tree->height = 0;
	tree->flags = flags;
	tree->textarea = NULL;
	tree->textarea_drag_start = false;
	tree->editing = NULL;
	tree->redraw = false;
	tree->drag = TREE_NO_DRAG;
	tree->callbacks = callbacks;
	tree->client_data = client_data;

	return tree;
}


/**
 * Recalculates the dimensions of a node element.
 *
 * \param tree	   the tree to which the element belongs, may be NULL
 * \param element  the element to recalculate
 */
static void tree_recalculate_node_element(struct tree *tree,
		struct node_element *element)
{
	struct bitmap *bitmap = NULL;
	int width, height;

	assert(element != NULL);

	switch (element->type) {
	case NODE_ELEMENT_TEXT_PLUS_ICON:
	case NODE_ELEMENT_TEXT:
		if(element->text == NULL)
			break;

		if (tree != NULL && element == tree->editing) {
			textarea_get_dimensions(tree->textarea,
						&element->box.width, NULL);
		} else {
			nsfont.font_width(&plot_fstyle,
					  element->text,
					  strlen(element->text),
					  &element->box.width);
		}

		element->box.width += 8;
		element->box.height = TREE_TEXT_HEIGHT;

		if (element->type == NODE_ELEMENT_TEXT_PLUS_ICON)
			element->box.width += NODE_INSTEP;

		break;

	case NODE_ELEMENT_BITMAP:
		bitmap = element->bitmap;
		if (bitmap != NULL) {
			width = bitmap_get_width(bitmap);
			height = bitmap_get_height(bitmap);
			element->box.width = width + 1;
			element->box.height = height + 2;
		} else {
			element->box.width = 0;
			element->box.height = 0;
		}
		break;
	}
}


/**
 * Calculates the height of a node including any children
 *
 * \param node  the node to calculate the height of
 * \return the total height of the node and children
 */
static int tree_get_node_height(struct node *node)
{
	int y1;

	assert(node != NULL);

	if ((node->child == NULL) || (node->expanded == false)) {
		return node->box.height;
	}

	y1 = node->box.y;
	if (y1 < 0) {
		y1 = 0;
	}
	node = node->child;

	while ((node->next != NULL) ||
	       ((node->child != NULL) && (node->expanded))) {
		for (; node->next != NULL; node = node->next);

		if ((node->child != NULL) && (node->expanded)) {
			node = node->child;
		}
	}
	return node->box.y + node->box.height - y1;
}


/**
 * Calculates the width of a node including any children
 *
 * \param node  the node to calculate the height of
 * \return the total width of the node and children
 */
static int tree_get_node_width(struct node *node)
{
	int width = 0;
	int child_width;

	assert(node != NULL);

	for (; node != NULL; node = node->next) {
		if (width < (node->box.x + node->box.width)) {
			width = node->box.x + node->box.width;
		}

		if ((node->child != NULL) && (node->expanded)) {
			child_width = tree_get_node_width(node->child);
			if (width < child_width) {
				width = child_width;
			}
		}
	}
	return width;
}


/**
 * Recalculates the position of a node, its siblings and children.
 *
 * \param tree	the tree to which 'root' belongs
 * \param root  the root node to update from
 */
static void tree_recalculate_node_positions(struct tree *tree,
		struct node *root)
{
	struct node *parent;
	struct node *node;
	struct node *child;
	struct node_element *element;
	int y;
	bool has_icon;

	for (node = root; node != NULL; node = node->next) {

		parent = node->parent;

		if (node->previous != NULL) {
			node->box.x = node->previous->box.x;
			node->box.y = node->previous->box.y +
				tree_get_node_height(node->previous);
		} else if (parent != NULL) {
			node->box.x = parent->box.x + NODE_INSTEP;
			node->box.y = parent->box.y +
				parent->box.height;
			for (child = parent->child; child != node;
			     child = child->next)
				node->box.y += child->box.height;
		} else {
			node->box.x = tree->flags & TREE_NO_FURNITURE
				? -NODE_INSTEP + 4 : 0;
			node->box.y = -20;
		}

		if (!node->expanded) {
			node->data.box.x = node->box.x;
			node->data.box.y = node->box.y;
			continue;
		}

		if (node->folder) {
			node->data.box.x = node->box.x;
			node->data.box.y = node->box.y;
			tree_recalculate_node_positions(tree, node->child);
		} else {
			y = node->box.y;
			has_icon = false;
			for (element = &node->data; element != NULL;
			     element = element->next)
				if (element->type ==
				    NODE_ELEMENT_TEXT_PLUS_ICON) {
					has_icon = true;
					break;
				}

			for (element = &node->data; element != NULL;
			     element = element->next) {
				element->box.x = node->box.x;
				if (element->type !=
				    NODE_ELEMENT_TEXT_PLUS_ICON &&
				    has_icon)
					element->box.x += NODE_INSTEP;
				element->box.y = y;
				y += element->box.height;
			}
		}

	}
}


/**
 * Recalculates the size of a node.
 *
 * \param tree			the tree to which node belongs, may be NULL
 * \param node			the node to update
 * \param recalculate_sizes	whether the node elements have changed
 */
static void tree_recalculate_node_sizes(struct tree *tree, struct node *node,
		bool recalculate_sizes)
{
	struct node_element *element;
	int width, height;

	assert(node != NULL);

	width = node->box.width;
	height = node->box.height;
	node->box.width = 0;
	node->box.height = 0;
	if (node->expanded) {
		for (element = &node->data; element != NULL;
		     element = element->next) {
			if (recalculate_sizes)
				tree_recalculate_node_element(tree, element);
			node->box.width = (node->box.width > element->box.x +
					   element->box.width - node->box.x) ?
				node->box.width :
				element->box.width + element->box.x -
				node->box.x;
			node->box.height += element->box.height;
		}
	} else {
		if (recalculate_sizes)
			for (element = &node->data; element != NULL;
			     element = element->next)
				tree_recalculate_node_element(tree, element);
		else
			tree_recalculate_node_element(tree, &node->data);
		node->box.width = node->data.box.width;
		node->box.height = node->data.box.height;
	}

	if (tree != NULL && height != node->box.height)
		tree_recalculate_node_positions(tree, tree->root);
}


/**
 * Creates a folder node with the specified title, and optionally links it into
 * the tree.
 *
 * \param tree		    the owner tree of 'parent', may be NULL
 * \param parent	    the parent node, or NULL not to link
 * \param title		    the node title (not copied, used directly)
 * \param editable	    if true, the node title will be editable
 * \param retain_in_memory  if true, the node will stay in memory after deletion
 * \param deleted	    if true, the node is created with the deleted flag
 * \return                  the newly created node.
 */
struct node *tree_create_folder_node(struct tree *tree, struct node *parent,
		const char *title, bool editable, bool retain_in_memory,
		bool deleted)
{
	struct node *node;

	assert(title != NULL);

	node = calloc(sizeof(struct node), 1);
	if (node == NULL) {
		LOG(("calloc failed"));
		warn_user("NoMemory", 0);
		return NULL;
	}
	node->folder = true;
	node->retain_in_memory = retain_in_memory;
	node->deleted = deleted;
	node->data.parent = node;
	node->data.type = NODE_ELEMENT_TEXT;
	node->data.text = title;
	node->data.flag = TREE_ELEMENT_TITLE;
	node->data.editable = editable;
	node->sort = NULL;
	node->user_callback = NULL;

	tree_recalculate_node_sizes(tree, node, true);
	if (parent != NULL)
		tree_link_node(tree, parent, node, false);

	return node;
}


/**
 * Creates a leaf node with the specified title, and optionally links it into
 * the tree.
 *
 * \param tree		    the owner tree of 'parent', may be NULL
 * \param parent	    the parent node, or NULL not to link
 * \param title		    the node title (not copied, used directly)
 * \param editable	    if true, the node title will be editable
 * \param retain_in_memory  if true, the node will stay in memory after deletion
 * \param deleted	    if true, the node is created with the deleted flag
 * \return		    the newly created node.
 */
struct node *tree_create_leaf_node(struct tree *tree, struct node *parent,
		const char *title, bool editable, bool retain_in_memory,
		bool deleted)
{
	struct node *node;

	assert(title != NULL);

	node = calloc(sizeof(struct node), 1);
	if (node == NULL) {
		LOG(("calloc failed"));
		warn_user("NoMemory", 0);
		return NULL;
	}

	node->folder = false;
	node->retain_in_memory = retain_in_memory;
	node->deleted = deleted;
	node->data.parent = node;
	node->data.type = NODE_ELEMENT_TEXT;
	node->data.text = title;
	node->data.flag = TREE_ELEMENT_TITLE;
	node->data.editable = editable;
	node->sort = NULL;
	node->user_callback = NULL;

	tree_recalculate_node_sizes(tree, node, true);
	if (parent != NULL)
		tree_link_node(tree, parent, node, false);

	return node;
}


/**
 * Creates an empty text node element and links it to a node.
 *
 * \param parent  the parent node
 * \param type    the required element type
 * \param flag	  user assigned flag used for searches
 * \return        the newly created element.
 */
struct node_element *tree_create_node_element(struct node *parent,
		node_element_type type, unsigned int flag, bool editable)
{
	struct node_element *element;

	element = calloc(sizeof(struct node_element), 1);
	if (element == NULL)
		return NULL;

	element->parent = parent;
	element->flag = flag;
	element->type = type;
	element->editable = editable;
	element->next = parent->data.next;
	parent->data.next = element;

	return element;
}


/**
 * Inserts a node into the correct place according to the parent's sort function
 *
 * \param parent  the node whose child node 'node' becomes
 * \param node	  the node to be inserted
 */
static void tree_sort_insert(struct node *parent, struct node *node)
{
	struct node *after;

	assert(node != NULL);
	assert(parent != NULL);
	assert(parent->sort != NULL);

	after = parent->last_child;
	while ((after != NULL) &&
	       (parent->sort(node, after) == -1))
		after = after->previous;

	if (after != NULL) {
		if (after->next != NULL)
			after->next->previous = node;
		node->next = after->next;
		node->previous = after;
		after->next = node;
	} else {
		node->previous = NULL;
		node->next = parent->child;
		if (parent->child != NULL) {
			parent->child->previous = node;
		}
		parent->child = node;
	}

	if (node->next == NULL)
		parent->last_child = node;

	node->parent = parent;
}


/**
 * Recalculates the size of a tree.
 *
 * \param tree  the tree to recalculate
 */
static void tree_recalculate_size(struct tree *tree)
{
	int width, height;

	assert(tree != NULL);

	width = tree->width;
	height = tree->height;

	tree->width = tree_get_node_width(tree->root);
	tree->height = tree_get_node_height(tree->root);

	if ((width != tree->width) || (height != tree->height))
		tree->callbacks->resized(tree, tree->width, tree->height,
					 tree->client_data);
}

/**
 * Recalculate the node data and redraw the relevant section of the tree.
 *
 * \param tree		     the tree to redraw, may be NULL
 * \param node		     the node to update
 * \param recalculate_sizes  whether the elements have changed
 * \param expansion	     the request is the result of a node expansion
 */
static void tree_handle_node_changed(struct tree *tree, struct node *node,
		bool recalculate_sizes, bool expansion)
{
	int node_width, node_height, tree_width, tree_height;

	assert(node != NULL);

	node_width = node->box.width;
	node_height = node->box.height;
	tree_width = tree->width;
	tree_height = tree->height;

	if ((recalculate_sizes) || (expansion)) {
		tree_recalculate_node_sizes(tree, node, true);
	}

	if (tree != NULL) {
		if ((node->box.height != node_height) || (expansion)) {
			tree_recalculate_node_positions(tree, tree->root);
			tree_recalculate_size(tree);
			if (tree->width > tree_width)
				tree_width = tree->width;
			if (tree->height > tree_height)
				tree_height = tree->height;
			if (tree->redraw) {
				tree->callbacks->redraw_request(0, node->box.y,
						tree_width,
						tree_height - node->box.y,
						tree->client_data);
			}
		} else {
			if (node->box.width > node_width)
				node_width = node->box.width;
			if (tree->redraw)
				tree->callbacks->redraw_request(node->box.x,
						node->box.y,
						node_width, node->box.height,
						tree->client_data);
			if (recalculate_sizes) {
				tree_recalculate_size(tree);
			}
		}
	}
}


/**
 * Links a node to another node.
 *
 * \param tree	  the tree in which the link takes place, may be NULL
 * \param link	  the node to link before/as a child (folders)
 * 		  or before/after (link)
 * \param node	  the node to link
 * \param before  whether to link siblings before or after the supplied node
 */
void tree_link_node(struct tree *tree, struct node *link, struct node *node,
		bool before)
{

	struct node *parent;
	bool sort = false;

	assert(link != NULL);
	assert(node != NULL);

	if ((link->folder == 0) || (before)) {
		parent = node->parent = link->parent;
		if (parent->sort) {
			sort = true;
		} else {
			if (before) {
				node->next = link;
				node->previous = link->previous;
				if (link->previous != NULL)
					link->previous->next = node;
				link->previous = node;
				if ((parent != NULL) && (parent->child == link))
					parent->child = node;
			} else {
				node->previous = link;
				node->next = link->next;
				if (link->next != NULL)
					link->next->previous = node;
				link->next = node;
				if ((parent != NULL) &&
				    (parent->last_child == link))
					parent->last_child = node;
			}
		}
	} else {
		parent = node->parent = link;
		if (parent->sort != NULL) {
			sort = true;
		} else {
			node->next = NULL;
			if (link->child == NULL) {
				link->child = link->last_child = node;
				node->previous = NULL;
			} else {
				link->last_child->next = node;
				node->previous = link->last_child;
				link->last_child = node;
			}
		}

	}

	if (sort) {
		tree_sort_insert(parent, node);
	}

	tree_handle_node_changed(tree, link, false, true);

	node->deleted = false;
}


/**
 * Recalculate the node element and redraw the relevant section of the tree.
 * The tree size is not updated.
 *
 * \param tree	   the tree to redraw, may be NULL
 * \param element  the node element to update
 */
static void tree_handle_node_element_changed(struct tree *tree,
		struct node_element *element)
{
	int width, height;

	assert(element != NULL);

	width = element->box.width;
	height = element->box.height;
	tree_recalculate_node_element(tree, element);

	if (element->box.height != height) {
		tree_recalculate_node_sizes(tree, element->parent, false);
		if ((tree != NULL) && (tree->redraw)) {
			tree->callbacks->redraw_request(0, element->box.y,
					tree->width + element->box.width -
					width,
					tree->height - element->box.y +
					element->box.height - height,
					tree->client_data);
		}
	} else {
		if (element->box.width != width) {
			tree_recalculate_node_sizes(tree, element->parent,
						    false);
		}

		if (tree != NULL) {
			width = (width > element->box.width) ? width :
				element->box.width;
			if (tree->redraw) {
				tree->callbacks->redraw_request(element->box.x,
						element->box.y,
						width,
						element->box.height,
						tree->client_data);
			}
		}
	}
}


/**
 * Stops editing a node_element
 *
 * \param tree		The tree to stop editing for
 * \param keep_changes	If true the changes made to the text will be kept,
 *			if false they will be dropped
 */
static void tree_stop_edit(struct tree *tree, bool keep_changes)
{
	int text_len;
	char *text = NULL;
	struct node_element *element;
	struct node_msg_data msg_data;
	node_callback_resp response;

	assert(tree != NULL);

	if (tree->editing == NULL || tree->textarea == NULL)
		return;

	element = tree->editing;

	if (keep_changes) {
		text_len = textarea_get_text(tree->textarea, NULL, 0);
		text = malloc(text_len * sizeof(char));
		if (text == NULL) {
			LOG(("malloc failed"));
			warn_user("NoMemory", 0);
			textarea_destroy(tree->textarea);
			tree->textarea = NULL;
			return;
		}
		textarea_get_text(tree->textarea, text, text_len);
	}


	if (keep_changes && element->parent->user_callback != NULL) {
		msg_data.msg = NODE_ELEMENT_EDIT_FINISHING;
		msg_data.flag = element->flag;
		msg_data.node = element->parent;
		msg_data.data.text = text;
		response = element->parent->user_callback(
			element->parent->callback_data,
			&msg_data);

		switch (response) {
		case NODE_CALLBACK_REJECT:
			free(text);
			text = NULL;
			break;
		case NODE_CALLBACK_CONTINUE:
			free(text);
			text = NULL;
			return;
		case NODE_CALLBACK_HANDLED:
		case NODE_CALLBACK_NOT_HANDLED:
			text = msg_data.data.text;
			break;
		}
	}

	textarea_destroy(tree->textarea);
	tree->textarea = NULL;
	tree->editing = NULL;

	if (text != NULL)
		tree_update_node_element(tree, element, text, NULL);
	else
		tree_handle_node_element_changed(tree, element);


	tree_recalculate_size(tree);
	if (element->parent->user_callback != NULL) {
		msg_data.msg = NODE_ELEMENT_EDIT_FINISHED;
		msg_data.flag = element->flag;
		msg_data.node = element->parent;
		element->parent->user_callback(element->parent->callback_data,
					       &msg_data);
	}
}


/**
 * Delinks a node from the tree structures.
 *
 * \param tree	the tree in which the delink takes place, may be NULL
 * \param node  the node to delink
 */
void tree_delink_node(struct tree *tree, struct node *node)
{
	struct node *parent;

	assert(node != NULL);

	/* do not remove the root */
	if (tree != NULL && node == tree->root)
		return;
	if ((tree != NULL) && (tree->editing != NULL)) {
		parent = tree->editing->parent;
		while (parent != NULL) {
			if (node == parent) {
				tree_stop_edit(tree, false);
				break;
			}
			parent = parent->parent;
		}
	}

	if (node->parent->child == node)
		node->parent->child = node->next;
	if (node->parent->last_child == node)
		node->parent->last_child = node->previous;
	parent = node->parent;
	node->parent = NULL;

	if (node->previous != NULL)
		node->previous->next = node->next;
	if (node->next != NULL)
		node->next->previous = node->previous;
	node->previous = NULL;
	node->next = NULL;

	tree_handle_node_changed(tree, parent, false, true);
}


/**
 * Deletes a node from the tree.
 *
 * \param tree	    the tree to delete from, may be NULL
 * \param node	    the node to delete
 * \param siblings  whether to delete all siblings
 */
static void tree_delete_node_internal(struct tree *tree, struct node *node,
		bool siblings)
{
	struct node *next, *child, *parent;
	struct node_element *e, *f;
	node_callback_resp response;
	struct node_msg_data msg_data;

	assert(node != NULL);

	if (tree != NULL && tree->root == node)
		return;

	next = node->next;
	parent = node->parent;
	if (tree != NULL && parent == tree->root)
		parent = NULL;
	tree_delink_node(tree, node);
	child = node->child;
	node->child = NULL;

	node->deleted = true;
	if (child != NULL)
		tree_delete_node_internal(tree, child, true);

	if (!node->retain_in_memory) {
	  	node->retain_in_memory = true;
		for (e = &node->data; e != NULL; e = f) {
			if (e->text != NULL) {
				response = NODE_CALLBACK_NOT_HANDLED;
				if (!e->editable &&
				    node->user_callback != NULL) {
					msg_data.msg = NODE_DELETE_ELEMENT_TXT;
					msg_data.flag = e->flag;
					msg_data.node = node;
					msg_data.data.text = (void *)e->text;
					response = node->user_callback(
						node->callback_data,
						&msg_data);
				}
				if (response != NODE_CALLBACK_HANDLED)
					free((void *)e->text);
				e->text = NULL;
			}
			if (e->bitmap != NULL) {
				response = NODE_CALLBACK_NOT_HANDLED;
				if (node->user_callback != NULL) {
					msg_data.msg = NODE_DELETE_ELEMENT_IMG;
					msg_data.flag = e->flag;
					msg_data.node = node;
					msg_data.data.bitmap =
						(void *)e->bitmap;
					response = node->user_callback(
						node->callback_data,
						&msg_data);
				}
				/* TODO the type of this field is platform
				   dependent */
				if (response != NODE_CALLBACK_HANDLED)
					free(e->bitmap);
				e->bitmap = NULL;
			}
			f = e->next;
			if (e != &node->data)
				free(e);
		}
		free(node);
	}

	if (siblings && next)
		tree_delete_node_internal(tree, next, true);
	if ((tree->flags & TREE_DELETE_EMPTY_DIRS) && parent != NULL &&
	    parent->child == NULL && !parent->deleted)
		tree_delete_node_internal(tree, parent, false);
}


/**
 * Deletes all nodes of a tree and the tree itself.
 *
 * \param tree the tree to be deleted
 */
void tree_delete(struct tree *tree)
{
	tree_set_redraw(tree, false);
	if (tree->root->child != NULL)
		tree_delete_node_internal(tree, tree->root->child, true);

	free((void *)tree->root->data.text);
	free(tree->root);
	free(tree);
}


/**
 * Gets the redraw property of the given tree.
 *
 * \param tree	the tree for which to retrieve the property
 * \return	the redraw property of the tree
 */
bool tree_get_redraw(struct tree *tree)
{
	return tree->redraw;
}


/**
 * Deletes a node from the tree.
 *
 * \param tree	    the tree to delete from, may be NULL
 * \param node	    the node to delete
 * \param siblings  whether to delete all siblings
 */
void tree_delete_node(struct tree *tree, struct node *node, bool siblings)
{
	int y = node->box.y;
	tree_delete_node_internal(tree, node, siblings);
	tree_recalculate_node_positions(tree, tree->root);
	if (tree->redraw)
		tree->callbacks->redraw_request(0, y, tree->width, tree->height,
						tree->client_data);
	tree_recalculate_size(tree);
}


/**
 * Sets an icon for a node
 *
 * \param tree		The tree to which node belongs, may be NULL
 * \param node		The node for which the icon is set
 * \param icon	  	the image to use
 */
void tree_set_node_icon(struct tree *tree, struct node *node,
		hlcache_handle *icon)
{
	node->data.type = NODE_ELEMENT_TEXT_PLUS_ICON;
	tree_update_node_element(tree, &(node->data), NULL, icon);
}


/**
 * Updates all siblings and descendants of a node to an expansion state.
 * No update is performed for the tree changes.
 *
 * \param tree		the tree to which 'node' belongs
 * \param node		the node to set all siblings and descendants of
 * \param expanded	the expansion state to set
 */
static void tree_set_node_expanded_all(struct tree *tree, struct node *node,
		bool expanded)
{
	for (; node != NULL; node = node->next) {
		if (node->expanded != expanded) {
			node->expanded = expanded;
			tree_recalculate_node_sizes(tree, node, false);
		}
		if ((node->child != NULL) && (node->expanded))
			tree_set_node_expanded_all(tree, node->child, expanded);
	}
}


/**
 * Updates [all siblings and descendants of] a node to an expansion state.
 *
 * \param tree	    the tree to update
 * \param node	    the node to set [all siblings and descendants of]
 * \param expanded  the expansion state to set
 * \param folder    whether to update folders, if this together with leaf
 *		    will be false only 'node' will be updated
 * \param leaf	    whether to update leaves (check also description for folder)
 * \return	    whether any changes were made
 */
static bool tree_set_node_expanded_internal(struct tree *tree,
		struct node *node, bool expanded, bool folder, bool leaf)
{
	bool redraw = false;
	struct node *end = (folder == false && leaf == false) ?
		node->next : NULL;

	if (tree->editing != NULL && node == tree->editing->parent)
		tree_stop_edit(tree, false);

	for (; node != end; node = node->next) {
		if ((node->expanded != expanded) && (node != tree->root) &&
		    ((folder && (node->folder)) ||
		     (leaf && (!node->folder)) ||
		     (!folder && !leaf))) {
			node->expanded = expanded;
			if (node->child != NULL)
				tree_set_node_expanded_all(tree,
							   node->child, false);
			if ((node->data.next != NULL) &&
			    (node->data.next->box.height == 0))
				tree_recalculate_node_sizes(tree, node, true);
			else
				tree_recalculate_node_sizes(tree, node, false);
			redraw = true;
		}
		if ((folder || leaf) && (node->child != NULL) &&
		    (node->expanded))
			redraw |= tree_set_node_expanded_internal(tree,
								  node->child, expanded, folder, leaf);
	}
	return redraw;
}


/**
 * Updates [all siblings and descendants of] a node to an expansion state.
 *
 * \param tree	    the tree to update
 * \param node	    the node to set [all siblings and descendants of]
 * \param expanded  the expansion state to set
 * \param folder    whether to update folders, if this together with leaf
 *		    will be false only 'node' will be updated
 * \param leaf	    whether to update leaves (check also description for folder)
 */
void tree_set_node_expanded(struct tree *tree, struct node *node, bool expanded,
		bool folder, bool leaf)
{
	if (tree_set_node_expanded_internal(tree, node, expanded, folder, leaf))
		tree_handle_node_changed(tree, node, false, true);
}


/**
 * Updates a node to an selected state. The required areas of the tree are
 * redrawn.
 *
 * \param tree	    the tree to update nodes for, may be NULL
 * \param node	    the node to set all siblings and descendants of
 * \param all	    if true update node together with its siblings and
 *		    descendants
 * \param selected  the selection state to set
 */
void tree_set_node_selected(struct tree *tree, struct node *node, bool all,
		bool selected)
{
	struct node *end;

	if (tree != NULL && node == tree->root)
		node = tree->root->child;
	if (node == NULL)
		return;

	end = all ? NULL : node->next;

	for (; node != end; node = node->next) {
		if (node->selected != selected) {
			node->selected = selected;
			if (tree != NULL && tree->redraw)
				tree->callbacks->redraw_request(node->box.x,
								node->box.y,
								node->box.width,
								node->data.box.height,
								tree->client_data);
		}
		if (all && (node->child != NULL) && (node->expanded))
			tree_set_node_selected(tree, node->child, all,
					       selected);
	}
}


/**
 * Sets the sort function for a node
 *
 * \param tree  the tree to which 'node' belongs, may be NULL
 * \param node	the node to be inserted
 * \param sort	pointer to the sorting function
 */
void tree_set_node_sort_function(struct tree *tree, struct node *node,
		int (*sort) (struct node *, struct node *))
{
	struct node *child;

	node->sort = sort;

	if (tree != NULL && tree->editing != NULL)
		tree_stop_edit(tree, false);

	/* the node had already some children so they must get sorted */
	if (node->child != NULL) {

		child = node->child;
		node->child = NULL;

		while (child != NULL) {
			tree_sort_insert(node, child);
			child = child->next;
		}

	}

	if (tree != NULL)
		tree_recalculate_node_positions(tree, node->child);
}


/**
 * Sets the delete callback for a node.
 *
 * \param node      the node for which the callback is set
 * \param callback  the callback functions to be set
 * \param data      user data to be passed to callback
 */
void tree_set_node_user_callback(struct node *node,
		tree_node_user_callback callback, void *data)
{
	node->user_callback = callback;
	node->callback_data = data;
}


/**
 * Sets the redraw property to the given value. If redraw is true, the tree will
 * be redrawn on layout/appearance changes.
 *
 * \param tree    the tree for which the property is set
 * \param redraw  the value to set
 */
void tree_set_redraw(struct tree *tree, bool redraw)
{
	/* the tree might have no graphical representation, do not set the
	   redraw flag in such case */
	if (tree->callbacks == NULL)
		return;
	tree->redraw = redraw;
}


/**
 * Checks whether a node, its siblings or any children are selected.
 *
 * \param node  the root node to check from
 * \return	whether 'node', its siblings or any children are selected.
 */
bool tree_node_has_selection(struct node *node)
{
	for (; node != NULL; node = node->next) {
		if (node->selected)
			return true;
		if ((node->child != NULL) && (node->expanded) &&
		    (tree_node_has_selection(node->child)))
			return true;
	}
	return false;
}


/**
 * Returns the current value of the nodes deleted property.
 *
 * \param node  the node to be checked
 * \return      the current value of the nodes deleted property
 */
bool tree_node_is_deleted(struct node *node)
{
	return node->deleted;
}


/**
 * Returns true if the node is a folder
 *
 * \param node  the node to be checked
 * \return      true if the node is a folder, false otherwise
 */
bool tree_node_is_folder(struct node *node)
{
	return node->folder;
}


/**
 * Update the text of a node element if it has changed.
 *
 * \param element The node element to update.
 * \param text The text to update the element with. The ownership of
 *             this string is taken by this function and must not be
 *             referred to after the function exits.
 */
bool tree_update_element_text(struct tree *tree,
		struct node_element *element, char *text)
{
	const char *node_text; /* existing node text */

	if (text == NULL)
		return false;

	if (element == NULL) {
		free(text);
		return false;
	}

	node_text = tree_node_element_get_text(element);

	if ((node_text == NULL) || (strcmp(node_text, text) != 0)) {
		tree_update_node_element(tree, element, text, NULL);
	} else {
		/* text does not need changing, free it */
		free(text);
	}
	return true;
}


/**
 * Updates the content of a node_element.
 *
 * \param tree      the tree owning element, may be NULL
 * \param element   the element to be updated
 * \param text      new text to be set, may be NULL
 * \param bitmap    new bitmap to be set, may be NULL
 */
void tree_update_node_element(struct tree *tree, struct node_element *element,
		const char *text, void *bitmap)
{
	node_callback_resp response;
	struct node_msg_data msg_data;

	assert(element != NULL);

	if (tree != NULL && element == tree->editing)
		tree_stop_edit(tree, false);

	if (text != NULL && (element->type == NODE_ELEMENT_TEXT ||
			     element->type == NODE_ELEMENT_TEXT_PLUS_ICON)) {
		if (element->text != NULL) {
			response = NODE_CALLBACK_NOT_HANDLED;
			if (!element->editable &&
			    element->parent->user_callback !=
			    NULL) {
				msg_data.msg = NODE_DELETE_ELEMENT_TXT;
				msg_data.flag = element->flag;
				msg_data.node = element->parent;
				msg_data.data.text = (void *)element->text;
				response = element->parent->user_callback(
					element->parent->callback_data,
					&msg_data);
			}
			if (response != NODE_CALLBACK_HANDLED)
				free((void *)element->text);
		}
		element->text = text;
	}

	if (bitmap != NULL && (element->type == NODE_ELEMENT_BITMAP ||
			       element->type == NODE_ELEMENT_TEXT_PLUS_ICON)) {
		if (element->bitmap != NULL) {
			response = NODE_CALLBACK_NOT_HANDLED;
			if (element->parent->user_callback != NULL) {
				msg_data.msg = NODE_DELETE_ELEMENT_IMG;
				msg_data.flag = element->flag;
				msg_data.node = element->parent;
				msg_data.data.bitmap = (void *)element->bitmap;
				response = element->parent->user_callback(
					element->parent->callback_data,
					&msg_data);
			}
			if (response != NODE_CALLBACK_HANDLED)
				free(element->bitmap);
		}
		element->bitmap = bitmap;
	}

	tree_handle_node_element_changed(tree, element);
}


/**
 * Returns the node element's text
 *
 * \return  the node element's text
 */
const char *tree_node_element_get_text(struct node_element *element)
{
	return element->text;
}


/**
 * Get the root node of a tree
 *
 * \param tree  the tree to get the root of
 * \return	the root of the tree
 */
struct node *tree_get_root(struct tree *tree)
{
	return tree->root;
}


/**
 * Returns whether the current tree is being edited at this time
 *
 * \param tree  the tree to be checked
 * \return	true if the tree is currently being edited
 */
bool tree_is_edited(struct tree *tree)
{
	return tree->editing == NULL ? false : true;
}


/**
 * Returns the first child of a node
 *
 * \param node	the node to get the child of
 * \return	the nodes first child
 */
struct node *tree_node_get_child(struct node *node)
{
	return node->child;
}


/**
 * Returns the closest sibling a node
 *
 * \param node	the node to get the sibling of
 * \return	the nodes sibling
 */
struct node *tree_node_get_next(struct node *node)
{
	return node->next;
}


/**
 * Draws an elements expansion icon
 *
 * \param tree	   the tree to draw the expansion for
 * \param element  the element to draw the expansion for
 * \param tree_x	X coordinate of the tree
 * \param tree_y	Y coordinate of the tree
 */
static void tree_draw_node_expansion(struct tree *tree, struct node *node,
		int tree_x, int tree_y)
{
	int x, y;

	assert(tree != NULL);
	assert(node != NULL);

	if ((node->child != NULL) || (node->data.next != NULL)) {
		x = tree_x + node->box.x - (NODE_INSTEP / 2) - 4;
		y = tree_y + node->box.y - (TREE_TEXT_HEIGHT / 2) + 16;
		plot.rectangle(x, y, x + 9, y + 9, plot_style_fill_white);
		plot.rectangle(x , y, x + 8, y + 8,
			       plot_style_stroke_darkwbasec);
		plot.line(x + 2, y + 4, x + 7, y + 4,
			  plot_style_stroke_darkwbasec);
		if (!node->expanded)
			plot.line(x + 4, y + 2, x + 4, y + 7,
				  plot_style_stroke_darkwbasec);

	}

}


/**
 * Draws an element, including any expansion icons
 *
 * \param tree	   the tree to draw an element for
 * \param element  the element to draw
 * \param tree_x	X coordinate of the tree
 * \param tree_y	Y coordinate of the tree
 */
static void tree_draw_node_element(struct tree *tree,
		struct node_element *element, int tree_x, int tree_y)
{

	struct bitmap *bitmap = NULL;
	int x, y, width;
	bool selected = false;
	hlcache_handle *icon;
	plot_font_style_t *fstyle;

	assert(tree != NULL);
	assert(element != NULL);
	assert(element->parent != NULL);

	x = tree_x + element->box.x;
	y = tree_y + element->box.y;
	width = element->box.width;
	if (&element->parent->data == element)
		if (element->parent->selected)
			selected = true;

	switch (element->type) {
	case NODE_ELEMENT_TEXT_PLUS_ICON:
		icon = element->bitmap;
		if (icon != NULL &&
		    (content_get_status(icon) ==
		     CONTENT_STATUS_READY ||
		     content_get_status(icon) ==
		     CONTENT_STATUS_DONE)) {
			content_redraw(icon , x, y + 3,
				       TREE_ICON_SIZE, TREE_ICON_SIZE,
				       x, y, x + TREE_ICON_SIZE,
				       y + TREE_ICON_SIZE, 1, 0);
		}

		x += NODE_INSTEP;
		width -= NODE_INSTEP;

		/* fall through */
	case NODE_ELEMENT_TEXT:
		if (element->text == NULL)
			break;

		if (element == tree->editing)
			return;

		if (selected) {
			fstyle = &plot_fstyle_selected;
			plot.rectangle(x, y, x + width,
				       y + element->box.height,
				       plot_style_fill_black);
		} else {
			fstyle = &plot_fstyle;
			plot.rectangle(x, y,	x + width,
				       y + element->box.height,
				       plot_style_fill_white);
		}

		plot.text(x + 4, y + TREE_TEXT_HEIGHT * 0.75,
			  element->text, strlen(element->text),
			  fstyle);
		break;
	case NODE_ELEMENT_BITMAP:
		bitmap = element->bitmap;
		if (bitmap == NULL)
			break;
		plot.bitmap(x, y, element->box.width - 1,
			    element->box.height - 2,
			    bitmap, 0xFFFFFF, BITMAPF_NONE);
		if (!(tree->flags & TREE_NO_FURNITURE))
			plot.rectangle(x, y, x + element->box.width - 1,
				       y + element->box.height - 3,
				       plot_style_stroke_darkwbasec);

		break;
	}

}


/**
 * Redraws a node.
 *
 * \param tree		the tree to draw
 * \param node		the node to draw children and siblings of
 * \param tree_x	X coordinate of the tree
 * \param tree_y	Y coordinate of the tree
 * \param clip_x	the minimum x of the clipping rectangle
 * \param clip_y	the minimum y of the clipping rectangle
 * \param clip_width	the width of the clipping rectangle
 * \param clip_height	the height of the clipping rectangle
 */
static void tree_draw_node(struct tree *tree, struct node *node,
		int tree_x, int tree_y,
		int clip_x, int clip_y,
		int clip_width, int clip_height)
{
	struct node_element *element;
	struct node *parent;
	int x_max, y_max;
	int x0, y0, x1, y1;

	assert(tree != NULL);
	assert(node != NULL);


	x_max = clip_x + clip_width + NODE_INSTEP;
	y_max = clip_y + clip_height;

	if ((node->parent->next != NULL) &&
	    (node->parent->next->box.y < clip_y))
		return;

	for (; node != NULL; node = node->next) {
		if (node->box.y > y_max) return;
		if ((node->next != NULL) &&
		    (!(tree->flags & TREE_NO_FURNITURE))) {
			x0 = x1 = tree_x + node->box.x - (NODE_INSTEP / 2);
			y0 = tree_y + node->box.y + (20 / 2);
			y1 = y0 + node->next->box.y - node->box.y;
			plot.line(x0, y0, x1, y1, plot_style_stroke_darkwbasec);
		}
		if ((node->box.x < x_max) && (node->box.y < y_max) &&
		    (node->box.x + node->box.width
		     + NODE_INSTEP >= clip_x) &&
		    (node->box.y + node->box.height >= clip_y)) {
			if (!(tree->flags & TREE_NO_FURNITURE)) {
				if ((node->expanded) && (node->child != NULL)) {
					x0 = x1 = tree_x + node->box.x +
						(NODE_INSTEP / 2);
					y0 = tree_y + node->data.box.y
						+ node->data.box.height;
					y1 = y0 + (20 / 2);
					plot.line(x0, y0, x1, y1,
						  plot_style_stroke_darkwbasec);

				}
				parent = node->parent;
				if ((parent != NULL) &&
				    (parent != tree->root) &&
				    (parent->child == node)) {
					x0 = x1 = tree_x + parent->box.x +
						(NODE_INSTEP / 2);
					y0 = tree_y + parent->data.box.y +
						parent->data.box.height;
					y1 = y0 + (20 / 2);
					plot.line(x0, y0, x1, y1,
						  plot_style_stroke_darkwbasec);
				}
				x0 = tree_x + node->box.x - (NODE_INSTEP / 2);
				x1 = x0 + (NODE_INSTEP / 2) - 2;
				y0 = y1 = tree_y + node->data.box.y +
					node->data.box.height -
					(20 / 2);
				plot.line(x0, y0, x1, y1,
					  plot_style_stroke_darkwbasec);
				tree_draw_node_expansion(tree, node,
							 tree_x, tree_y);
			}
			if (node->expanded)
				for (element = &node->data; element != NULL;
				     element = element->next)
					tree_draw_node_element(tree, element,
							       tree_x, tree_y);
			else
				tree_draw_node_element(tree, &node->data,
						       tree_x, tree_y);
		}
		if ((node->child != NULL) && (node->expanded))
			tree_draw_node(tree, node->child, tree_x, tree_y,
				       clip_x, clip_y,
				       clip_width, clip_height);
	}
}


/**
 * Redraws a tree.
 *
 * \param tree		the tree to draw
 * \param x		X coordinate to draw the tree at
 * \param y		Y coordinate to draw the tree at
 * \param clip_x	the minimum x of the clipping rectangle relative to
 * 			the tree origin
 * \param clip_y	the minimum y of the clipping rectangle relative to
 * 			the tree origin
 * \param clip_width	the width of the clipping rectangle
 * \param clip_height	the height of the clipping rectangle
 */
void tree_draw(struct tree *tree, int x, int y,
	       int clip_x, int clip_y, int clip_width, int clip_height)
{
	int absolute_x, absolute_y;
	assert(tree != NULL);
	assert(tree->root != NULL);

	/* don't draw empty trees or trees with redraw flag set to false */
	if (tree->root->child == NULL || !tree->redraw) return;

	absolute_x = x + clip_x;
	absolute_y = y + clip_y;
	plot.rectangle(absolute_x, absolute_y,
		       absolute_x + clip_width, absolute_y + clip_height,
		       plot_style_fill_white);
	plot.clip(absolute_x, absolute_y,
		  absolute_x + clip_width, absolute_y + clip_height);
	tree_draw_node(tree, tree->root->child, x, y, clip_x,
		       clip_y, clip_width, clip_height);
 	if (tree->editing != NULL) {
		x = x + tree->editing->box.x;
		y = y + tree->editing->box.y;
		if (tree->editing->type == NODE_ELEMENT_TEXT_PLUS_ICON)
			x += NODE_INSTEP;
		textarea_redraw(tree->textarea, x, y, absolute_x, absolute_y,
 				absolute_x + clip_width,
 				absolute_y + clip_height);
 	}
}


/**
 * Finds a node element from a node with a specific user_type
 *
 * \param node  the node to examine
 * \param flag  user assinged flag used is searches
 * \param after if this is not NULL the search will start after the given
 *		node_element
 * \return      the corresponding element
 */
struct node_element *tree_node_find_element(struct node *node,
		unsigned int flag, struct node_element *after)
{
	struct node_element *element;

	if (after == NULL)
		element = &node->data;
	else {
		assert(after->parent == node);
		element = after->next;
	}

	for (; element != NULL; element = element->next)
		if (element->flag == flag) return element;

	return NULL;
}


/**
 * Deletes all selected nodes from the tree.
 *
 * \param tree  the tree to delete from
 * \param node  the node to delete
 */
void tree_delete_selected_nodes(struct tree *tree, struct node *node)
{
	struct node *next;

	if (node == tree->root) {
		if (node->child != NULL)
			tree_delete_selected_nodes(tree, node->child);
		return;
	}

	while (node != NULL) {
		next = node->next;
		if (node->selected)
			tree_delete_node(tree, node, false);
		else if (node->child != NULL)
			tree_delete_selected_nodes(tree, node->child);
		node = next;
	}
}


/**
 * Returns the selected node, or NULL if multiple nodes are selected.
 *
 * \param node  the node to search sibling and children
 * \return      the selected node, or NULL if multiple nodes are selected
 */
struct node *tree_get_selected_node(struct node *node)
{
	struct node *result = NULL;
	struct node *temp;

	for (; node != NULL; node = node->next) {
		if (node->selected) {
			if (result != NULL)
				return NULL;
			result = node;
		}
		if ((node->child != NULL) && (node->expanded)) {
			temp = tree_get_selected_node(node->child);
			if (temp != NULL) {
				if (result != NULL)
					return NULL;
				else
					result = temp;
			}
		}
	}
	return result;
}


/**
 * Finds a node element at a specific location.
 *
 * \param node	     the root node to check from
 * \param x	     the x co-ordinate
 * \param y	     the y co-ordinate
 * \param furniture  whether the returned area was in an elements furniture
 * \return the node at the specified position, or NULL for none
 */
static struct node_element *tree_get_node_element_at(struct node *node,
		int x, int y, bool *furniture)
{
	struct node_element *element;
	int x0, x1, y0, y1;

	*furniture = false;
	for (; node != NULL; node = node->next) {
		if (node->box.y > y) return NULL;
		if ((node->box.x - NODE_INSTEP < x) && (node->box.y < y) &&
		    (node->box.x + node->box.width >= x) &&
		    (node->box.y + node->box.height >= y)) {
			if (node->expanded) {
				for (element = &node->data; element != NULL;
				     element = element->next) {
					x0 = element->box.x;
					y0 = element->box.y;
					x1 = element->box.x +
						element->box.width;
					y1 = element->box.y +
						element->box.height;
					if ((x0 < x) && (y0 < y) && (x1 >= x)
					    && (y1 >= y))
						return element;
				}
			} else {
				x0 = node->data.box.x;
				y0 = node->data.box.y;
				x1 = node->data.box.x +	node->data.box.width;
				y1 = node->data.box.y +	node->data.box.height;
				if ((x0 < x) && (y0 < y) && (x1 >= x) &&
				    (y1>= y))
					return &node->data;
			}
			if (((node->child != NULL) ||
			     (node->data.next != NULL)) &&
			    (node->data.box.x - NODE_INSTEP + 4 < x)
			    && (node->data.box.y + 4 < y) &&
			    (node->data.box.x > x) &&
			    (node->data.box.y + 20 > y)) {
				*furniture = true;
				return &node->data;
			}
		}

		element = tree_get_node_element_at(node->child, x, y,
						   furniture);
		if ((node->child != NULL) && (node->expanded) &&
		    (element != NULL))
			return element;
	}
	return NULL;
}


/**
 * Finds a node at a specific location.
 *
 * \param root	     the root node to check from
 * \param x	     the x co-ordinate
 * \param y	     the y co-ordinate
 * \param furniture  whether the returned area was in an elements furniture
 * \return the node at the specified position, or NULL for none
 */
static struct node *tree_get_node_at(struct node *root, int x, int y,
		bool *furniture)
{
	struct node_element *result;

	if ((result = tree_get_node_element_at(root, x, y, furniture)))
		return result->parent;
	return NULL;
}


/**
 * Gets link characteristics to insert a node at a specified position.
 *
 * \param tree	  the tree to find link information for
 * \param x	  the x co-ordinate
 * \param y	  the y co-ordinate
 * \param before  set to whether the node should be linked before on exit
 * \return        the node to link with
 */
struct node *tree_get_link_details(struct tree *tree, int x, int y,
		bool *before)
{
	struct node *node = NULL;
	bool furniture;

	assert(tree != NULL);
	assert(tree->root != NULL);

	*before = false;
	if (tree->root->child != NULL)
		node = tree_get_node_at(tree->root->child, x, y, &furniture);
	if ((node == NULL) || (furniture))
		return tree->root;

	if (y < (node->box.y + (node->box.height / 2))) {
		*before = true;
	} else if ((node->folder) && (node->expanded) &&
		   (node->child != NULL)) {
		node = node->child;
		*before = true;
	}
	return node;
}


/**
 * Launches all the selected nodes of the tree
 *
 * \param tree  the tree for which all nodes will be launched
 * \param node  the node which will be checked together with its children
 */
static void tree_launch_selected_internal(struct tree *tree, struct node *node)
{
	struct node_msg_data msg_data;

	for (; node != NULL; node = node->next) {
		if (node->selected && node->user_callback != NULL) {
			msg_data.msg = NODE_LAUNCH;
			msg_data.flag = TREE_ELEMENT_TITLE;
			msg_data.node = node;
			node->user_callback(node->callback_data, &msg_data);
		}
		if (node->child != NULL)
			tree_launch_selected_internal(tree, node->child);
	}
}


/**
 * Launches all the selected nodes of the tree
 *
 * \param tree  the tree for which all nodes will be launched
 */
void tree_launch_selected(struct tree *tree)
{
	if (tree->root->child != NULL)
		tree_launch_selected_internal(tree, tree->root->child);
}


/**
 * Handles a mouse action for a tree
 *
 * \param tree   the tree to handle a click for
 * \param mouse  the mouse state
 * \param x	 X coordinate of mouse action
 * \param y	 Y coordinate of mouse action
 * \return	 whether the click was handled
 */
bool tree_mouse_action(struct tree *tree, browser_mouse_state mouse, int x,
		int y)
{
	bool furniture;
	struct node *node;
	struct node *last;
	struct node_element *element;
	struct node_msg_data msg_data;

	assert(tree != NULL);
	assert(tree->root != NULL);

	if (tree->root->child == NULL)
		return true;

	element = tree_get_node_element_at(tree->root->child, x, y, &furniture);

	/* pass in-textarea mouse action and drags which started in it
	   to the textarea */
	if (tree->editing != NULL) {
		int x0, x1, y0, y1;
		x0 = tree->editing->box.x;
		if (tree->editing->type == NODE_ELEMENT_TEXT_PLUS_ICON)
			x0 += NODE_INSTEP;
		x1 = tree->editing->box.x + tree->editing->box.width;
		y0 = tree->editing->box.y;
		y1 = tree->editing->box.y + tree->editing->box.height;

		if (tree->textarea_drag_start &&
		    (mouse & (BROWSER_MOUSE_HOLDING_1 |
			      BROWSER_MOUSE_HOLDING_2))) {

			textarea_mouse_action(tree->textarea, mouse,
					      x - x0, y - y0);
			return true;
		}



		if ((x >= x0) && (x < x1) && (y >= y0) && (y < y1)) {

			if (mouse & (BROWSER_MOUSE_DRAG_1 |
				     BROWSER_MOUSE_DRAG_2))
				tree->textarea_drag_start = true;
			else
				tree->textarea_drag_start = false;
			textarea_mouse_action(tree->textarea, mouse,
					      x - x0, y - y0);
			return true;

		}
	}

	tree->textarea_drag_start = false;

	/* we are not interested in the drag path or in mouse presses, return */
	if (mouse & (BROWSER_MOUSE_HOLDING_1 | BROWSER_MOUSE_HOLDING_2 |
		     BROWSER_MOUSE_PRESS_1 | BROWSER_MOUSE_PRESS_2))
		return true;

	/* cancel edit */
	if (tree->editing != NULL)
 		tree_stop_edit(tree, false);



	/* no item either means cancel selection on (select) click or a drag */
	if (element == NULL) {
		if (tree->flags & TREE_SINGLE_SELECT) {
			tree_set_node_selected(tree, tree->root->child, true,
					       false);
	  		return true;
	  	}
		if (mouse & (BROWSER_MOUSE_CLICK_1 | BROWSER_MOUSE_DRAG_1))
			tree_set_node_selected(tree, tree->root->child, true,
					       false);
		if (mouse & (BROWSER_MOUSE_DRAG_1 | BROWSER_MOUSE_DRAG_2)) {

			/** @todo the tree window has to scroll the tree when
			 * mouse reaches border while dragging this isn't
			 * solved for the browser window too.
			 */
			tree->drag = TREE_SELECT_DRAG;
		}
		return true;
	}

	node = element->parent;

	/* click on furniture or double click on folder toggles node expansion
	 */
	if (((furniture) && (mouse & (BROWSER_MOUSE_CLICK_1 |
				      BROWSER_MOUSE_CLICK_2))) ||
	    ((!furniture) && (node->child != NULL) &&
	     (mouse & BROWSER_MOUSE_DOUBLE_CLICK))) {

		/* clear any selection */
		tree_set_node_selected(tree, tree->root->child, true, false);

		/* expand / contract node and redraw */
		tree_set_node_expanded(tree, node, !node->expanded,
				       false, false);

		/* find the last child node if expanded */
		last = node;
		if ((last->child != NULL) && (last->expanded)) {
			last = last->child;
			while ((last->next != NULL) ||
			       ((last->child != NULL) &&
				(last->expanded))) {
				if (last->next != NULL)
					last = last->next;
				else
					last = last->child;
			}
		}
		/* scroll to the bottom element then back to the top */
		element = &last->data;
		if (last->expanded)
			for (; element->next != NULL; element = element->next);
		tree->callbacks->scroll_visible(element->box.y,
						element->box.height,
						tree->client_data);
		tree->callbacks->scroll_visible(node->data.box.y,
						node->data.box.height,
						tree->client_data);
		return true;
	}

	/* no use for any other furniture click */
	if (furniture)
		return true;

	/* single/double ctrl+click or alt+click starts editing */
	if ((element->editable) && (!tree->editing) &&
	    ((element->type == NODE_ELEMENT_TEXT) ||
	     (element->type == NODE_ELEMENT_TEXT_PLUS_ICON)) &&
	    (mouse & (BROWSER_MOUSE_CLICK_1 |
		      BROWSER_MOUSE_DOUBLE_CLICK)) &&
	    (mouse & BROWSER_MOUSE_MOD_2 ||
	     mouse & BROWSER_MOUSE_MOD_3)) {
		tree_set_node_selected(tree, tree->root->child, true, false);
		tree_start_edit(tree, element);
		return true;
	}

	/* double click launches the leaf */
	if (mouse & BROWSER_MOUSE_DOUBLE_CLICK) {
		if (node->user_callback == NULL)
			return false;
		msg_data.msg = NODE_LAUNCH;
		msg_data.flag = TREE_ELEMENT_TITLE;
		msg_data.node = node;
		if (node->user_callback(node->callback_data, &msg_data) !=
		    NODE_CALLBACK_HANDLED)
			return false;

		return true;
	}

	/* single click (select) cancels current selection and selects item */
	if (mouse & BROWSER_MOUSE_CLICK_1 || (mouse & BROWSER_MOUSE_CLICK_2 &&
					      tree->flags & TREE_SINGLE_SELECT)) {
		if (tree->flags & TREE_NO_SELECT)
			return true;
		if (!node->selected) {
			tree_set_node_selected(tree, tree->root->child, true,
					       false);
			node->selected = true;
			tree_handle_node_element_changed(tree, &node->data);
		}
		return true;
	}

	/* single click (adjust) toggles item selection */
	if (mouse & BROWSER_MOUSE_CLICK_2) {
		if (tree->flags & TREE_NO_SELECT)
			return true;
		node->selected = !node->selected;
		tree_handle_node_element_changed(tree, &node->data);
		return true;
	}

	/* drag starts a drag operation */
	if ((!tree->editing) && (mouse & (BROWSER_MOUSE_DRAG_1 |
					  BROWSER_MOUSE_DRAG_2))) {
		if (tree->flags & TREE_NO_DRAGS)
			return true;

		if (!node->selected) {
			tree_set_node_selected(tree, tree->root->child, true,
					       false);
			node->selected = true;
			tree_handle_node_element_changed(tree, &node->data);
		}

		tree->drag = TREE_MOVE_DRAG;

		return true;
	}


	return false;
}


/**
 * Updates the selected state for a region of nodes.
 *
 * \param tree	  the tree to update
 * \param node	  the node to update children and siblings of
 * \param y	  the minimum y of the selection rectangle
 * \param height  the height of the selection rectangle
 * \param invert  whether to invert the selected state
 */
static void tree_handle_selection_area_node(struct tree *tree,
		struct node *node, int y, int height, bool invert)
{
	struct node_element *element;
	struct node *update;
	int y_max;
	int y0, y1;

	assert(tree != NULL);
	assert(node != NULL);

	y_max = y + height;

	for (; node != NULL; node = node->next) {
		if (node->box.y > y_max) return;
		y0 = node->box.y;
		y1 = node->box.y + node->box.height;
		if ((y0 < y_max) && (y1 >= y)) {
			update = NULL;
			if (node->expanded) {
				for (element = &node->data; element != NULL;
				     element = element->next) {
					y0 = element->box.y;
					y1 = element->box.y +
						element->box.height;
					if ((y0 < y_max) && (y1 >= y)) {
						update = element->parent;
						break;
					}
				}
			} else {
				y0 = node->data.box.y;
				y1 = node->data.box.y +	node->data.box.height;
				if ((y0 < y_max) && (y1 >= y))
					update = node->data.parent;
			}
			if ((update) && (node != tree->root)) {
				if (invert) {
					node->selected = !node->selected;
					tree_handle_node_element_changed(tree,
							&node->data);
				} else if (!node->selected) {
					node->selected = true;
					tree_handle_node_element_changed(tree,
							&node->data);
				}
			}
		}
		if ((node->child != NULL) && (node->expanded))
			tree_handle_selection_area_node(tree, node->child, y,
							height, invert);
	}
}


/**
 * Updates the selected state for a region of nodes.
 *
 * \param tree	  the tree to update
 * \param y	  the minimum y of the selection rectangle
 * \param height  the height of the selection rectangle
 * \param invert  whether to invert the selected state
 */
static void tree_handle_selection_area(struct tree *tree, int y, int height,
		bool invert)
{
	assert(tree != NULL);
	assert(tree->root != NULL);

	if (tree->root->child == NULL)
		return;

	if (height < 0) {
		y += height;
		height = -height;
	}
	tree_handle_selection_area_node(tree, tree->root->child, y, height,
			invert);
}


/**
 * Clears the processing flag.
 *
 * \param node	  the node to process siblings and children of
 */
static void tree_clear_processing(struct node *node)
{
	for (; node != NULL; node = node->next) {
		node->processing = false;
		if (node->child != NULL)
			tree_clear_processing(node->child);
	}
}


/**
 * Sets the processing flag to the selection state.
 *
 * \param node	  the node to process siblings and children of
 */
static void tree_selected_to_processing(struct node *node)
{
	for (; node != NULL; node = node->next) {
		node->processing = node->selected;
		if ((node->child != NULL) && (node->expanded))
			tree_selected_to_processing(node->child);
	}
}


/**
 * Moves the first node in a tree with the processing flag set.
 *
 * \param tree    the tree in which the move takes place
 * \param node	  the node to move siblings/children of
 * \param link	  the node to link before/as a child (folders) or before/after
 *		  (link)
 * \param before  whether to link siblings before or after the supplied node
 * \param first   whether to always link after the supplied node (ie not
 *		  inside of folders)
 * \return        the node moved
 */
static struct node *tree_move_processing_node(struct tree *tree,
		struct node *node, struct node *link, bool before, bool first)
{
	struct node *result;

	bool folder = link->folder;
	for (; node != NULL; node = node->next) {
	  	if (node->processing) {
	  		node->processing = false;
	  	  	tree_delink_node(tree, node);
	  	  	if (!first)
	  	  		link->folder = false;
	  	  	tree_link_node(tree, link, node, before);
	  	  	if (!first)
	  	  		link->folder = folder;
	  		return node;
	  	}
		if (node->child != NULL) {
		  	result = tree_move_processing_node(tree, node->child,
							   link, before, first);
			if (result != NULL)
		  		return result;
		}
	}
	return NULL;
}


/**
 * Moves nodes within a tree.
 *
 * \param tree	       the tree to process
 * \param destination  the node to link before/as a child (folders)
 * 		       or before/after (link)
 * \param before       whether to link siblings before or after the supplied
 *		       node
 */
static void tree_move_selected_nodes(struct tree *tree,
		struct node *destination, bool before)
{
	struct node *link;
	struct node *test;
	bool error;

	tree_clear_processing(tree->root);
	tree_selected_to_processing(tree->root);

	/* the destination node cannot be a child of any node with
	   the processing flag set */
	error = destination->processing;
	for (test = destination; test != NULL; test = test->parent)
		error |= test->processing;
	if (error) {
		tree_clear_processing(tree->root);
		return;
	}
	if ((destination->folder) && (!destination->expanded) && (!before)) {
		tree_set_node_expanded(tree, destination, true, false, false);
	}
	link = tree_move_processing_node(tree, tree->root, destination, before,
					 true);
	while (link != NULL)
		link = tree_move_processing_node(tree, tree->root, link, false,
						 false);

	tree_clear_processing(tree->root);
	tree_recalculate_node_positions(tree, tree->root);
	if (tree->redraw)
		tree->callbacks->redraw_request(0, 0, tree->width, tree->height,
						tree->client_data);
}


/**
 * Handle the end of a drag operation
 *
 * \param tree	the tree on which the drag was performed
 * \param mouse	mouse state during drag end
 * \param x0	x coordinate of drag start
 * \param y0	y coordinate of drag start
 * \param x1	x coordinate of drag end
 * \param y1	y coordinate of drag end
 */
void tree_drag_end(struct tree *tree, browser_mouse_state mouse, int x0, int y0,
		int x1, int y1)
{

	bool before;
	struct node *node;
	int x, y;

	if (tree->textarea_drag_start) {
		x = tree->editing->box.x;
		y = tree->editing->box.y;
		if (tree->editing->type == NODE_ELEMENT_TEXT_PLUS_ICON)
			x += NODE_INSTEP;
		textarea_drag_end(tree->textarea, mouse, x1 - x, y1 - y);
	}

	tree->textarea_drag_start = false;

	switch (tree->drag) {
	case TREE_NO_DRAG:
		break;
	case TREE_SELECT_DRAG:
		tree_handle_selection_area(tree, y0, y1 - y0,
					   (mouse | BROWSER_MOUSE_HOLDING_2));
		break;
	case TREE_MOVE_DRAG:
		if (!(tree->flags & TREE_MOVABLE))
			return;
		node = tree_get_link_details(tree, x1, y1, &before);
		tree_move_selected_nodes(tree, node, before);
		break;
	}

	tree->drag = TREE_NO_DRAG;
}


/**
 * Key press handling for a tree.
 *
 * \param tree	The tree which got the keypress
 * \param key	The ucs4 character codepoint
 * \return     	true if the keypress is dealt with, false otherwise.
 */
bool tree_keypress(struct tree *tree, uint32_t key)
{

	if (tree->editing != NULL)
		switch (key) {
		case KEY_ESCAPE:
			tree_stop_edit(tree, false);
			return true;
		case KEY_NL:
			tree_stop_edit(tree, true);
			return true;
		default:
			return textarea_keypress(tree->textarea, key);
		}

	return false;
}


/**
 * Alphabetical comparison function for nodes
 *
 * \param n1  first node to compare
 * \param n2  first node to compare
 * \return    0 if equal, greater then zero if n1 > n2,
 *	      less then zero if n2 < n1
 */
int tree_alphabetical_sort(struct node *n1, struct node *n2)
{
	return strcmp(n1->data.text, n2->data.text);
}


/**
 * Redraw requests from the textarea are piped through this because we have to
 * check the redraw flag of the tree before requesting a redraw and change the
 * position to tree origin relative.
 */
static void tree_textarea_redraw_request(void *data, int x, int y,
		int width, int height)
{
	struct tree *tree = data;
	x = x + tree->editing->box.x;
	y = y + tree->editing->box.y;
	if (tree->editing->type == NODE_ELEMENT_TEXT_PLUS_ICON)
		x += NODE_INSTEP;

	if (tree->redraw)
		tree->callbacks->redraw_request(x, y,
						width, height,
						tree->client_data);
}


/**
 * Starts editing a node_element
 *
 * \param tree		The tree to which element belongs
 * \param element	The element to start being edited
 */
void tree_start_edit(struct tree *tree, struct node_element *element)
{
	struct node *parent;
	int width, height;

	assert(tree != NULL);
	assert(element != NULL);

	if (tree->editing != NULL)
		tree_stop_edit(tree, true);

	parent = element->parent;
	if (&parent->data == element)
		parent = parent->parent;
	for (; parent != NULL; parent = parent->parent) {
		if (!parent->expanded) {
			tree_set_node_expanded(tree, parent, true,
					       false, false);
		}
	}

	tree->editing = element;
	tree->callbacks->get_window_dimensions(&width, NULL, tree->client_data);
	width -= element->box.x;
	height = element->box.height;
	if (element->type == NODE_ELEMENT_TEXT_PLUS_ICON)
		width -= NODE_INSTEP;

	tree->textarea = textarea_create(width, height, 0,
			&plot_fstyle, tree_textarea_redraw_request, tree);
	if (tree->textarea == NULL) {
		tree_stop_edit(tree, false);
		return;
	}
	textarea_set_text(tree->textarea, element->text);

	tree_handle_node_element_changed(tree, element);
	tree_recalculate_size(tree);
	tree->callbacks->scroll_visible(element->box.y, element->box.height,
					tree->client_data);
}


/**
 * Callback for fetchcache(). Should be removed once bitmaps get loaded directly
 * from disc
 */
static nserror tree_icon_callback(hlcache_handle *handle,
		const hlcache_event *event, void *pw)
{
	return NSERROR_OK;
}


/**
 * Tree utility function. Placed here so that this code doesn't have to be
 * copied by each user.
 *
 * \param name	the name of the loaded icon, if it's not a full path the icon is
 *		looked for in the directory specified by option_tree_icons_dir
 * \return the icon in form of a content or NULL on failure
 */
hlcache_handle *tree_load_icon(const char *name)
{
	char *url = NULL;
	const char *icon_url = NULL;
	int len;
	hlcache_handle *c;
	nserror err;

	/** @todo something like bitmap_from_disc is needed here */

	if (!strncmp(name, "file://", 7)) {
		icon_url = name;
	} else {
		char *native_path;

		if (option_tree_icons_dir == NULL)
			return NULL;

		/* path + separator + leafname + '\0' */
		len = strlen(option_tree_icons_dir) + 1 + strlen(name) + 1;
		native_path = malloc(len);
		if (native_path == NULL) {
			LOG(("malloc failed"));
			warn_user("NoMemory", 0);
			return NULL;
		}

		/* Build native path */
		memcpy(native_path, option_tree_icons_dir,
		       strlen(option_tree_icons_dir) + 1);
		path_add_part(native_path, len, name);

		/* Convert native path to URL */
		url = path_to_url(native_path);

		free(native_path);
		icon_url = url;
	}

	/* Fetch the icon */
	err = hlcache_handle_retrieve(icon_url, 0, 0, 0,
				      tree_icon_callback, 0, 0, 0, &c);


	/* If we built the URL here, free it */
	if (url != NULL)
		free(url);

	if (err != NSERROR_OK) {
		return NULL;
	}

	return c;
}
