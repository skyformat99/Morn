/*
Copyright (C) 2019  JingWeiZhangHuai
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _MORN_IMAGE_H_
#define _MORN_IMAGE_H_

#include "morn_math.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MORN_MAX_IMAGE_CN 4

#define MORN_IMAGE_GRAY 1
#define MORN_IMAGE_RGB  3
#define MORN_IMAGE_RGBA 4
#define MORN_IMAGE_YUV  5
#define MORN_IMAGE_HSV  6

#define MORN_BORDER_UNDEFINED DFLT
#define MORN_BORDER_BLACK     0
#define MORN_BORDER_WHITE     1
#define MORN_BORDER_REPLICATE 2
#define MORN_BORDER_REFLECT   3
#define MORN_BORDER_IMAGE     4
#define MORN_BORDER_INVALID   5

typedef struct MImageBorder
{
    short height;
    short width;
    short y1;
    short y2;
    short *x1;
    short *x2;
}MImageBorder;

typedef struct MImage {
    int channel;                         // ?????
    int height;
    int width;
    
    unsigned char **data[MORN_MAX_IMAGE_CN];
    
    MList *handle;
    
    MImageBorder *border;
    
    MInfo info;
    void *reserve;
}MImage;

#define INVALID_IMAGE(Img) ((((Img) ==NULL)||((intptr_t)(Img) == DFLT))?1:(((Img)->data == NULL)||((intptr_t)((Img)->data) == DFLT)\
                                                                    ||((Img)->channel <= 0)\
                                                                    ||((Img)->height <= 0)||((Img)->width <= 0)\
                                                                    ||((Img)->handle == NULL)))

MImage *mImageCreate(int channel,int height,int width,unsigned char **data[]);
void mImageRedefine(MImage *img,int channel,int height,int width,unsigned char **data[]);
void mImageRelease(MImage *img);

void mImageExpand(MImage *img,int r,int border_type);
void mImageCopy(MImage *src,MImage *dst);
MImage *mImageChannelSplit(MImage *src,int channel);

void mImageDiff(MImage *src1,MImage *src2,MImage *diff);
void mImageAdd(MImage *src1,MImage *src2,MImage *dst);
void mImageSub(MImage *src1,MImage *src2,MImage *dst);
void mImageInvert(MImage *src,MImage *dst);

void mImageOperate(MImage *src,MImage *dst,void (*func)(unsigned char *,unsigned char *,void *),void *para);
void mImageBinaryFilter(MImage *src,MImage *dst,int r,int thresh);


#define MORN_IMAGE_STREAM_HWC DFLT
#define MORN_IMAGE_STREAM_HCW 1
#define MORN_IMAGE_STREAM_CHW 2
#define MORN_IMAGE_STREAM_CWH 3
#define MORN_IMAGE_STREAM_WHC 4
#define MORN_IMAGE_STREAM_WCH 5

void mImageDataInputU8( MImage *img,U8  *stream,int stream_type,unsigned char (*func)(U8 ,void *),void *para);
void mImageDataInputS8( MImage *img,S8  *stream,int stream_type,unsigned char (*func)(S8 ,void *),void *para);
void mImageDataInputU16(MImage *img,U16 *stream,int stream_type,unsigned char (*func)(U16,void *),void *para);
void mImageDataInputS16(MImage *img,S16 *stream,int stream_type,unsigned char (*func)(S16,void *),void *para);
void mImageDataInputS32(MImage *img,S32 *stream,int stream_type,unsigned char (*func)(S32,void *),void *para);
void mImageDataInputF32(MImage *img,F32 *stream,int stream_type,unsigned char (*func)(F32,void *),void *para);
void mImageDataInputD64(MImage *img,D64 *stream,int stream_type,unsigned char (*func)(D64,void *),void *para);
#define mImageDataInput(Img,Stream,Stream_type,Type,Func,Para) mImageDataInput##Type(Img,Stream,Stream_type,Func,Para)

void mImageDataOutputU8( MImage *img,U8  *stream,int stream_type,U8  (*func)(unsigned char,void *),void *para);
void mImageDataOutputS8( MImage *img,S8  *stream,int stream_type,S8  (*func)(unsigned char,void *),void *para);
void mImageDataOutputU16(MImage *img,U16 *stream,int stream_type,U16 (*func)(unsigned char,void *),void *para);
void mImageDataOutputS16(MImage *img,S16 *stream,int stream_type,S16 (*func)(unsigned char,void *),void *para);
void mImageDataOutputS32(MImage *img,S32 *stream,int stream_type,S32 (*func)(unsigned char,void *),void *para);
void mImageDataOutputF32(MImage *img,F32 *stream,int stream_type,F32 (*func)(unsigned char,void *),void *para);
void mImageDataOutputD64(MImage *img,D64 *stream,int stream_type,D64 (*func)(unsigned char,void *),void *para);
#define mImageDataOutput(Img,Stream,Stream_type,Type,Func,Para) mImageDataOutput##Type(Img,Stream,Stream_type,Func,Para)

void mImageRGBToYUV(MImage *src,MImage *dst);
void mImageYUVToRGB(MImage *src,MImage *dst);
void mImageRGBToHSV(MImage *src,MImage *dst);
void mImageHSVToRGB(MImage *src,MImage *dst);
void mImageRGBToGray(MImage *src,MImage *dst);
void mImageYUVToGray(MImage *src,MImage *dst);
void mImageToGray(MImage *src,MImage *dst); 

void mImageCoordinateTransform(MImage *src,MImage *dst,float (*x_func)(int,int,void *),float (*y_func)(int,int,void *),void *para);

MImageBorder *mImageBorderCreate(int height,int width,int num,...);
void mImageBorderRelease(MImageBorder *border);
#define ImageY1(Img) (((Img)->border==NULL)?0:(((Img)->border)->y1))
#define ImageY2(Img) (((Img)->border==NULL)?((Img)->height):(((Img)->border)->y2))
#define ImageX1(Img,n) (((Img)->border==NULL)?0:(((Img)->border)->x1[n]))
#define ImageX2(Img,n) (((Img)->border==NULL)?((Img)->width):(((Img)->border)->x2[n]))

#define mImageExchange(Src,Dst) mObjectExchange(Src,Dst,MImage)
#define mImageReset(Img) mHandleReset(Img->handle)

typedef struct MImageRect
{
    int x1;
    int y1;
    int x2;
    int y2;
}MImageRect;
#define mRect(Rect,X1,Y1,X2,Y2) do{(Rect)->x1=X1;(Rect)->y1=Y1;(Rect)->x2=X2;(Rect)->y2=Y2;}while(0)
#define mRectHeight(Rect) ((Rect)->y2-(Rect)->y1)
#define mRectWidth(Rect)  ((Rect)->x2-(Rect)->x1)

typedef struct MImagePoint
{
    float x;
    float y;
}MImagePoint;
#define mPoint(Point,X,Y) do{(Point)->x=X; (Point)->y=Y;}while(0)

typedef struct MImageCircle
{
    MImagePoint center;
    float r;
}MImageCircle;
#define mCircle(Circle,Cx,Cy,R) do{(Circle)->center.x=Cx;(Circle)->center.y=Cy;(Circle)->r=R;}while(0)
    
void mBMPSave(MImage *src,const char *filename);
void mBMPLoad(const char *filename,MImage *dst);
void mJPGSave(MImage *src,const char *filename);
void mJPGLoad(const char *filename,MImage *dst);
void mPNGSave(MImage *src,const char *filename);
void mPNGLoad(const char *filename,MImage *dst);
void mImageLoad(const char *filename,MImage *img);
void mImageSave(MImage *img,const char *filename);

void mImageRotate(MImage *src,MImage *dst,MImagePoint *src_hold,MImagePoint *dst_hold,float angle);

#define MORN_RESIZE_UNUNIFORM  DFLT
#define MORN_RESIZE_MINUNIFORM 0xFE
#define MORN_RESIZE_MAXUNIFORM 0xFD
#define MORN_INTERPOLATE       DFLT
#define MORN_NEAREST           0xEF
void mImageResize(MImage *src,MImage *dst,int height,int width,int type);

extern unsigned char morn_default_color[3];
void mImageDrawPoint(MImage *src,MImage *dst,MImagePoint *point,unsigned char *color,int width);
void mImageDrawRect(MImage *src,MImage *dst,MImageRect *rect,unsigned char *color,int width);
void mImageDrawLine(MImage *src,MImage *dst,MImagePoint *p1,MImagePoint *p2,unsigned char *color,int width);
void mImageDrawShape(MImage *src,MImage *dst,MList *shape,unsigned char *color,int width);
void mImageDrawCircle(MImage *src,MImage *dst,MImageCircle *circle,unsigned char *color,int width);
void mImageDrawCurve(MImage *src,MImage *dst,float (*func)(float,void *),void *para,unsigned char *color,int width);

void LineTravel(MImagePoint *p1,MImagePoint *p2,int stride,void (*func)(MImagePoint *,void *),void *para);
void mLineTravel(MList *line,int stride,void (*func)(MImagePoint *,void *),void *para);
void mPolygonSideTravel(MList *polygon,int stride,void (*func)(MImagePoint *,void *),void *para);

#define PointLineCheck(px,py,lx1,ly1,lx2,ly2) (((lx1)-(lx2))*((py)-(ly2))-((ly1)-(ly2))*((px)-(lx2)))
int mLinePointCheck(MList *line,MImagePoint *point);
int LineCrossCheck(double l1x1,double l1y1,double l1x2,double l1y2,double l2x1,double l2y1,double l2x2,double l2y2);
int mLineCrossCheck(MList *line1,MList *line2);
int LineCrossPoint(double l1x1,double l1y1,double l1x2,double l1y2,double l2x1,double l2y1,double l2x2,double l2y2,float *px,float *py);
void mPolygon(MList *polygon,int num,...);
#define mRectArea(Rect) ((Rect->x2-Rect->x1)*(Rect->y2-Rect->y1))
float mPolygonArea(MList *polygon);
float TriangleArea(float x1,float y1,float x2,float y2,float x3,float y3);
float mTriangleArea(MImagePoint *p1,MImagePoint *p2,MImagePoint *p3);
#define mCircleArea(Circle) ((Circle)->r*(Circle)->r*MORN_PI)
float QuadrangleArea(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4);
float mQuadrangleArea(MImagePoint *p1,MImagePoint *p2,MImagePoint *p3,MImagePoint *p4);
#define PointDistance(X1,Y1,X2,Y2) ((float)sqrt(((X1)-(X2))*((X1)-(X2))+((Y1)-(Y2))*((Y1)-(Y2))))
#define mPointDistance(P1,P2) PointDistance((P1)->x,(P1)->y,(P2)->x,(P2)->y)
double PointVerticalDistance(double px,double py,double lx1,double ly1,double lx2,double ly2,float *vx,float *vy);
float mPointVerticalDistance(MImagePoint *point,MList *line,MImagePoint *pedal);
#define mPointInRect(Point,Rect) (((Point)->x>(Rect)->x1)&&((Point)->x<(Rect)->x2)&&((Point)->y>(Rect)->y1)&&((Point)->y<(Rect)->y2));
int PointInPolygon(double x,double y,MList *polygon);
int mPointInPolygon(MImagePoint *point,MList *polygon);
float mRectUnionsetArea(MImageRect *rect1,MImageRect *rect2);
float mRectIntersetArea(MImageRect *rect1,MImageRect *rect2);
float mCircleIntersetArea(MImageCircle *circle1,MImageCircle *circle2);
float mPolygonIntersetArea(MList *polygon1,MList *polygon2);
int PolygonCross(MList *polygon1,MList *polygon2);
int mPolygonCross(MList *polygon1,MList *polygon2);
int mPolygonConcaveCheck(MList *polygon);
void mShapeBounding(MList *shape,MList *bounding);

void mImageBinaryEdge(MImage *src,MSheet *edge,MList *rect);
void mEdgeBoundary(MList *edge,MList *polygon,float thresh);

void mVideoBackground(MImage *src,MImage *bgd,int time_thresh,int diff_thresh);

void mImageMeanFilter(MImage *src,MImage *dst,int r);
void mImageMidValueFilter(MImage *src,MImage *dst);

void mImageDirection(MImage *src,MImage *dst,int r,int thresh);
void mImageDirectionGradient(MImage *src,MImage *dst,int direction,int r,int thresh);
void mImageGradientValue(MImage *src,MImage *dst,int r,int thresh);
void mImageGradientCheck(MImage *src,MImage *dst,int r,int thresh);
void mImageGradientAdaptCheck(MImage *src,MImage *dst,int r,float thresh);
void mImageGradient(MImage *src,MImage *direction,MImage *value,int r,int thresh);
void mImageGradientFilter(MImage *dir,MImage *value,MImage *ddst,MImage *vdst,int r);
void mImageGradientSuppression(MImage *dir,MImage *value,MImage *dst,int r);
void mImageCanny(MImage *src,MImage *dst,int r,int thresh);

void mImageLensTemplate(MObject *temp,float k,int r);
void mImageTemplateTransform(MImage *src,MImage *dst,MObject *temp,int x,int y);

int mColorCluster(MImage *src,MImage *dst,MList *list,int r,int thresh);


#ifdef __cplusplus
}
#endif

#endif
