/*
 * Copyright 2008-2010 Chris Young <chris@unsatisfactorysoftware.co.uk>
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

#include "desktop/gui.h"
#include "utils/utf8.h"
#include "desktop/selection.h"
#include "desktop/textinput.h"

#include "amiga/iff_cset.h"
#include "amiga/options.h"
#include "amiga/gui.h"
#include "amiga/utf8.h"
#include "amiga/bitmap.h"
#include "amiga/iff_dr2d.h"
#include "amiga/menu.h"

#include <proto/iffparse.h>
#include <proto/intuition.h>
#include <proto/exec.h>
#include <proto/datatypes.h>
#include <proto/diskfont.h>

#include <diskfont/diskfonttag.h>
#include <datatypes/textclass.h>
#include <datatypes/pictureclass.h>

struct IFFHandle *iffh = NULL;

bool ami_add_to_clipboard(const char *text, size_t length, bool space);

void ami_clipboard_init(void)
{
	if(iffh = AllocIFF())
	{
		if(iffh->iff_Stream = (ULONG)OpenClipboard(0))
		{
			InitIFFasClip(iffh);
		}
	}
}

void ami_clipboard_free(void)
{
	if(iffh->iff_Stream) CloseClipboard((struct ClipboardHandle *)iffh->iff_Stream);
	if(iffh) FreeIFF(iffh);
}

void gui_start_selection(struct gui_window *g)
{
	if(!g) return;
	if(!g->shared->win) return;

	OnMenu(g->shared->win, AMI_MENU_CLEAR);
	OnMenu(g->shared->win, AMI_MENU_COPY);

	if(selection_read_only(g->shared->bw->sel) == false)
		OnMenu(g->shared->win, AMI_MENU_CUT);
}

void gui_clear_selection(struct gui_window *g)
{
	if(!g) return;
	if(!g->shared->win) return;

	OffMenu(g->shared->win, AMI_MENU_CLEAR);
	OffMenu(g->shared->win, AMI_MENU_CUT);
	OffMenu(g->shared->win, AMI_MENU_COPY);
}

void gui_paste_from_clipboard(struct gui_window *g, int x, int y)
{
	/* This and the other clipboard code is heavily based on the RKRM examples */
	struct ContextNode *cn;
	ULONG rlen=0,error;
	struct CSet cset;
	char *clip;
	STRPTR readbuf = AllocVec(1024,MEMF_PRIVATE | MEMF_CLEAR);

	cset.CodeSet = 0;

	if(OpenIFF(iffh,IFFF_READ)) return;
	if(StopChunk(iffh,ID_FTXT,ID_CHRS)) return;
	if(StopChunk(iffh,ID_FTXT,ID_CSET)) return;

	while(1)
	{
		error = ParseIFF(iffh,IFFPARSE_SCAN);
		if(error == IFFERR_EOC) continue;
		else if(error) break;

		cn = CurrentChunk(iffh);

		if((cn)&&(cn->cn_Type == ID_FTXT)&&(cn->cn_ID == ID_CSET))
		{
			rlen = ReadChunkBytes(iffh,&cset,24);
		}

		if((cn)&&(cn->cn_Type == ID_FTXT)&&(cn->cn_ID == ID_CHRS))
		{
			while((rlen = ReadChunkBytes(iffh,readbuf,1024)) > 0)
			{
				if(cset.CodeSet == 0)
				{
					utf8_from_local_encoding(readbuf,rlen,&clip);
				}
				else
				{
					utf8_from_enc(readbuf,
						ObtainCharsetInfo(DFCS_NUMBER, cset.CodeSet, DFCS_MIMENAME),
						rlen, &clip);
				}

				browser_window_paste_text(g->shared->bw,clip,rlen,true);
			}
			if(rlen < 0) error = rlen;
		}
	}
	CloseIFF(iffh);
}

bool gui_empty_clipboard(void)
{
	/* Put a half-completed FTXT on the clipboard and leave it open for more additions */

	struct CSet cset = {0};

	if(!(OpenIFF(iffh,IFFF_WRITE)))
	{
		if(!(PushChunk(iffh,ID_FTXT,ID_FORM,IFFSIZE_UNKNOWN)))
		{
			if(option_utf8_clipboard)
			{
				if(!(PushChunk(iffh,0,ID_CSET,24)))
				{
					cset.CodeSet = 106; // UTF-8
					WriteChunkBytes(iffh,&cset,24);
					PopChunk(iffh);
				}
			}
		}
		else
		{
			PopChunk(iffh);
			return false;
		}
		return true;
	}
	return false;
}

