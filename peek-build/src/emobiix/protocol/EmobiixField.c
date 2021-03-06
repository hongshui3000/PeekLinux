#include "EmobiixField.h"

#include <string.h>

typedef struct EmoFieldDesc_t
{
	EmoField fieldId;
	const char *fieldName;
} EmoFieldDesc;

EmoFieldDesc EmoFieldMap[EMO_FIELD_UNKNOWN_FIELD] = 
{
	{ EMO_FIELD_ERRORNONE, "ERROR-NONE" },
	{ EMO_FIELD_ALIGNMENT, EMO_FIELD_ALIGNMENT_STR },
	{ EMO_FIELD_ALPHA, EMO_FIELD_ALPHA_STR },
	{ EMO_FIELD_ARRAY, EMO_FIELD_ARRAY_STR },
	{ EMO_FIELD_ARRAYSOURCE, EMO_FIELD_ARRAYSOURCE_STR },
	{ EMO_FIELD_AUTOSCROLL, EMO_FIELD_AUTOSCROLL_STR },
	{ EMO_FIELD_BACKGROUND_COLOR, EMO_FIELD_BACKGROUND_COLOR_STR },
	{ EMO_FIELD_BOLD, EMO_FIELD_BOLD_STR },
	{ EMO_FIELD_BORDER, EMO_FIELD_BORDER_STR },
	{ EMO_FIELD_BORDER_COLOR, EMO_FIELD_BORDER_COLOR_STR },
	{ EMO_FIELD_BORDER_CORNERS, EMO_FIELD_BORDER_CORNERS_STR },
	{ EMO_FIELD_BOTTOM, EMO_FIELD_BOTTOM_STR },
	{ EMO_FIELD_BOX, EMO_FIELD_BOX_STR },
	{ EMO_FIELD_BOX_RENDERER, EMO_FIELD_BOX_RENDERER_STR },
	{ EMO_FIELD_BUTTON, EMO_FIELD_BUTTON_STR },
	{ EMO_FIELD_CANFOCUS, EMO_FIELD_CANFOCUS_STR },
	{ EMO_FIELD_CENTER, EMO_FIELD_CENTER_STR },
	{ EMO_FIELD_CHECK_COLOR, EMO_FIELD_CHECK_COLOR_STR },
	{ EMO_FIELD_CHECKBOX, EMO_FIELD_CHECKBOX_STR },
	{ EMO_FIELD_CHECKFIELD, EMO_FIELD_CHECKFIELD_STR },
	{ EMO_FIELD_CHECKVALUE, EMO_FIELD_CHECKVALUE_STR },
	{ EMO_FIELD_COLOR, EMO_FIELD_COLOR_STR },
	{ EMO_FIELD_CURSOR, EMO_FIELD_CURSOR_STR },
	{ EMO_FIELD_CURSOR_COLOR, EMO_FIELD_CURSOR_COLOR_STR },
	{ EMO_FIELD_DATA, EMO_FIELD_DATA_STR },
	{ EMO_FIELD_DATAFIELD, EMO_FIELD_DATAFIELD_STR },
	{ EMO_FIELD_DIRECTION, EMO_FIELD_DIRECTION_STR },
	{ EMO_FIELD_ENDINDEX, EMO_FIELD_ENDINDEX_STR },
	{ EMO_FIELD_ENTRY, EMO_FIELD_ENTRY_STR },
	{ EMO_FIELD_FALSE, EMO_FIELD_FALSE_STR },
	{ EMO_FIELD_FIELDNAME, EMO_FIELD_FIELDNAME_STR },
	{ EMO_FIELD_FIELDVALUE, EMO_FIELD_FIELDVALUE_STR },
	{ EMO_FIELD_FILL, EMO_FIELD_FILL_STR },
	{ EMO_FIELD_FILTERFIELD, EMO_FIELD_FILTERFIELD_STR },
	{ EMO_FIELD_FILTERVALUE, EMO_FIELD_FILTERVALUE_STR },
	{ EMO_FIELD_FIXED, EMO_FIELD_FIXED_STR },
	{ EMO_FIELD_FOCUSINDEX, EMO_FIELD_FOCUSINDEX_STR },
	{ EMO_FIELD_FOCUSSTACK, EMO_FIELD_FOCUSSTACK_STR },
	{ EMO_FIELD_FONT, EMO_FIELD_FONT_STR },
	{ EMO_FIELD_FONT_ALIGNMENT, EMO_FIELD_FONT_ALIGNMENT_STR },
	{ EMO_FIELD_FONT_COLOR, EMO_FIELD_FONT_COLOR_STR },
	{ EMO_FIELD_FRAME, EMO_FIELD_FRAME_STR },
	{ EMO_FIELD_GRADBOX, EMO_FIELD_GRADBOX_STR },
	{ EMO_FIELD_GRADIENT, EMO_FIELD_GRADIENT_STR },
	{ EMO_FIELD_HASFOCUS, EMO_FIELD_HASFOCUS_STR },
	{ EMO_FIELD_HEIGHT, EMO_FIELD_HEIGHT_STR },
	{ EMO_FIELD_HGRID, EMO_FIELD_HGRID_STR },
	{ EMO_FIELD_HORIZONTAL, EMO_FIELD_HORIZONTAL_STR },
	{ EMO_FIELD_ID, EMO_FIELD_ID_STR },
	{ EMO_FIELD_IMAGE, EMO_FIELD_IMAGE_STR },
	{ EMO_FIELD_IMGHEIGHT, EMO_FIELD_IMGHEIGHT_STR },
	{ EMO_FIELD_IMGWIDTH, EMO_FIELD_IMGWIDTH_STR },
	{ EMO_FIELD_IMGX, EMO_FIELD_IMGX_STR },
	{ EMO_FIELD_IMGY, EMO_FIELD_IMGY_STR },
	{ EMO_FIELD_LABEL, EMO_FIELD_LABEL_STR },
	{ EMO_FIELD_LEFT, EMO_FIELD_LEFT_STR },
	{ EMO_FIELD_MARGIN, EMO_FIELD_MARGIN_STR },
	{ EMO_FIELD_MARGINBOTTOM, EMO_FIELD_MARGINBOTTOM_STR },
	{ EMO_FIELD_MARGINLEFT, EMO_FIELD_MARGINLEFT_STR },
	{ EMO_FIELD_MARGINRIGHT, EMO_FIELD_MARGINRIGHT_STR },
	{ EMO_FIELD_MARGINTOP, EMO_FIELD_MARGINTOP_STR },
	{ EMO_FIELD_MULTILINE, EMO_FIELD_MULTILINE_STR },
	{ EMO_FIELD_NAME, EMO_FIELD_NAME_STR },
	{ EMO_FIELD_NONE, EMO_FIELD_NONE_STR },
	{ EMO_FIELD_ORIENTATION, EMO_FIELD_ORIENTATION_STR },
	{ EMO_FIELD_PACKING, EMO_FIELD_PACKING_STR },
	{ EMO_FIELD_PIXELFORMAT, EMO_FIELD_PIXELFORMAT_STR },
	{ EMO_FIELD_POSITION, EMO_FIELD_POSITION_STR },
	{ EMO_FIELD_PREPEND, EMO_FIELD_PREPEND_STR },
	{ EMO_FIELD_RADIUS, EMO_FIELD_RADIUS_STR },
	{ EMO_FIELD_REFERENCE, EMO_FIELD_REFERENCE_STR },
	{ EMO_FIELD_RENDERER, EMO_FIELD_RENDERER_STR },
	{ EMO_FIELD_RIGHT, EMO_FIELD_RIGHT_STR },
	{ EMO_FIELD_ROUNDED, EMO_FIELD_ROUNDED_STR },
	{ EMO_FIELD_SCROLLBAR, EMO_FIELD_SCROLLBAR_STR },
	{ EMO_FIELD_SCROLLED, EMO_FIELD_SCROLLED_STR },
	{ EMO_FIELD_SET, EMO_FIELD_SET_STR },
	{ EMO_FIELD_SOLID, EMO_FIELD_SOLID_STR },
	{ EMO_FIELD_STACK, EMO_FIELD_STACK_STR },
	{ EMO_FIELD_STARTCOUNT, EMO_FIELD_STARTCOUNT_STR },
	{ EMO_FIELD_STARTINDEX, EMO_FIELD_STARTINDEX_STR },
	{ EMO_FIELD_STENCIL, EMO_FIELD_STENCIL_STR },
	{ EMO_FIELD_TEXT, EMO_FIELD_TEXT_STR },
	{ EMO_FIELD_THUMB_COLOR, EMO_FIELD_THUMB_COLOR_STR },
	{ EMO_FIELD_TOP, EMO_FIELD_TOP_STR },
	{ EMO_FIELD_TRACK_COLOR, EMO_FIELD_TRACK_COLOR_STR },
	{ EMO_FIELD_TRANSPARENCY, EMO_FIELD_TRANSPARENCY_STR },
	{ EMO_FIELD_TYPE, EMO_FIELD_TYPE_STR },
	{ EMO_FIELD_VERTICAL, EMO_FIELD_VERTICAL_STR },
	{ EMO_FIELD_VGRID, EMO_FIELD_VGRID_STR },
	{ EMO_FIELD_VIEW, EMO_FIELD_VIEW_STR },
	{ EMO_FIELD_WEIGHT, EMO_FIELD_WEIGHT_STR },
	{ EMO_FIELD_WIDTH, EMO_FIELD_WIDTH_STR },
	{ EMO_FIELD_X, EMO_FIELD_X_STR },
	{ EMO_FIELD_XOFFSET, EMO_FIELD_XOFFSET_STR },
	{ EMO_FIELD_Y, EMO_FIELD_Y_STR },
	{ EMO_FIELD_YOFFSET, EMO_FIELD_YOFFSET_STR },
	{ EMO_FIELD_ICONIFIED, EMO_FIELD_ICONIFIED_STR },
	{ EMO_FIELD_STYLE, EMO_FIELD_STYLE_STR },
	{ EMO_FIELD_STARTUPVIEW, EMO_FIELD_STARTUPVIEW_STR },
	{ EMO_FIELD_FULLSCREEN, EMO_FIELD_FULLSCREEN_STR },
	{ EMO_FIELD_URL, EMO_FIELD_URL_STR },
	{ EMO_FIELD_INDEX, EMO_FIELD_INDEX_STR },
	{ EMO_FIELD_MIMETYPE, EMO_FIELD_MIMETYPE_STR },
	{ EMO_FIELD_DESCRIPTION, EMO_FIELD_DESCRIPTION_STR },
	{ EMO_FIELD_ONLOAD, EMO_FIELD_ONLOAD_STR },
	{ EMO_FIELD_ACCESSKEY, EMO_FIELD_ACCESSKEY_STR },
	{ EMO_FIELD_ONRETURN, EMO_FIELD_ONRETURN_STR },
	{ EMO_FIELD_ICON, EMO_FIELD_ICON_STR },
	{ EMO_FIELD_SETITEM, EMO_FIELD_SETITEM_STR },
	{ EMO_FIELD_ONSYNCFINISHED, EMO_FIELD_ONSYNCFINISHED_STR },
};

