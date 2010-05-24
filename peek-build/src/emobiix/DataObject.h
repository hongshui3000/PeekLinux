#ifndef _DATAOBJECT_H_
#define _DATAOBJECT_H_

#include "DataObjectMap.h"
#include "URL.h"
#include "Map.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "DataObject_internal.h"

DataObjectField *dataobjectfield_string(const char *str);
DataObjectField *dataobjectfield_data(void *data, int bytes);
DataObjectField *dataobjectfield_int(int val);
DataObjectField *dataobjectfield_uint(unsigned int val);

typedef enum {DOS_INIT, DOS_SYNC, DOS_OK, DOS_ERROR} DataObjectState;
typedef enum {DOOP_CHANGE, DOOP_DESTROY} DataObjectOperation;

typedef void (*DataObjectSubscriptionCallback)(DataObject *dobj,
		DataObjectOperation doop, void *pararm);

DataObject *dataobject_new(void);
DataObject *dataobject_newMap(DataObject *src, DataObjectMap *dmap);
void dataobject_setValue(DataObject *dobj, const char *key, DataObjectField *v);
DataObjectField *dataobject_getValue(DataObject *obj, const char *key);
int dataobject_isLocal(DataObject *dobj);
DataObjectState dataobject_getState(DataObject *dobj);
void dataobject_setState(DataObject *dobj, DataObjectState state);
void dataobject_getStamp(DataObject *dobj, unsigned int *stampMinor,
		unsigned int *stampMajor);
void dataobject_setStamp(DataObject *dobj, unsigned int stampMinor,
		unsigned int stampMajor);
MapIterator *dataobject_fieldIterator(DataObject *dobj);
ListIterator *dataobject_childIterator(DataObject *dobj);
DataObject *dataobject_getTree(DataObject *dobj, int index);
int dataobject_getTreeNextOp(DataObject *dobj, int *ischild);
int dataobject_treeIndex(DataObject *dobj);
void dataobject_pack(DataObject *parent, DataObject *child);

DataObject *dataobject_superparent(DataObject *dobj);
DataObject *dataobject_findByName(DataObject *root, const char *name);

void dataobject_setScriptContext(DataObject *dobj, void *ctx);
void *dataobject_getScriptContext(DataObject *dobj);
void *dataobject_findScriptContext(DataObject *dobj);

DataObject *dataobject_findFieldParent(DataObject *dobj, const char *str);

void dataobject_debugPrint(DataObject *dobj);

/* Global DataObject table functions */
void dataobject_platformInit(void);
DataObject *dataobject_locateStr(const char *str);
DataObject *dataobject_locate(URL *url);
DataObject *dataobject_construct(URL *url, int isLocal);

#ifdef __cplusplus
}
#endif

#endif /* _DATAOBJECT_H_ */