bool gui_add_to_clipboard(const char *text, size_t length, bool space)
{
	/* This might crash or at least not work if gui_empty_clipboard isn't called first,
	   and gui_commit_clipboard after.
	   These only seem to be called from desktop/textinput.c in this specific order, if they
	   are added elsewhere this might need a rewrite. */

	if(!(PushChunk(iffh,0,ID_CHRS,IFFSIZE_UNKNOWN)))
	{
		if(text)
		{
			if(!ami_add_to_clipboard(text, length, false)) return false;
		}

		PopChunk(iffh);
	}
	else
	{
		PopChunk(iffh);
		return false;
	}

	return true;
}

bool ami_add_to_clipboard(const char *text, size_t length, bool space)
{
	char *buffer;

	if(option_utf8_clipboard)
	{
		WriteChunkBytes(iffh,text,length);
	}
	else
	{
		utf8_to_local_encoding(text,length,&buffer);
		if(buffer) WriteChunkBytes(iffh,buffer,strlen(buffer));
		ami_utf8_free(buffer);
	}

	if(space) WriteChunkBytes(iffh," ",1);

	return true;
}

bool gui_commit_clipboard(void)
{
	if(iffh) CloseIFF(iffh);

	return true;
}

bool ami_clipboard_copy(const char *text, size_t length, struct box *box,
	void *handle, const char *whitespace_text,size_t whitespace_length)
{
	if(!(PushChunk(iffh,0,ID_CHRS,IFFSIZE_UNKNOWN)))
	{
		if (whitespace_text)
		{
			if(!ami_add_to_clipboard(whitespace_text,whitespace_length, false)) return false;
		}

		if(text)
		{
			if (!ami_add_to_clipboard(text, length, box->space)) return false;
		}

		PopChunk(iffh);
	}
	else
	{
		PopChunk(iffh);
		return false;
	}

	return true;
}

bool gui_copy_to_clipboard(struct selection *s)
{
	if(!gui_empty_clipboard()) return false;

	if (s->defined && selection_traverse(s, ami_clipboard_copy, NULL))
	{
		gui_commit_clipboard();
		return true;
	}

	return false;
}

void ami_drag_selection(struct selection *s)
{
	struct box *text_box;
	ULONG x;
	ULONG y;
	struct gui_window_2 *gwin = ami_window_at_pointer();

	if(!gwin)
	{
		DisplayBeep(scrn);
		return;
	}

	x = gwin->win->MouseX;
	y = gwin->win->MouseY;

	if(text_box = ami_text_box_at_point(gwin, &x, &y))
	{
		if(gui_copy_to_clipboard(s))
		{
			browser_window_mouse_click(gwin->bw, BROWSER_MOUSE_PRESS_1, x, y);
			browser_window_key_press(gwin->bw, KEY_PASTE);
		}
	}
	else
	{
		DisplayBeep(scrn);
	}
}

bool ami_easy_clipboard(char *text)
{
	if(!gui_empty_clipboard()) return false;
	if(!gui_add_to_clipboard(text,strlen(text),false)) return false;
	if(!gui_commit_clipboard()) return false;

	return true;
}

bool ami_easy_clipboard_bitmap(struct bitmap *bitmap)
{
	Object *dto = NULL;

	if(dto = ami_datatype_object_from_bitmap(bitmap))
	{
		DoDTMethod(dto,NULL,NULL,DTM_COPY,NULL);
		DisposeDTObject(dto);
	}
}

#ifdef WITH_NS_SVG
bool ami_easy_clipboard_svg(struct hlcache_handle *c)
{
	char *source_data;
	ULONG source_size;

	if(content_get_type(c) != CONTENT_SVG) return false;
	if((source_data = content_get_source_data(c, &source_size)) == NULL) return false;

	if(!(OpenIFF(iffh,IFFF_WRITE)))
	{
		ami_svg_to_dr2d(iffh, source_data, source_size, content_get_url(c));
		CloseIFF(iffh);
	}

	return true;
}
#endif