const char *emo_field_to_string(int fieldId)
{
	if (fieldId <= 0 || fieldId >= EMO_FIELD_UNKNOWN_FIELD)
		return NULL;

	return EmoFieldMap[fieldId].fieldName;
}

int emo_field_to_int(const char *fieldName)
{
	int fieldId = EMO_FIELD_ERRORNONE + 1;

	if (!fieldName || !fieldName[0])
		return EMO_FIELD_UNKNOWN_FIELD;

	for ( ; fieldId < EMO_FIELD_UNKNOWN_FIELD; ++fieldId)
		if (!strcmp(fieldName, EmoFieldMap[fieldId].fieldName))
			return fieldId;

	return EMO_FIELD_UNKNOWN_FIELD;
}


const char *EMO_TYPE_NAMES =
	"view\0"
	"entry\0"
	"checkbox\0"
	"box\0"
	"application\0"
	"script\0"
	"style\0"
	"image\0"
	"label\0"
	"stack\0"
	"set\0"
	"setitem\0"
	"text\0"
	"array\0"
	"frame\0"
	"reference\0"
	"scrolled\0"
	"gradient\0"
	"data\0"
	"\0";


const char *EMO_TYPE_VIEW = NULL;
const char *EMO_TYPE_ENTRY = NULL;
const char *EMO_TYPE_CHECKBOX = NULL;
const char *EMO_TYPE_BOX = NULL;
const char *EMO_TYPE_APPLICATION = NULL;
const char *EMO_TYPE_SCRIPT = NULL;
const char *EMO_TYPE_STYLE = NULL;
const char *EMO_TYPE_IMAGE = NULL;
const char *EMO_TYPE_LABEL = NULL;
const char *EMO_TYPE_STACK = NULL;
const char *EMO_TYPE_SET = NULL;
const char *EMO_TYPE_SETITEM = NULL;
const char *EMO_TYPE_TEXT = NULL;
const char *EMO_TYPE_ARRAY = NULL;
const char *EMO_TYPE_FRAME = NULL;
const char *EMO_TYPE_REFERENCE = NULL;
const char *EMO_TYPE_SCROLLED = NULL;
const char *EMO_TYPE_GRADIENT = NULL;
const char *EMO_TYPE_DATA = NULL;

