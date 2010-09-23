#include "RootApplication.h"

#include "DataObject.h"
#include "Widget.h"
#include "File.h"
#include "Mime.h"
#include "Color.h"
#include "Gradient.h"
#include "Debug.h"

#include "p_malloc.h"
#include "system_battery.h"

#include <stdio.h>

#ifdef SIMULATOR
#define BARHEIGHT "16"
#define CONTENTHEIGHT "224"
#else
#define BARHEIGHT "25"
#define CONTENTHEIGHT "215"
#endif

DataObject *dobjFromFile(const char *filename, DataObject *root)
{
	DataObject *dobj1;
	File *mimefile;
	int mimedata_size;
	void *mimedata;

	EMO_ASSERT_NULL(filename != NULL, "load obj from file missing filename")
	EMO_ASSERT_NULL(root != NULL, "load obj from file missing root")

	mimefile = file_openRead(filename);
	dobj1 = widget_newTypeIdName("image", NULL, NULL, root);
	if (mimefile != NULL) {
		mimedata_size = file_size(mimefile);
		mimedata = (char *)p_malloc(mimedata_size);
		file_read(mimefile, mimedata_size, mimedata);
		file_close(mimefile);
		dataobject_setValue(dobj1, "mime-type", dataobjectfield_string("png"));
		dataobject_setValue(dobj1, "data", dataobjectfield_data(mimedata, mimedata_size));
		mime_load(dobj1);
	}

	return dobj1;
}

DataObject *RootApplication(void)
{
    static DataObject *output = NULL;
	DataObject *dobj1, *root, *dobj2, *frame;
	DataObject *setw, *setiw, *setc, *settd;
	DataObject *signalstack, *testgprs;
	DataObject *batterystack, *testbattery, *testcharge;
	DataObject *testweather, *testspkr;

    if (output != NULL)
        return output;

    output = dataobject_new();
    dataobject_setValue(output, "type", dataobjectfield_string("application"));
    dataobject_setValue(output, "name", dataobjectfield_string("Boot Manager"));
    dataobject_setValue(output, "description", dataobjectfield_string( "Construct program for loading other programs"));
    dataobject_setValue(output, "icon", dataobjectfield_string("appicon"));
    dataobject_setValue(output, "startupview", dataobjectfield_string("rootview"));

	root = widget_newTypeIdName("view", NULL, "rootview", output);
	widget_setPacking(root, WP_VERTICAL);

	dobj1 = widget_newTypeIdName("box", "darkgradbox", NULL, root);
	dataobject_setValue(dobj1, "width", dataobjectfield_string("320"));
	dataobject_setValue(dobj1, "height", dataobjectfield_string(BARHEIGHT));
	widget_setPacking(dobj1, WP_HORIZONTAL);

//    settd = widget_newTypeIdName("label", "labelw", NULL, dobj1);
//    dataobject_setValue(settd, "data", dataobjectfield_string(hw_td_get_clock_str()));
//    dataobject_setValue(settd, "weight", dataobjectfield_string("bold"));
//    dataobject_setValue(settd, "width", dataobjectfield_string("100"));
//    dataobject_setValue(settd, "height", dataobjectfield_string(BARHEIGHT));
//    dataobject_setValue(settd, "marginleft", dataobjectfield_string("20"));
//    dataobject_setValue(settd, "margintop", dataobjectfield_string("4"));

	signalstack = widget_newTypeIdName("stack", NULL, NULL, dobj1);
#if 0
	/* GPRS set */
	setw = widget_newTypeIdName("set", NULL, NULL, signalstack);
	dataobject_setValue(setw, "fieldname", dataobjectfield_string("GPRS"));
	dataobject_setValue(setw, "margintop", dataobjectfield_string("3"));
	dataobject_setValue(setw, "marginleft", dataobjectfield_string("3"));
	widget_setDataObject(setw, testgprs);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_string("1"));
	setc = dobjFromFile("gprs.png", setiw);
	dataobject_setValue(setc, "transparency", dataobjectfield_string("stencil"));
	dataobject_setValue(setc, "color", dataobjectfield_string("00000000"));

	setiw = widget_newTypeIdName("box", NULL, NULL, setw);
