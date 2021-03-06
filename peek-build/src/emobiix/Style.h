#ifndef _STYLE_H_
#define _STYLE_H_

/*#include "Widget_internal.h"*/
#include "WidgetRenderer.h"
#include "Gradient.h"
#include "DataObject.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "Style_internal.h"
struct WidgetRenderer_t;

/*Style *style_new(void);*/

void style_renderWidgetTree(Style *s, Widget *w);
Style *style_getID(Style *styleRoot, const char *type, const char *id, int isFocused,
		int *wentUp);
DataObjectField *style_getProperty(Style *s, DataObject *dobj, EmoField eenum);
DataObjectField *style_getPropertyAsInt(Style *s, DataObject *dobj, EmoField eenum);
DataObjectField *style_getColor(Style *s, DataObject *dobj, EmoField eenum,
		unsigned int *color);
DataObjectField *style_getRenderer(Style *s, DataObject *dobj, EmoField eenum,
		struct WidgetRenderer_t **wr);
Gradient *style_getGradient(Style *s);

/*void style_setProperty(Style *s, const char *className, const char *id,
		const char *type, const char *lvalue, void *value);
void *style_getProperty(Style *s, const char *className, const char *id,
		const char *type, const char *lvalue);*/

/* dont make sense here but ok for now Focus.h ? */
void widget_focusNext(Widget *w, Style *s);
void widget_focusPrev(Widget *w, Style *s);
void widget_focusNone(Widget *w, Style *s, int doRender);
Widget * widget_focusWhichOne(Widget *w);

#ifdef __cplusplus
}
#endif

#endif /* _STYLE_H_ */