const char *EMO_TYPE_NAMES_END = NULL;

void emo_builtin_type_init(void)
{
	EMO_TYPE_VIEW = EMO_TYPE_NAMES;
	EMO_TYPE_ENTRY = EMO_TYPE_VIEW+5;
	EMO_TYPE_CHECKBOX = EMO_TYPE_ENTRY+6;
	EMO_TYPE_BOX = EMO_TYPE_CHECKBOX+9;
	EMO_TYPE_APPLICATION = EMO_TYPE_BOX+4;
	EMO_TYPE_SCRIPT = EMO_TYPE_APPLICATION+12;
	EMO_TYPE_STYLE = EMO_TYPE_SCRIPT+7;
	EMO_TYPE_IMAGE = EMO_TYPE_STYLE+6;
	EMO_TYPE_LABEL = EMO_TYPE_IMAGE+6;
	EMO_TYPE_STACK = EMO_TYPE_LABEL+6;
	EMO_TYPE_SET = EMO_TYPE_STACK+6;
	EMO_TYPE_SETITEM = EMO_TYPE_SET+4;
	EMO_TYPE_TEXT = EMO_TYPE_SETITEM+8;
	EMO_TYPE_ARRAY = EMO_TYPE_TEXT+5;
	EMO_TYPE_FRAME = EMO_TYPE_ARRAY+6;
	EMO_TYPE_REFERENCE = EMO_TYPE_FRAME+6;
	EMO_TYPE_SCROLLED = EMO_TYPE_REFERENCE+10;
	EMO_TYPE_GRADIENT = EMO_TYPE_SCROLLED+9;
	EMO_TYPE_DATA = EMO_TYPE_GRADIENT+9;

	EMO_TYPE_NAMES_END = EMO_TYPE_DATA+5;
}

const char *emo_builtin_type(const char *typeName)
{
	const char *builtinName = EMO_TYPE_NAMES;

	while (builtinName[0] != 0) {
		if (strcmp(typeName, builtinName) == 0)
			return builtinName;
		builtinName += strlen(builtinName)+1;
	}

	return NULL;
}