#endif 

	/* Signal set */
	setw = widget_newTypeIdName("set", NULL, NULL, signalstack);
	dataobject_setValue(setw, "fieldname", dataobjectfield_string("signal-level"));
	dataobject_setValue(setw, "reference", dataobjectfield_string("system://local/gprs"));

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(0));
	setc = dobjFromFile("signal0.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(1));
	setc = dobjFromFile("signal1.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(2));
	setc = dobjFromFile("signal2.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(3));
	setc = dobjFromFile("signal3.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(4));
	setc = dobjFromFile("signal4.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(5));
	setc = dobjFromFile("signal5.png", setiw);

#if 0
	/* Weather set */
	setw = widget_newTypeIdName("set", NULL, NULL, dobj1);
	dataobject_setValue(setw, "fieldname", dataobjectfield_string("data"));
	/*dataobject_setValue(setw, "margintop", dataobjectfield_string("3"));
	dataobject_setValue(setw, "marginleft", dataobjectfield_string("3"));*/
	widget_setDataObject(setw, testweather);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_string("1"));
	setc = dobjFromFile("wi-cloud.png", setiw);
	dataobject_setValue(setc, "color", dataobjectfield_string("00000000"));
	dataobject_setValue(setc, "margintop", dataobjectfield_string("3"));

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_string("2"));
	setc = dobjFromFile("wi-rain.png", setiw);
	dataobject_setValue(setc, "color", dataobjectfield_string("00000000"));

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_string("3"));
	setc = dobjFromFile("wi-storm.png", setiw);
	dataobject_setValue(setc, "color", dataobjectfield_string("00000000"));

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_string("4"));
	setc = dobjFromFile("wi-snow.png", setiw);
	dataobject_setValue(setc, "color", dataobjectfield_string("00000000"));

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	/*dataobject_setValue(setiw, "fieldvalue", dataobjectfield_string("5"));*/
	setc = dobjFromFile("wi-sun.png", setiw);
	dataobject_setValue(setc, "color", dataobjectfield_string("00000000"));

	setw = widget_newTypeIdName("label", NULL, NULL, dobj1);
	dataobject_setValue(setw, "data", dataobjectfield_string("73F"));
	dataobject_setValue(setw, "margintop", dataobjectfield_string("1"));

	/* Spkr set */
	setw = widget_newTypeIdName("set", NULL, NULL, dobj1);
	dataobject_setValue(setw, "fieldname", dataobjectfield_string("data"));
	dataobject_setValue(setw, "margin", dataobjectfield_string("2"));
	/*dataobject_setValue(setw, "marginleft", dataobjectfield_string("3"));*/
	widget_setDataObject(setw, testspkr);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_string("1"));
	setc = dobjFromFile("spkr-on.png", setiw);
	dataobject_setValue(setc, "color", dataobjectfield_string("00000000"));
	
	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	setc = dobjFromFile("spkr-off.png", setiw);
	dataobject_setValue(setc, "color", dataobjectfield_string("00000000"));
#endif

	/* battery indicator */
	batterystack = widget_newTypeIdName("stack", NULL, NULL, dobj1);

	setw = widget_newTypeIdName("set", NULL, NULL, batterystack);
	dataobject_setValue(setw, "fieldname", dataobjectfield_string("charge-level"));
	dataobject_setValue(setw, "reference", dataobjectfield_string("system://local/battery"));

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(0));
	setc = dobjFromFile("battery0.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(1));
	setc = dobjFromFile("battery1.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(2));
	setc = dobjFromFile("battery2.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(3));
	setc = dobjFromFile("battery3.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(4));
	setc = dobjFromFile("battery4.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(5));
	setc = dobjFromFile("battery5.png", setiw);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_uint(6));
	setc = dobjFromFile("battery6.png", setiw);

	setiw = widget_newTypeIdName("box", NULL, NULL, setw);

	setw = widget_newTypeIdName("set", NULL, NULL, batterystack);
	dataobject_setValue(setw, "fieldname", dataobjectfield_string("charging"));
	dataobject_setValue(setw, "margintop", dataobjectfield_string("3"));
	widget_setDataObject(setw, testcharge);

	setiw = widget_newTypeIdName("setitem", NULL, NULL, setw);
	dataobject_setValue(setiw, "fieldvalue", dataobjectfield_string("1"));
	setc = dobjFromFile("batterycharge.png", setiw);

	setiw = widget_newTypeIdName("box", NULL, NULL, setw);

	/* application box */
	dobj1 = widget_newTypeIdName("box", NULL, NULL, root);
	dataobject_setValue(dobj1, "width", dataobjectfield_string("320"));
	dataobject_setValue(dobj1, "height", dataobjectfield_string(CONTENTHEIGHT));
	widget_setPacking(dobj1, WP_VERTICAL);

	dobj2 = widget_newTypeIdName("stack", NULL, NULL, dobj1);
	dataobject_setValue(dobj2, "width", dataobjectfield_string("320"));
	dataobject_setValue(dobj2, "height", dataobjectfield_string(CONTENTHEIGHT));
	widget_setPacking(dobj2, WP_VERTICAL);

	frame = widget_newTypeIdName("frame", NULL, "placeholder", dobj2);
	dataobject_setValue(frame, "width", dataobjectfield_string("320"));
	dataobject_setValue(frame, "height", dataobjectfield_string(CONTENTHEIGHT));
	widget_setPacking(frame, WP_VERTICAL);

	frame = widget_newTypeIdName("box", NULL, "menuholder", dobj2);
	dataobject_setValue(frame, "width", dataobjectfield_string("320"));
	dataobject_setValue(frame, "height", dataobjectfield_string(CONTENTHEIGHT));
	widget_setPacking(frame, WP_VERTICAL);

	frame = widget_newTypeIdName("reference", NULL, "dialogholder", dobj2);
	dataobject_setValue(frame, "width", dataobjectfield_string("320"));
	dataobject_setValue(frame, "height", dataobjectfield_string(CONTENTHEIGHT));
	widget_setPacking(frame, WP_VERTICAL);

    return output;
}
