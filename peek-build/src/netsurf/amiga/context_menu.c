/*
 * Copyright 2008-9 Chris Young <chris@unsatisfactorysoftware.co.uk>
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

#include "amiga/context_menu.h"
#include "render/box.h"
#include "render/form.h"
#include <proto/popupmenu.h>
#include <proto/intuition.h>
#include "amiga/utf8.h"
#include "utils/utf8.h"
#include "utils/messages.h"
#include "amiga/options.h"
#include "amiga/clipboard.h"
#include <proto/asl.h>
#include <proto/dos.h>
#include <string.h>
#include "utils/utils.h"
#include <proto/asl.h>
#include "desktop/textinput.h"
#include "desktop/selection.h"
#include "amiga/bitmap.h"
#include "amiga/iff_dr2d.h"

uint32 ami_context_menu_hook(struct Hook *hook,Object *item,APTR reserved);

char *ctxmenulab[CMID_LAST];

void ami_context_menu_init(void)
{
	ctxmenulab[CMID_SELECTFILE] = ami_utf8_easy((char *)messages_get("SelectFile"));
	ctxmenulab[CMID_COPYURL] = ami_utf8_easy((char *)messages_get("CopyURL"));
	ctxmenulab[CMID_SHOWOBJ] = ami_utf8_easy((char *)messages_get("ObjShow"));
	ctxmenulab[CMID_COPYOBJ] = ami_utf8_easy((char *)messages_get("CopyURL"));
	ctxmenulab[CMID_CLIPOBJ] = ami_utf8_easy((char *)messages_get("CopyClip"));
	ctxmenulab[CMID_SAVEOBJ] = ami_utf8_easy((char *)messages_get("SaveAs"));
	ctxmenulab[CMID_SAVEIFFOBJ] = ami_utf8_easy((char *)messages_get("SaveIFF"));

	ctxmenulab[CMID_SAVEURL] = ami_utf8_easy((char *)messages_get("LinkDload"));
	ctxmenulab[CMID_URLOPENWIN] = ami_utf8_easy((char *)messages_get("LinkNewWin"));
	ctxmenulab[CMID_URLOPENTAB] = ami_utf8_easy((char *)messages_get("LinkNewTab"));

	ctxmenulab[CMID_SELCUT] = ami_utf8_easy((char *)messages_get("CutNS"));
	ctxmenulab[CMID_SELCOPY] = ami_utf8_easy((char *)messages_get("CopyNS"));
	ctxmenulab[CMID_SELPASTE] = ami_utf8_easy((char *)messages_get("PasteNS"));
	ctxmenulab[CMID_SELALL] = ami_utf8_easy((char *)messages_get("SelectAllNS"));
	ctxmenulab[CMID_SELCLEAR] = ami_utf8_easy((char *)messages_get("ClearNS"));

	ctxmenulab[CMSUB_OBJECT] = ami_utf8_easy((char *)messages_get("Object"));
	ctxmenulab[CMSUB_URL] = ami_utf8_easy((char *)messages_get("Link"));
	ctxmenulab[CMSUB_SEL] = ami_utf8_easy((char *)messages_get("Selection"));
}

void ami_context_menu_free(void)
{
	int i;

	for(i=0;i<CMID_LAST;i++)
	{
		ami_utf8_free(ctxmenulab[i]);
	}
}

void ami_context_menu_show(struct gui_window_2 *gwin,int x,int y)
{
	struct hlcache_handle *cc = gwin->bw->current_content;
	struct box *curbox;
	int box_x=0;
	int box_y=0;
	bool menuhascontent = false;

	if(!cc) return;
	if(content_get_type(cc) != CONTENT_HTML) return;

	if(gwin->objects[OID_MENU]) DisposeObject(gwin->objects[OID_MENU]);

	gwin->popuphook.h_Entry = ami_context_menu_hook;
	gwin->popuphook.h_Data = gwin;

    gwin->objects[OID_MENU] = NewObject( POPUPMENU_GetClass(), NULL,
                        PMA_MenuHandler, &gwin->popuphook,
						TAG_DONE);

	curbox = html_get_box_tree(gwin->bw->current_content);

	while(curbox = box_at_point(curbox,x,y,&box_x,&box_y,&cc))
	{
		if (curbox->style && css_computed_visibility(curbox->style) == CSS_VISIBILITY_HIDDEN)	continue;

		if(curbox->href)
		{
			IDoMethod(gwin->objects[OID_MENU],PM_INSERT,
				NewObject(POPUPMENU_GetItemClass(), NULL,
					PMIA_Title, (ULONG)ctxmenulab[CMSUB_URL],
					PMSIMPLESUB,
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_URLOPENWIN],
							PMIA_ID,CMID_URLOPENWIN,
							PMIA_UserData,curbox->href,
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_URLOPENTAB],
							PMIA_ID,CMID_URLOPENTAB,
							PMIA_UserData,curbox->href,
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_COPYURL],
							PMIA_ID,CMID_COPYURL,
							PMIA_UserData,curbox->href,
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SAVEURL],
							PMIA_ID,CMID_SAVEURL,
							PMIA_UserData,curbox->href,
						TAG_DONE),
					TAG_DONE),
				TAG_DONE),
				~0);

			menuhascontent = true;
		}

		if (curbox->object)
		{
			IDoMethod(gwin->objects[OID_MENU],PM_INSERT,
				NewObject(POPUPMENU_GetItemClass(), NULL,
					PMIA_Title, (ULONG)ctxmenulab[CMSUB_OBJECT],
					PMSIMPLESUB,
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SHOWOBJ],
							PMIA_ID,CMID_SHOWOBJ,
							PMIA_UserData, content_get_url(curbox->object),
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_COPYOBJ],
							PMIA_ID,CMID_COPYOBJ,
							PMIA_UserData, content_get_url(curbox->object),
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_CLIPOBJ],
							PMIA_ID,CMID_CLIPOBJ,
							PMIA_UserData,curbox->object,
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SAVEOBJ],
							PMIA_ID,CMID_SAVEOBJ,
							PMIA_UserData,curbox->object,
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SAVEIFFOBJ],
							PMIA_ID,CMID_SAVEIFFOBJ,
							PMIA_UserData,curbox->object,
						TAG_DONE),
					TAG_DONE),
				TAG_DONE),
				~0);

			menuhascontent = true;
		}

		if(curbox->text)
		{
			BOOL disabled_readonly = selection_read_only(gwin->bw->sel);
			BOOL disabled_noselection = !selection_defined(gwin->bw->sel);

			IDoMethod(gwin->objects[OID_MENU],PM_INSERT,
				NewObject(POPUPMENU_GetItemClass(), NULL,
					PMIA_Title, (ULONG)ctxmenulab[CMSUB_SEL],
					PMIA_SubMenu, NewObject(POPUPMENU_GetClass(), NULL,
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SELCUT],
							PMIA_ID,CMID_SELCUT,
							PMIA_Disabled, disabled_noselection && disabled_readonly,
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SELCOPY],
							PMIA_ID,CMID_SELCOPY,
							PMIA_Disabled, disabled_noselection,
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SELPASTE],
							PMIA_ID,CMID_SELPASTE,
							PMIA_Disabled, (gwin->bw->window->c_h == 0),
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SELALL],
							PMIA_ID,CMID_SELALL,
							//PMIA_UserData,curbox->href,
						TAG_DONE),
						PMA_AddItem,NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SELCLEAR],
							PMIA_ID,CMID_SELCLEAR,
							PMIA_Disabled, disabled_noselection,
						TAG_DONE),
					TAG_DONE),
				TAG_DONE),
				~0);

			menuhascontent = true;
		}

		if (curbox->gadget)
		{
			switch (curbox->gadget->type)
			{
				case GADGET_FILE:
					IDoMethod(gwin->objects[OID_MENU],PM_INSERT,
						NewObject(POPUPMENU_GetItemClass(), NULL,
							PMIA_Title, (ULONG)ctxmenulab[CMID_SELECTFILE],
							PMIA_ID,CMID_SELECTFILE,
							PMIA_UserData,curbox,
							TAG_DONE),
						~0);

					menuhascontent = true;
				break;
			}
		}
	}

	if(!menuhascontent) return;

	gui_window_set_pointer(gwin->bw->window,GUI_POINTER_DEFAULT);
	IDoMethod(gwin->objects[OID_MENU],PM_OPEN,gwin->win);
}

uint32 ami_context_menu_hook(struct Hook *hook,Object *item,APTR reserved)
{
    int32 itemid = 0;
	struct gui_window_2 *gwin = hook->h_Data;
	APTR userdata = NULL;
	struct browser_window *bw;
	struct hlcache_handle *object;
	char *source_data;
	ULONG source_size;
	struct bitmap *bm;

    if(GetAttrs(item,PMIA_ID,&itemid,
					PMIA_UserData,&userdata,
					TAG_DONE))
    {
		switch(itemid)
		{
			case CMID_SELECTFILE:
				if(AslRequestTags(filereq,
					ASLFR_TitleText,messages_get("NetSurf"),
					ASLFR_Screen,scrn,
					ASLFR_DoSaveMode,FALSE,
					TAG_DONE))
				{
					struct box *box = userdata;
					char *utf8_fn;
					char fname[1024];
					int x,y;

					strlcpy(&fname,filereq->fr_Drawer,1024);
					AddPart(fname,filereq->fr_File,1024);

					if(utf8_from_local_encoding(fname,0,&utf8_fn) != UTF8_CONVERT_OK)
					{
						warn_user("NoMemory","");
						break;
					}

					free(box->gadget->value);
					box->gadget->value = utf8_fn;

					box_coords(box, (int *)&x, (int *)&y);
					gui_window_redraw(gwin->bw->window,x,y,
						x + box->width,
						y + box->height);
				}
			break;

			case CMID_COPYURL:
			case CMID_COPYOBJ:
				ami_easy_clipboard((char *)userdata);
			break;

			case CMID_URLOPENWIN:
				bw = browser_window_create(userdata, gwin->bw,
					content_get_url(gwin->bw->current_content), true, false);
			break;

			case CMID_URLOPENTAB:
				bw = browser_window_create(userdata, gwin->bw,
					content_get_url(gwin->bw->current_content), true, true);
			break;

			case CMID_SAVEURL:
				browser_window_download(gwin->bw, userdata,
					content_get_url(gwin->bw->current_content));
			break;

			case CMID_SHOWOBJ:
				browser_window_go(gwin->bw, userdata,
					content_get_url(gwin->bw->current_content), true);
			break;

			case CMID_CLIPOBJ:
				object = (struct hlcache_handle *)userdata;
				if((bm = content_get_bitmap(object)))
				{
					bm->url = content_get_url(object);
					bm->title = content_get_title(object);
					ami_easy_clipboard_bitmap(bm);
				}
#ifdef WITH_NS_SVG
				else if(content_get_type(object) == CONTENT_SVG)
				{
					ami_easy_clipboard_svg(object);
				}
#endif
			break;

			case CMID_SAVEOBJ:
				object = (struct hlcache_handle *)userdata;

				if(AslRequestTags(savereq,
							ASLFR_TitleText,messages_get("NetSurf"),
							ASLFR_Screen,scrn,
							ASLFR_InitialFile,FilePart(content_get_url(object)),
							TAG_DONE))
				{
					BPTR fh = 0;
					char fname[1024];
					strlcpy(&fname,savereq->fr_Drawer,1024);
					AddPart(fname,savereq->fr_File,1024);
					ami_update_pointer(gwin->win,GUI_POINTER_WAIT);
					if(fh = FOpen(fname,MODE_NEWFILE,0))
					{
						if((source_data = content_get_source_data(object, &source_size)))
							FWrite(fh, source_data, 1, source_size);

						FClose(fh);
						SetComment(fname, content_get_url(object));
					}
					ami_update_pointer(gwin->win,GUI_POINTER_DEFAULT);
				}
			break;

			case CMID_SAVEIFFOBJ:
				object = (struct hlcache_handle *)userdata;

				if(AslRequestTags(savereq,
							ASLFR_TitleText,messages_get("NetSurf"),
							ASLFR_Screen,scrn,
							ASLFR_InitialFile,FilePart(content_get_url(object)),
							TAG_DONE))
				{
					BPTR fh = 0;
					char fname[1024];

					strlcpy(&fname,savereq->fr_Drawer,1024);
					AddPart(fname,savereq->fr_File,1024);
					if((bm = content_get_bitmap(object)))
					{
						bm->url = content_get_url(object);
						bm->title = content_get_title(object);
						bitmap_save(bm, fname, 0);
					}
#ifdef WITH_NS_SVG
					else if(content_get_type(object) == CONTENT_SVG)
					{
						ami_save_svg(object,fname);
					}
#endif
					SetComment(fname, content_get_url(object));
					ami_update_pointer(gwin->win,GUI_POINTER_DEFAULT);
				}
			break;

			case CMID_SELCUT:
				browser_window_key_press(gwin->bw, KEY_CUT_SELECTION);
			break;

			case CMID_SELCOPY:
				browser_window_key_press(gwin->bw, KEY_COPY_SELECTION);
				browser_window_key_press(gwin->bw, KEY_CLEAR_SELECTION);
			break;

			case CMID_SELPASTE:
				browser_window_key_press(gwin->bw, KEY_PASTE);
			break;

			case CMID_SELALL:
				browser_window_key_press(gwin->bw, KEY_SELECT_ALL);
				gui_start_selection(gwin->bw->window);
			break;

			case CMID_SELCLEAR:
				browser_window_key_press(gwin->bw, KEY_CLEAR_SELECTION);
			break;
		}
    }

    return itemid;
}
