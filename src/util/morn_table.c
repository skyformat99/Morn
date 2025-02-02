/*
Copyright (C) 2019  JingWeiZhangHuai
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "morn_util.h"

struct HandleTableCreate
{
    MTable *tab;
    int row;
    int col;
    int element_size;
    void **index;
    MMemory *memory;
};
void endTableCreate(void *info)
{
    struct HandleTableCreate *handle = info;
    mException((handle->tab==NULL),EXIT,"invalid table");
    
    if(handle->index != NULL) mFree(handle->index);
    if(handle->memory!= NULL) mMemoryRelease(handle->memory);
    
    mFree(handle->tab);
}
#define HASH_TableCreate 0x56f55a7f
MTable *TableCreate(int row,int col,int element_size,void **data)
{
    MTable *tab = (MTable *)mMalloc(sizeof(MTable));
    memset(tab,0,sizeof(MTable));
    
    if(col <0) col = 0;
    if(row <0) row = 0;
    
    tab->col = col;
    tab->row = row;
    
    MHandle *hdl; ObjectHandle(tab,TableCreate,hdl);
    
    struct HandleTableCreate *handle = hdl->handle;
    handle->tab = tab;
    
    if(row == 0)
    {
        mException((!INVALID_POINTER(data)),EXIT,"invalid input");
        return tab;
    }
    
    if(!INVALID_POINTER(data))
    {
        tab->data = data;
        return tab;
    }
    
    handle->index = (void **)mMalloc(row*sizeof(void *));
    handle->row = row;
    
    if(col == 0)
    {
        mException((!INVALID_POINTER(data)),EXIT,"invalid input");
        memset(handle->index,0,row*sizeof(void *));
    }
    else 
    {
        if(handle->memory == NULL) handle->memory = mMemoryCreate(row,col*element_size);
        mMemoryIndex(handle->memory,row,col*element_size,(void **)(handle->index));

        handle->col = col;
        handle->element_size = element_size;
        tab->data = handle->index;
    }
    
    return tab;
}

void mTableRelease(MTable *tab)
{
    mException(INVALID_POINTER(tab),EXIT,"invalid input");
    
    if(!INVALID_POINTER(tab->handle))
        mHandleRelease(tab->handle);
}

void TableRedefine(MTable *tab,int row,int col,int element_size,void **data)
{
    mException((INVALID_POINTER(tab)),EXIT,"invalid input");
    
    if(row<=0) row = tab->row;
    if(col<=0) col = tab->col;
    
    struct HandleTableCreate *handle = ((MHandle *)(tab->handle->data[0]))->handle;
    if((row!=tab->row)||(col!=tab->col)||(element_size!=handle->element_size)) mHandleReset(tab->handle);
    int same_size = (row<=tab->row)&&(col*element_size<=tab->col*handle->element_size);
    int reuse = (data==tab->data);
    int flag = (tab->row&&tab->col);
    tab->row = row;
    tab->col = col;
    
    if(same_size&&reuse) return;
    if(same_size&&(data==NULL)&&(handle->col>0)) return;
    
    mException(reuse&&flag&&(handle->col==0),EXIT,"invalid redefine");
    
    if(reuse) data=NULL;
    handle->col=0;
    
    if((row == 0)||(col==0)) {tab->data=NULL; return;}
    
    if(row>handle->row)
    {
        if(handle->index != NULL)mFree(handle->index);
        handle->index = NULL;
    }
    if(handle->index == NULL) 
    {
        handle->index = (void **)mMalloc(row*sizeof(void *));
        handle->row = row;
    }
    tab->data = handle->index;
    
    if(data!=NULL) {memcpy(handle->index,data,row*sizeof(void *));return;}
    
    if(handle->memory == NULL) handle->memory = mMemoryCreate(row,col*element_size);
    mMemoryIndex(handle->memory,row,col*element_size,handle->index);
    handle->col = col;
    handle->element_size = element_size;
}

void mTableCopy(MTable *src,MTable *dst)
{
    struct HandleTableCreate *handle = ((MHandle *)(src->handle->data[0]))->handle;
    int element_size = handle->element_size;
    
    TableRedefine(dst,src->row,src->col,element_size,dst->data);
    for(int j=0;j<src->row;j++)
        memcpy(dst->data[j],src->data[j],src->col*element_size);
}

struct HandleArrayCreate
{
    MArray *array;
    int num;
    int element_size;
    char *memory;
};
void endArrayCreate(void *info)
{
    struct HandleArrayCreate *handle = info;
    mException((handle->array == NULL),EXIT,"invalid array");
    
    if(handle->memory!= NULL) mFree(handle->memory);
    
    mFree(handle->array);
}
#define HASH_ArrayCreate 0xb3feafa4
MArray *ArrayCreate(int num,int element_size,void *data)
{
    MArray *array = (MArray *)mMalloc(sizeof(MArray));
    memset(array,0,sizeof(MArray));   
    
    if(num<0) num = 0;
    array->num = num;
    
    MHandle *hdl; ObjectHandle(array,ArrayCreate,hdl);
    struct HandleArrayCreate *handle = hdl->handle;
    handle->array = array;
    
    if(num==0)
    {
        mException((!INVALID_POINTER(data)),EXIT,"invalid input");
    }
    else if(INVALID_POINTER(data))
    {
        handle->memory = mMalloc(num*element_size);
        handle->num = num;
        handle->element_size = element_size;
        array->data = (void *)(handle->memory);
    }
    else
        array->data = data;
    
    return array;
}

void mArrayRelease(MArray *array)
{   
    mException(INVALID_POINTER(array),EXIT,"invalid input");
    
    if(!INVALID_POINTER(array->handle))
        mHandleRelease(array->handle);
    
    mFree(array);
}

void ArrayRedefine(MArray *array,int num,int element_size,void *data)
{
    mException(INVALID_POINTER(array),EXIT,"invalid input");
    
    if(num <= 0) num = array->num;
    
    struct HandleArrayCreate *handle = ((MHandle *)(array->handle->data[0]))->handle;
    if((num!= array->num)||(element_size!=handle->element_size)) mHandleReset(array->handle);
    int same_size = (num*element_size <= array->num*handle->element_size);
    int reuse = (data==array->data);
    int flag = (array->num>0);
    
    array->num = num;
    if(same_size&&reuse) return;
    if(same_size&&(data==NULL)&&(handle->num >0)) return;
    
    mException(reuse&&flag&&(handle->num==0),EXIT,"invalid redefine");
    
    if(reuse) data=NULL;
    handle->num=0;
    
    if(num == 0) {array->data = NULL; return;}
    
    if(data!=NULL) {handle->element_size = element_size;array->data = data;return;}
        
    if(num>handle->num)
    {
        if(handle->memory!=NULL) mFree(handle->memory);
        handle->num = num;
        handle->element_size = element_size;
        handle->memory = mMalloc(num*element_size);
        array->data = handle->memory;
    }
}

struct HandleStream
{
    char *read;
    char *write;
};
void endStream(void *info) {NULL;}
#define HASH_Stream 0xcab28d39
int mStreamRead(MArray *buff,void *data,int num)
{
    mException((buff==NULL)||(data==NULL)||(num<=0),EXIT,"invalid input");
    
    struct HandleArrayCreate *handle0 = ((MHandle *)(buff->handle->data[0]))->handle;
    num = num*handle0->element_size;
    
    MHandle *hdl;ObjectHandle(buff,Stream,hdl);
    struct HandleStream *handle = hdl->handle;
    if(hdl->valid == 0) return MORN_FAIL;
    
    int size=(handle->write>=handle->read)?(handle->write-handle->read):(buff->num-(handle->read-handle->write));
    if(num>size) return MORN_FAIL;
    
    size = (buff->dataS8+buff->num)-handle->read;
    if(size <= num)
    {
        memcpy(data,handle->read,size*sizeof(char));
        handle->read = buff->dataS8;
        num = num-size;
        if(num==0) return MORN_SUCCESS;
        data = ((char *)data)+size;
    }
    memcpy(data,handle->read,num*sizeof(char));
    handle->read = handle->read+num;
    
    return MORN_SUCCESS;
}

int mStreamWrite(MArray *buff,void *data,int num)
{
    mException((buff==NULL)||(data==NULL)||(num<=0),EXIT,"invalid input");
    
    struct HandleArrayCreate *handle0 = ((MHandle *)(buff->handle->data[0]))->handle;
    num = num*handle0->element_size;
    
    MHandle *hdl;ObjectHandle(buff,Stream,hdl);
    struct HandleStream *handle = hdl->handle;
    if(hdl->valid == 0)
    {
        handle->read = buff->dataS8;
        handle->write= buff->dataS8;
        hdl->valid = 1;
    }
    
    int size=(handle->read>handle->write)?(handle->read-handle->write):(buff->num-(handle->write-handle->read));
    if(num>size) return MORN_FAIL;
    
    size = (buff->dataS8+buff->num)-handle->write;
    if(size <= num)
    {
        memcpy(handle->write,data,size*sizeof(char));
        handle->write = buff->dataS8;
        num = num-size;
        if(num==0) return MORN_SUCCESS;
        data = ((char *)data)+size;
    }
    memcpy(handle->write,data,num*sizeof(char));
    handle->write = handle->write + num;
    
    return MORN_SUCCESS;
}
    



