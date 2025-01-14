/*
Copyright (C) 2019  JingWeiZhangHuai
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "morn_image.h"

struct HandleImageTemplate
{
    int height;
    int width;
    char type[32];
    MTable *lx;
    MTable *ly;
    MTable *w;
    float para[8];
};
void endImageTemplate(void *info)
{
    struct HandleImageTemplate *handle = info;
    if(handle->lx != NULL) mTableRelease(handle->lx);
    if(handle->ly != NULL) mTableRelease(handle->ly);
    if(handle->w  != NULL) mTableRelease(handle->w );
}
#define HASH_ImageTemplate 0x873419e
void mImageLensTemplate(MObject *temp,float k,int r)
{
    mException(INVALID_POINTER(temp),EXIT,"invalid input");
    mException((k<-1)||(k>1)||(r<=0),EXIT,"invalid input para");
    
    int j;
    int height = r+r+1;int width = r+r+1;
    MHandle *hdl; ObjectHandle(temp,ImageTemplate,hdl);
    struct HandleImageTemplate *handle = hdl->handle;
    if((handle->height!=height)||(handle->width!=width)||(strcmp(handle->type,"lens")!=0)||(handle->para[0]!=k))
        hdl->valid = 0;
    if(hdl->valid==0)
    {
        handle->height = height;
        handle->width = width;
        strcpy(handle->type,"lens");
        handle->para[0] = k;
        
        if(handle->lx == NULL) handle->lx = mTableCreate(height,width,S16,NULL);
        else                   mTableRedefine(handle->lx,height,width,S16,NULL);
        if(handle->ly == NULL) handle->ly = mTableCreate(height,width,S16,NULL);
        else                   mTableRedefine(handle->ly,height,width,S16,NULL);
        if(handle->w  == NULL) handle->w  = mTableCreate(height,width,U8 ,NULL);
        else                   mTableRedefine(handle->w ,height,width,U8 ,NULL);
        
        hdl->valid = 1;
    }
    short **xdata = handle->lx->dataS16;
    short **ydata = handle->ly->dataS16;
    unsigned char **wdata = handle->w->dataU8;
    
    float cx = (float)r;float cy = (float)r;
    
    if(k<0)
    {
        k=0.0-k*3/(float)(r*r);
        #pragma omp parallel for
        for(j=0;j<height;j++)
            for(int i=0;i<width;i++)
            {
                float dx = i-cx;float dy = j-cy;
                float l = sqrt(dx*dx + dy*dy);
                
                if(l>=r) {xdata[j][i] = i;ydata[j][i]=j;wdata[j][i]=0xFF;continue;}
                float a = k*(l-r)*(l-r)+1.0f;
                float lx = dx*a+cx;
                float ly = dy*a+cy;
                
                xdata[j][i] = (short)lx; short x_locate = (short)lx;
                ydata[j][i] = (short)ly; short y_locate = (short)ly;

                x_locate = 15-(short)((lx-(float)x_locate)*15.0f+0.5f);
                y_locate = 15-(short)((ly-(float)y_locate)*15.0f+0.5f);
                wdata[j][i] = (x_locate<<4)+y_locate;
            }
    }
    else
    {
        k=k*3/(float)(r*r);
        #pragma omp parallel for
        for(j=0;j<height;j++)
            for(int i=0;i<width;i++)
            {
                float dx = i-cx;float dy = j-cy;
                float l = sqrt(dx*dx + dy*dy);
                
                if(l>=r) {xdata[j][i] = i;ydata[j][i]=j;wdata[j][i]=0xFF;continue;}
                float a = 1.0f/(k*(l-r)*(l-r)+1.0f);
                float lx = dx*a+cx;
                float ly = dy*a+cy;
                
                xdata[j][i] = (short)lx; short x_locate = (short)lx;
                ydata[j][i] = (short)ly; short y_locate = (short)ly;

                x_locate = 15-(short)((lx-(float)x_locate)*15+0.5f);
                y_locate = 15-(short)((ly-(float)y_locate)*15+0.5f);
                
                wdata[j][i] = (x_locate<<4)+y_locate;
            }
    }
    temp->object = handle;
}

void mImageTemplateTransform(MImage *src,MImage *dst,MObject *temp,int x,int y)
{
    int j;
    mException(INVALID_IMAGE(src),EXIT,"invalid input image");
    
    mException(INVALID_POINTER(temp),EXIT,"invalid input template");
    struct HandleImageTemplate *tmp = temp->object;
    mException(INVALID_POINTER(tmp),EXIT,"invalid input template");
    
    int height = tmp->height;
    int width = tmp->width;
    mException((height<=0)||(width<=0)||INVALID_POINTER(tmp->lx)||INVALID_POINTER(tmp->ly)||INVALID_POINTER(tmp->w),EXIT,"invalid template");
    int x1=x-width /2;int x2=x1+width;
    int y1=y-height/2;int y2=y1+height;
    short **xdata = tmp->lx->dataS16;
    short **ydata = tmp->ly->dataS16;
    unsigned char **wdata = tmp->w->dataU8;
    
    MImage *p = dst;
    if((dst==src)||(dst==NULL)) dst = mImageCreate(src->channel,src->height,src->width,NULL);
    mImageCopy(src,dst);
    
    #pragma omp parallel for
    for(j=y1;j<y2;j++)
    {
        for(int i=x1;i<x2;i++)
        {
            x = xdata[j-y1][i-x1]+x1;if((x<0)||(x>=src->width -1)) {for(int cn=0;cn<src->channel;cn++) dst->data[cn][j][i] = 0; continue;}
            y = ydata[j-y1][i-x1]+y1;if((y<0)||(y>=src->height-1)) {for(int cn=0;cn<src->channel;cn++) dst->data[cn][j][i] = 0; continue;}
            
            unsigned int wx = (wdata[j-y1][i-x1]&0xF0)>>4;
            unsigned int wy = (wdata[j-y1][i-x1]&0x0F);
            
            for(int cn=0;cn<src->channel;cn++)
            {
                register int data =((src->data[cn][y  ][x  ]*wx + src->data[cn][y  ][x+1]*(15-wx))*wy
                                   +(src->data[cn][y+1][x  ]*wx + src->data[cn][y+1][x+1]*(15-wx))*(15-wy) + 112)/225;

                dst->data[cn][j][i] = data;
            }
        }
    }
    
    memcpy(&(dst->info),&(src->info),sizeof(MInfo));
    
    if(p!=dst)
    {
        mImageExchange(src,dst);
        mImageRelease(dst);
    }
}





