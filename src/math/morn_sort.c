/*
Copyright (C) 2019  JingWeiZhangHuai
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "morn_math.h"

#define AscSort(Type,Data_in,Data_out,Num) {\
    int I,J;\
    Type Thresh;\
    \
    mException((INVALID_POINTER(Data_in)),EXIT,"invalid input");\
    \
    if((!INVALID_POINTER(Data_out))&&(Data_out!=Data_in))\
    {\
        memcpy(Data_out,Data_in,Num*sizeof(Type));\
        Data_in = Data_out;\
    }\
    \
    if(Num <= 1)\
        return;\
    \
    Thresh = Data_in[0];\
    I=0;J=Num-1;\
    while(I<J)\
    {\
        while(Data_in[J]>=Thresh)\
        {\
            if(J==I)\
                goto AscSort_next;\
            J=J-1;\
        }\
        \
        Data_in[I] = Data_in[J];\
        \
        while(Data_in[I]<=Thresh)\
        {\
            if(I==J)\
                goto AscSort_next;\
            I=I+1;\
        }\
        \
        Data_in[J] = Data_in[I];\
    }\
\
AscSort_next:\
    Data_in[I] = Thresh;\
    mAscSort##Type(Data_in,NULL,NULL,NULL,I);\
    mAscSort##Type(Data_in+I+1,NULL,NULL,NULL,Num-I-1);\
}

#define AscSortIndex(Type,Data_in,Index_in,Data_out,Index_out,Num) {\
    int I,J;\
    Type Thresh;\
    int Thresh_index;\
    \
    mException((INVALID_POINTER(Data_in)),EXIT,"invalid input");\
    \
    if(INVALID_POINTER(Index_in)&&INVALID_POINTER(Index_out))\
    {\
        AscSort(Type,Data_in,Data_out,Num);\
        return;\
    }\
    \
    if((!INVALID_POINTER(Data_out))&&(Data_out!=Data_in))\
    {\
        memcpy(Data_out,Data_in,Num*sizeof(Type));\
        Data_in = Data_out;\
    }\
    if(INVALID_POINTER(Index_in))\
    {\
        for(I=0;I<Num;I++)\
            Index_out[I] = I;\
        Index_in = Index_out;\
    }\
    else if((!INVALID_POINTER(Index_out))&&(Index_in != Index_out))\
    {\
        memcpy(Index_out,Index_in,Num*sizeof(int));\
        Index_in = Index_out;\
    }\
    \
    if(Num <= 1)\
        return;\
    \
    Thresh = Data_in[0];\
    Thresh_index = Index_in[0];\
    I=0;J=Num-1;\
    while(I<J)\
    {\
        while(Data_in[J]>=Thresh)\
        {\
            if(J==I)\
                goto AscSortIndex_next;\
            J=J-1;\
        }\
        \
        Data_in[I] = Data_in[J];\
        Index_in[I] = Index_in[J];\
        \
        while(Data_in[I]<=Thresh)\
        {\
            if(I==J)\
                goto AscSortIndex_next;\
            I=I+1;\
        }\
        \
        Data_in[J] = Data_in[I];\
        Index_in[J] = Index_in[I];\
    }\
\
AscSortIndex_next:\
    Data_in[I] = Thresh;\
    Index_in[I] = Thresh_index;\
    mAscSort##Type(Data_in,Index_in,NULL,NULL,I);\
    mAscSort##Type(Data_in+I+1,Index_in+I+1,NULL,NULL,Num-I-1);\
}

void mAscSortD64(        double *data_in,int *index_in,        double *data_out,int *index_out,int num) {AscSortIndex(D64,data_in,index_in,data_out,index_out,num);}
void mAscSortF32(         float *data_in,int *index_in,         float *data_out,int *index_out,int num) {AscSortIndex(F32,data_in,index_in,data_out,index_out,num);}
void mAscSortS32(           int *data_in,int *index_in,           int *data_out,int *index_out,int num) {AscSortIndex(S32,data_in,index_in,data_out,index_out,num);}
void mAscSortS16(         short *data_in,int *index_in,         short *data_out,int *index_out,int num) {AscSortIndex(S16,data_in,index_in,data_out,index_out,num);}
void mAscSortS8(           char *data_in,int *index_in,          char *data_out,int *index_out,int num) {AscSortIndex( S8,data_in,index_in,data_out,index_out,num);}
void mAscSortU32(  unsigned int *data_in,int *index_in,  unsigned int *data_out,int *index_out,int num) {AscSortIndex(U32,data_in,index_in,data_out,index_out,num);}
void mAscSortU16(unsigned short *data_in,int *index_in,unsigned short *data_out,int *index_out,int num) {AscSortIndex(U16,data_in,index_in,data_out,index_out,num);}
void mAscSortU8(  unsigned char *data_in,int *index_in, unsigned char *data_out,int *index_out,int num) {AscSortIndex( U8,data_in,index_in,data_out,index_out,num);}

struct HandleSequenceBorder {
    union
    {
        int data_S32[2];
        unsigned int data_U32[2];
        short data_S16[2];
        unsigned short data_U16[2];
        char data_S8[2];
        unsigned char data_U8[2];
        float data_F32[2];
        double data_D64[2];
    };
    int index[2];
};
#define HASH_SequenceBorderD64 0xf8da1258
#define HASH_SequenceBorderF32 0x598eb2d1
#define HASH_SequenceBorderS32 0x20ac6748
#define HASH_SequenceBorderU32 0x8193bb8e
#define HASH_SequenceBorderS16 0x94a88efe
#define HASH_SequenceBorderU16 0x0d9793d8
#define HASH_SequenceBorderS8  0x8bee3165
#define HASH_SequenceBorderU8  0x17ea79d3
#define HandleSequenceBorderD64 HandleSequenceBorder
#define HandleSequenceBorderF32 HandleSequenceBorder
#define HandleSequenceBorderS32 HandleSequenceBorder
#define HandleSequenceBorderU32 HandleSequenceBorder
#define HandleSequenceBorderS16 HandleSequenceBorder
#define HandleSequenceBorderU16 HandleSequenceBorder
#define HandleSequenceBorderS8  HandleSequenceBorder
#define HandleSequenceBorderU8  HandleSequenceBorder
void endSequenceBorderD64(void *handle) {NULL;}
void endSequenceBorderF32(void *handle) {NULL;}
void endSequenceBorderS32(void *handle) {NULL;}
void endSequenceBorderU32(void *handle) {NULL;}
void endSequenceBorderS16(void *handle) {NULL;}
void endSequenceBorderU16(void *handle) {NULL;}
void endSequenceBorderS8( void *handle) {NULL;}
void endSequenceBorderU8( void *handle) {NULL;}
#define SequenceBorder(Type,Proc,Data_in,Index_in,Min,Min_index,Max,Max_index) {\
    MHandle *hdl; ObjectHandle(Proc,SequenceBorder##Type,hdl);\
    struct HandleSequenceBorder *handle = hdl->handle;\
    if(hdl->valid == 0)\
    {\
        handle->data_##Type[0] = Data_in;\
        handle->data_##Type[1] = Data_in;\
        handle->index[0] = Index_in;\
        handle->index[1] = Index_in;\
        hdl->valid = 1;\
    }\
    else if(Data_in<handle->data_##Type[0])\
    {\
        handle->data_##Type[0] = Data_in;\
        handle->index[0] = Index_in;\
    }\
    else if(Data_in>handle->data_##Type[1])\
    {\
        handle->data_##Type[1] = Data_in;\
        handle->index[1] = Index_in;\
    }\
    \
    if(Min!=NULL)          *Min = handle->data_##Type[0];\
    if(Min_index!=NULL)    *Min_index = handle->index[0];\
    if(Max!=NULL)          *Max = handle->data_##Type[1];\
    if(Max_index!=NULL)    *Max_index = handle->index[1];\
}
void mSequenceBorderD64(MObject *proc,        double data_in,int index_in,        double *min,int *min_index,        double *max,int *max_index) {SequenceBorder(D64,proc,data_in,index_in,min,min_index,max,max_index);}
void mSequenceBorderF32(MObject *proc,         float data_in,int index_in,         float *min,int *min_index,         float *max,int *max_index) {SequenceBorder(F32,proc,data_in,index_in,min,min_index,max,max_index);}
void mSequenceBorderS32(MObject *proc,           int data_in,int index_in,           int *min,int *min_index,           int *max,int *max_index) {SequenceBorder(S32,proc,data_in,index_in,min,min_index,max,max_index);}
void mSequenceBorderU32(MObject *proc,  unsigned int data_in,int index_in,  unsigned int *min,int *min_index,  unsigned int *max,int *max_index) {SequenceBorder(U32,proc,data_in,index_in,min,min_index,max,max_index);}
void mSequenceBorderS16(MObject *proc,         short data_in,int index_in,         short *min,int *min_index,         short *max,int *max_index) {SequenceBorder(S16,proc,data_in,index_in,min,min_index,max,max_index);}
void mSequenceBorderU16(MObject *proc,unsigned short data_in,int index_in,unsigned short *min,int *min_index,unsigned short *max,int *max_index) {SequenceBorder(U16,proc,data_in,index_in,min,min_index,max,max_index);}
void mSequenceBorderS8( MObject *proc,          char data_in,int index_in,          char *min,int *min_index,          char *max,int *max_index) {SequenceBorder(S8, proc,data_in,index_in,min,min_index,max,max_index);}
void mSequenceBorderU8( MObject *proc, unsigned char data_in,int index_in, unsigned char *min,int *min_index, unsigned char *max,int *max_index) {SequenceBorder(U8, proc,data_in,index_in,min,min_index,max,max_index);}

struct SortTree {
    union
    {
        double data_D64;
        float data_F32;
        S32 data_S32;
        U32 data_U32;
        S16 data_S16;
        U16 data_U16;
        S8  data_S8;
        U8  data_U8;
    };
    int index;
    
    struct SortTree *parent;
    struct SortTree *left;
    struct SortTree *right;
};

#define SortTreeResult(Type,Tree,Data_out,Index_out,Order) {\
    int n;\
    \
    if(Tree->left != NULL)\
        SortTreeResult##Type(Tree->left,Data_out,Index_out,Order);\
    \
    n = *Order;\
    if(Data_out != NULL)\
        Data_out[n] = Tree->data_##Type;\
    if(Index_out != NULL)\
        Index_out[n] = Tree->index;\
    *Order = n+1;\
    \
    if(Tree->right != NULL)\
        SortTreeResult##Type(Tree->right,Data_out,Index_out,Order);\
}
void SortTreeResultD64(struct SortTree *tree,        double *data_out,int *index_out,int *order) {SortTreeResult(D64,tree,data_out,index_out,order);}
void SortTreeResultF32(struct SortTree *tree,         float *data_out,int *index_out,int *order) {SortTreeResult(F32,tree,data_out,index_out,order);}
void SortTreeResultS32(struct SortTree *tree,           int *data_out,int *index_out,int *order) {SortTreeResult(S32,tree,data_out,index_out,order);}
void SortTreeResultU32(struct SortTree *tree,  unsigned int *data_out,int *index_out,int *order) {SortTreeResult(U32,tree,data_out,index_out,order);}
void SortTreeResultS16(struct SortTree *tree,         short *data_out,int *index_out,int *order) {SortTreeResult(S16,tree,data_out,index_out,order);}
void SortTreeResultU16(struct SortTree *tree,unsigned short *data_out,int *index_out,int *order) {SortTreeResult(U16,tree,data_out,index_out,order);}
void SortTreeResultS8( struct SortTree *tree,          char *data_out,int *index_out,int *order) {SortTreeResult(S8, tree,data_out,index_out,order);}
void SortTreeResultU8( struct SortTree *tree, unsigned char *data_out,int *index_out,int *order) {SortTreeResult(U8, tree,data_out,index_out,order);}

void SortTreeRelease(struct SortTree *tree)
{
    if(tree->left != NULL)
        SortTreeRelease(tree->left);
    if(tree->right != NULL)
        SortTreeRelease(tree->right);
    
    mFree(tree);
}
     
struct HandleSequenceSort {
    struct SortTree *tree;
    struct SortTree *thresh;
    int num;
};

void endSequenceSort(void *info)
{
    struct HandleSequenceSort *handle = info;
    if(handle->tree != NULL)
        SortTreeRelease(handle->tree);
}
#define endSequenceSortD64 endSequenceSort
#define endSequenceSortF32 endSequenceSort
#define endSequenceSortS32 endSequenceSort
#define endSequenceSortU32 endSequenceSort
#define endSequenceSortS16 endSequenceSort
#define endSequenceSortU16 endSequenceSort
#define endSequenceSortS8  endSequenceSort
#define endSequenceSortU8  endSequenceSort

#define HASH_SequenceSortD64 0xeb3a3c23
#define HASH_SequenceSortF32 0xfa75b7a2
#define HASH_SequenceSortS32 0xb358b1ab
#define HASH_SequenceSortU32 0x52715d65
#define HASH_SequenceSortS16 0x3f5c89f5
#define HASH_SequenceSortU16 0xc66d851b
#define HASH_SequenceSortS8  0x3f741d0a
#define HASH_SequenceSortU8  0xc377ea6c
#define HandleSequenceSortD64 HandleSequenceSort
#define HandleSequenceSortF32 HandleSequenceSort
#define HandleSequenceSortS32 HandleSequenceSort
#define HandleSequenceSortU32 HandleSequenceSort
#define HandleSequenceSortS16 HandleSequenceSort
#define HandleSequenceSortU16 HandleSequenceSort
#define HandleSequenceSortS8  HandleSequenceSort
#define HandleSequenceSortU8  HandleSequenceSort

#define mSequenceAscSort(Type,Proc,Data_in,Index_in,Data_out,Index_out,Num) {\
    struct SortTree *tree;\
    struct SortTree *node;\
    \
    int order;\
    \
    MHandle *hdl; ObjectHandle(Proc,SequenceSort##Type,hdl);\
    struct HandleSequenceSort *handle = hdl->handle;\
    hdl->valid = 1;\
    \
    node = (struct SortTree *)mMalloc(sizeof(struct SortTree));\
    node->data_##Type = Data_in;\
    node->index = Index_in;\
    node->left = NULL;\
    node->right = NULL;\
    \
    if(handle->tree == NULL)\
        handle->tree = node;\
    else\
    {\
        tree = handle->tree;\
        \
        while(1)\
        {\
            if(Data_in <= tree->data_##Type)\
            {\
                if(tree->left == NULL)\
                {\
                    tree->left = node;\
                    break;\
                }\
                else\
                    tree = tree->left;\
            }\
            else\
            {\
                if(tree->right == NULL)\
                {\
                    tree->right = node;\
                    break;\
                }\
                else\
                    tree = tree->right;\
            }\
        }\
    }\
    handle->num = handle->num +1;\
    if(Num != NULL)\
        *Num = handle->num;\
    \
    if((Data_out != NULL)||(Index_out!=NULL))\
    {\
        order = 0;\
        SortTreeResult##Type(handle->tree,Data_out,Index_out,&order);\
    }\
}

#define mSequenceDescSort(Type,Proc,Data_in,Index_in,Data_out,Index_out,Num) {\
    struct SortTree *tree;\
    struct SortTree *node;\
    \
    int order;\
    \
    MHandle *hdl; ObjectHandle(Proc,SequenceSort##Type,hdl);\
    struct HandleSequenceSort *handle = hdl->handle;\
    hdl->valid = 1;\
    \
    node = (struct SortTree *)mMalloc(sizeof(struct SortTree));\
    node->data_##Type = Data_in;\
    node->index = Index_in;\
    node->left = NULL;\
    node->right = NULL;\
    \
    if(handle->tree == NULL)\
        handle->tree = node;\
    else\
    {\
        tree = handle->tree;\
        \
        while(1)\
        {\
            if(Data_in >= tree->data_##Type)\
            {\
                if(tree->left == NULL)\
                {\
                    tree->left = node;\
                    break;\
                }\
                else\
                    tree = tree->left;\
            }\
            else\
            {\
                if(tree->right == NULL)\
                {\
                    tree->right = node;\
                    break;\
                }\
                else\
                    tree = tree->right;\
            }\
        }\
    }\
    handle->num = handle->num +1;\
    if(Num != NULL)\
        *Num = handle->num;\
    \
    if((Data_out != NULL)||(Index_out!=NULL))\
    {\
        order = 0;\
        SortTreeResult##Type(handle->tree,Data_out,Index_out,&order);\
    }\
}

void mSequenceAscSortD64(MObject *proc,        double data_in,int index_in,        double *data_out,int *index_out,int *num) {mSequenceAscSort(D64,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceAscSortF32(MObject *proc,         float data_in,int index_in,         float *data_out,int *index_out,int *num) {mSequenceAscSort(F32,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceAscSortS32(MObject *proc,           int data_in,int index_in,           int *data_out,int *index_out,int *num) {mSequenceAscSort(S32,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceAscSortU32(MObject *proc,  unsigned int data_in,int index_in,  unsigned int *data_out,int *index_out,int *num) {mSequenceAscSort(U32,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceAscSortS16(MObject *proc,         short data_in,int index_in,         short *data_out,int *index_out,int *num) {mSequenceAscSort(S16,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceAscSortU16(MObject *proc,unsigned short data_in,int index_in,unsigned short *data_out,int *index_out,int *num) {mSequenceAscSort(U16,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceAscSortS8( MObject *proc,          char data_in,int index_in,          char *data_out,int *index_out,int *num) {mSequenceAscSort(S8 ,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceAscSortU8( MObject *proc, unsigned char data_in,int index_in, unsigned char *data_out,int *index_out,int *num) {mSequenceAscSort(U8 ,proc,data_in,index_in,data_out,index_out,num);}

void mSequenceDescSortD64(MObject *proc,        double data_in,int index_in,        double *data_out,int *index_out,int *num) {mSequenceDescSort(D64,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceDescSortF32(MObject *proc,         float data_in,int index_in,         float *data_out,int *index_out,int *num) {mSequenceDescSort(F32,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceDescSortS32(MObject *proc,           int data_in,int index_in,           int *data_out,int *index_out,int *num) {mSequenceDescSort(S32,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceDescSortU32(MObject *proc,  unsigned int data_in,int index_in,  unsigned int *data_out,int *index_out,int *num) {mSequenceDescSort(U32,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceDescSortS16(MObject *proc,         short data_in,int index_in,         short *data_out,int *index_out,int *num) {mSequenceDescSort(S16,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceDescSortU16(MObject *proc,unsigned short data_in,int index_in,unsigned short *data_out,int *index_out,int *num) {mSequenceDescSort(U16,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceDescSortS8( MObject *proc,          char data_in,int index_in,          char *data_out,int *index_out,int *num) {mSequenceDescSort(S8 ,proc,data_in,index_in,data_out,index_out,num);}
void mSequenceDescSortU8( MObject *proc, unsigned char data_in,int index_in, unsigned char *data_out,int *index_out,int *num) {mSequenceDescSort(U8 ,proc,data_in,index_in,data_out,index_out,num);}

#define endSequenceMinSubsetD64 endSequenceSort
#define endSequenceMinSubsetF32 endSequenceSort
#define endSequenceMinSubsetS32 endSequenceSort
#define endSequenceMinSubsetU32 endSequenceSort
#define endSequenceMinSubsetS16 endSequenceSort
#define endSequenceMinSubsetU16 endSequenceSort
#define endSequenceMinSubsetS8  endSequenceSort
#define endSequenceMinSubsetU8  endSequenceSort
#define HASH_SequenceMinSubsetD64 0x97ee81e0
#define HASH_SequenceMinSubsetF32 0x98df1e59
#define HASH_SequenceMinSubsetS32 0x3ffca730
#define HASH_SequenceMinSubsetU32 0x40e37896
#define HASH_SequenceMinSubsetS16 0xd3f8fa86
#define HASH_SequenceMinSubsetU16 0xace72540
#define HASH_SequenceMinSubsetS8  0x6252d47d
#define HASH_SequenceMinSubsetU8  0xee4f1ceb
#define HandleSequenceMinSubsetD64 HandleSequenceSort
#define HandleSequenceMinSubsetF32 HandleSequenceSort
#define HandleSequenceMinSubsetS32 HandleSequenceSort
#define HandleSequenceMinSubsetU32 HandleSequenceSort
#define HandleSequenceMinSubsetS16 HandleSequenceSort
#define HandleSequenceMinSubsetU16 HandleSequenceSort
#define HandleSequenceMinSubsetS8  HandleSequenceSort
#define HandleSequenceMinSubsetU8  HandleSequenceSort

#define SequenceMinSubset(Type,Proc,Data_in,Index_in,Num,Data_out,Index_out) {\
    struct SortTree *tree;\
    struct SortTree *node;\
    Type Limit;\
    \
    int order;\
    if(Num <= 1)\
    {\
        mSequenceBorder##Type(Proc,Data_in,Index_in,Data_out,Index_out,NULL,NULL);\
        Limit = *Data_out;\
        return Limit;\
    }\
    \
    MHandle *hdl; ObjectHandle(Proc,SequenceMinSubset##Type,hdl);\
    struct HandleSequenceSort *handle = hdl->handle;\
    hdl->valid = 1;\
    \
    if(handle->num<Num)\
    {\
        node = (struct SortTree *)mMalloc(sizeof(struct SortTree));\
    }\
    else if(Data_in >= handle->thresh->data_##Type)\
    {\
        if((Data_out != NULL)||(Index_out!=NULL))\
        {\
            order = 0;\
            SortTreeResult##Type(handle->tree,Data_out,Index_out,&order);\
        }\
        Limit = handle->thresh->data_##Type;\
        return Limit;\
    }\
    else\
    {\
        node = handle->thresh;\
        if(node->parent != NULL)\
        {\
            node->parent->right = node->left;\
            if(node->left != NULL)\
                node->left->parent = node->parent;\
            handle->thresh = node->parent;\
        }\
        else\
        {\
            handle->tree = node->left;\
            node->left->parent = NULL;\
            handle->thresh = node->left;\
        }\
    }\
    \
    node->data_##Type = Data_in;\
    node->index = Index_in;\
    node->left = NULL;\
    node->right = NULL;\
    \
    if(handle->tree == NULL)\
    {\
        node->parent = NULL;\
        handle->tree = node;\
        handle->thresh = node;\
    }\
    else\
    {\
        tree = handle->tree;\
        \
        while(1)\
        {\
            if(Data_in <= tree->data_##Type)\
            {\
                if(tree->left == NULL)\
                {\
                    node->parent = tree;\
                    tree->left = node;\
                    break;\
                }\
                else\
                    tree = tree->left;\
            }\
            else\
            {\
                if(tree->right == NULL)\
                {\
                    node->parent = tree;\
                    tree->right = node;\
                    break;\
                }\
                else\
                    tree = tree->right;\
            }\
        }\
    }\
    handle->num = handle->num +1;\
    \
    while(handle->thresh->right != NULL)\
        handle->thresh = handle->thresh->right;\
    Limit = handle->thresh->data_##Type;\
    \
    if((Data_out != NULL)||(Index_out!=NULL))\
    {\
        order = 0;\
        SortTreeResult##Type(handle->tree,Data_out,Index_out,&order);\
    }\
    return Limit;\
}

        double mSequenceMinSubsetD64(MObject *proc,        double data_in,int index_in,        double *data_out,int *index_out,int num) {SequenceMinSubset(D64,proc,data_in,index_in,num,data_out,index_out);}
         float mSequenceMinSubsetF32(MObject *proc,         float data_in,int index_in,         float *data_out,int *index_out,int num) {SequenceMinSubset(F32,proc,data_in,index_in,num,data_out,index_out);}
           int mSequenceMinSubsetS32(MObject *proc,           int data_in,int index_in,           int *data_out,int *index_out,int num) {SequenceMinSubset(S32,proc,data_in,index_in,num,data_out,index_out);}
  unsigned int mSequenceMinSubsetU32(MObject *proc,  unsigned int data_in,int index_in,  unsigned int *data_out,int *index_out,int num) {SequenceMinSubset(U32,proc,data_in,index_in,num,data_out,index_out);}
         short mSequenceMinSubsetS16(MObject *proc,         short data_in,int index_in,         short *data_out,int *index_out,int num) {SequenceMinSubset(S16,proc,data_in,index_in,num,data_out,index_out);}
unsigned short mSequenceMinSubsetU16(MObject *proc,unsigned short data_in,int index_in,unsigned short *data_out,int *index_out,int num) {SequenceMinSubset(U16,proc,data_in,index_in,num,data_out,index_out);}
          char mSequenceMinSubsetS8( MObject *proc,          char data_in,int index_in,          char *data_out,int *index_out,int num) {SequenceMinSubset(S8, proc,data_in,index_in,num,data_out,index_out);}
 unsigned char mSequenceMinSubsetU8( MObject *proc, unsigned char data_in,int index_in, unsigned char *data_out,int *index_out,int num) {SequenceMinSubset(U8, proc,data_in,index_in,num,data_out,index_out);}

#define endSequenceMaxSubsetD64 endSequenceSort
#define endSequenceMaxSubsetF32 endSequenceSort
#define endSequenceMaxSubsetS32 endSequenceSort
#define endSequenceMaxSubsetU32 endSequenceSort
#define endSequenceMaxSubsetS16 endSequenceSort
#define endSequenceMaxSubsetU16 endSequenceSort
#define endSequenceMaxSubsetS8  endSequenceSort
#define endSequenceMaxSubsetU8  endSequenceSort
#define HASH_SequenceMaxSubsetD64 0x0a6c43ce
#define HASH_SequenceMaxSubsetF32 0x9d7f8a87
#define HASH_SequenceMaxSubsetS32 0x981be396
#define HASH_SequenceMaxSubsetU32 0x97351230
#define HASH_SequenceMaxSubsetS16 0x041f9040
#define HASH_SequenceMaxSubsetU16 0x2b316586
#define HASH_SequenceMaxSubsetS8  0x58eb83eb
#define HASH_SequenceMaxSubsetU8  0xccef3b7d
#define HandleSequenceMaxSubsetD64 HandleSequenceSort
#define HandleSequenceMaxSubsetF32 HandleSequenceSort
#define HandleSequenceMaxSubsetS32 HandleSequenceSort
#define HandleSequenceMaxSubsetU32 HandleSequenceSort
#define HandleSequenceMaxSubsetS16 HandleSequenceSort
#define HandleSequenceMaxSubsetU16 HandleSequenceSort
#define HandleSequenceMaxSubsetS8  HandleSequenceSort
#define HandleSequenceMaxSubsetU8  HandleSequenceSort
 
#define SequenceMaxSubset(Type,Proc,Data_in,Index_in,Num,Data_out,Index_out) {\
    struct SortTree *tree;\
    struct SortTree *node;\
    Type Limit;\
    \
    int order;\
    if(Num <= 1)\
    {\
        mSequenceBorder##Type(Proc,Data_in,Index_in,NULL,NULL,Data_out,Index_out);\
        Limit = *Data_out;\
        return Limit;\
    }\
    \
    MHandle *hdl; ObjectHandle(Proc,SequenceMaxSubset##Type,hdl);\
    struct HandleSequenceSort *handle = hdl->handle;\
    hdl->valid = 1;\
    \
    if(handle->num<Num)\
    {\
        node = (struct SortTree *)mMalloc(sizeof(struct SortTree));\
    }\
    else if(Data_in <= handle->thresh->data_##Type)\
    {\
        if((Data_out != NULL)||(Index_out!=NULL))\
        {\
            order = 0;\
            SortTreeResult##Type(handle->tree,Data_out,Index_out,&order);\
        }\
        Limit = handle->thresh->data_##Type;\
        return Limit;\
    }\
    else\
    {\
        node = handle->thresh;\
        if(node->parent != NULL)\
        {\
            node->parent->right = node->left;\
            if(node->left != NULL)\
                node->left->parent = node->parent;\
            handle->thresh = node->parent;\
        }\
        else\
        {\
            handle->tree = node->left;\
            node->left->parent = NULL;\
            handle->thresh = node->left;\
        }\
    }\
    \
    node->data_##Type = Data_in;\
    node->index = Index_in;\
    node->left = NULL;\
    node->right = NULL;\
    \
    if(handle->tree == NULL)\
    {\
        node->parent = NULL;\
        handle->tree = node;\
        handle->thresh = node;\
    }\
    else\
    {\
        tree = handle->tree;\
        \
        while(1)\
        {\
            if(Data_in >= tree->data_##Type)\
            {\
                if(tree->left == NULL)\
                {\
                    node->parent = tree;\
                    tree->left = node;\
                    break;\
                }\
                else\
                    tree = tree->left;\
            }\
            else\
            {\
                if(tree->right == NULL)\
                {\
                    node->parent = tree;\
                    tree->right = node;\
                    break;\
                }\
                else\
                    tree = tree->right;\
            }\
        }\
    }\
    handle->num = handle->num +1;\
    \
    while(handle->thresh->right != NULL)\
        handle->thresh = handle->thresh->right;\
    Limit = handle->thresh->data_##Type;\
    \
    if((Data_out != NULL)||(Index_out!=NULL))\
    {\
        order = 0;\
        SortTreeResult##Type(handle->tree,Data_out,Index_out,&order);\
    }\
    return Limit;\
}

        double mSequenceMaxSubsetD64(MObject *proc,        double data_in,int index_in,        double *data_out,int *index_out,int num) {SequenceMaxSubset(D64,proc,data_in,index_in,num,data_out,index_out);}
         float mSequenceMaxSubsetF32(MObject *proc,         float data_in,int index_in,         float *data_out,int *index_out,int num) {SequenceMaxSubset(F32,proc,data_in,index_in,num,data_out,index_out);}
           int mSequenceMaxSubsetS32(MObject *proc,           int data_in,int index_in,           int *data_out,int *index_out,int num) {SequenceMaxSubset(S32,proc,data_in,index_in,num,data_out,index_out);}
  unsigned int mSequenceMaxSubsetU32(MObject *proc,  unsigned int data_in,int index_in,  unsigned int *data_out,int *index_out,int num) {SequenceMaxSubset(U32,proc,data_in,index_in,num,data_out,index_out);}
         short mSequenceMaxSubsetS16(MObject *proc,         short data_in,int index_in,         short *data_out,int *index_out,int num) {SequenceMaxSubset(S16,proc,data_in,index_in,num,data_out,index_out);}
unsigned short mSequenceMaxSubsetU16(MObject *proc,unsigned short data_in,int index_in,unsigned short *data_out,int *index_out,int num) {SequenceMaxSubset(U16,proc,data_in,index_in,num,data_out,index_out);}
          char mSequenceMaxSubsetS8( MObject *proc,          char data_in,int index_in,          char *data_out,int *index_out,int num) {SequenceMaxSubset(S8, proc,data_in,index_in,num,data_out,index_out);}
 unsigned char mSequenceMaxSubsetU8( MObject *proc, unsigned char data_in,int index_in, unsigned char *data_out,int *index_out,int num) {SequenceMaxSubset(U8, proc,data_in,index_in,num,data_out,index_out);}

#define SequenceSortResult(Type,Proc,Data_out,Index_out) {\
    mException((Data_out ==NULL)&&(Index_out==NULL),EXIT,"invalid input,with data out is %p,index out is %p",Data_out,Index_out);\
    \
    MHandle *hdl; ObjectHandle(Proc,SequenceSort##Type,hdl);\
    struct HandleSequenceSort *handle = hdl->handle;\
    mException((hdl->valid == 0),EXIT,"invalid input");\
    \
    int order = 0;\
    SortTreeResult##Type(handle->tree,Data_out,Index_out,&order);\
}
void mSequenceSortResultD64(MObject *proc,        double *data_out,int *index_out) {SequenceSortResult(D64,proc,data_out,index_out);}
void mSequenceSortResultF32(MObject *proc,         float *data_out,int *index_out) {SequenceSortResult(F32,proc,data_out,index_out);}
void mSequenceSortResultS32(MObject *proc,           int *data_out,int *index_out) {SequenceSortResult(S32,proc,data_out,index_out);}
void mSequenceSortResultU32(MObject *proc,  unsigned int *data_out,int *index_out) {SequenceSortResult(U32,proc,data_out,index_out);}
void mSequenceSortResultS16(MObject *proc,         short *data_out,int *index_out) {SequenceSortResult(S16,proc,data_out,index_out);}
void mSequenceSortResultU16(MObject *proc,unsigned short *data_out,int *index_out) {SequenceSortResult(U16,proc,data_out,index_out);}
void mSequenceSortResultS8( MObject *proc,          char *data_out,int *index_out) {SequenceSortResult(S8 ,proc,data_out,index_out);}
void mSequenceSortResultU8( MObject *proc, unsigned char *data_out,int *index_out) {SequenceSortResult(U8 ,proc,data_out,index_out);}

#define mSequenceMinSubsetResultD64 mSequenceSortResultD64
#define mSequenceMinSubsetResultF32 mSequenceSortResultF32
#define mSequenceMinSubsetResultS32 mSequenceSortResultS32
#define mSequenceMinSubsetResultU32 mSequenceSortResultU32
#define mSequenceMinSubsetResultS16 mSequenceSortResultS16
#define mSequenceMinSubsetResultU16 mSequenceSortResultU16
#define mSequenceMinSubsetResultS8  mSequenceSortResultS8
#define mSequenceMinSubsetResultU8  mSequenceSortResultU8

#define mSequenceMaxSubsetResultD64 mSequenceSortResultD64
#define mSequenceMaxSubsetResultF32 mSequenceSortResultF32
#define mSequenceMaxSubsetResultS32 mSequenceSortResultS32
#define mSequenceMaxSubsetResultU32 mSequenceSortResultU32
#define mSequenceMaxSubsetResultS16 mSequenceSortResultS16
#define mSequenceMaxSubsetResultU16 mSequenceSortResultU16
#define mSequenceMaxSubsetResultS8  mSequenceSortResultS8
#define mSequenceMaxSubsetResultU8  mSequenceSortResultU8
 
#define MinSubset(Type,Data_in,Index_in,Num_in,Data_out,Index_out,Num_out,Limit) {\
    int I;\
    MObject *Proc;\
    Proc = mObjectCreate(NULL);\
    if((Index_in == NULL)||(Index_out==NULL))\
    {\
        for(I=0;I<Num_in;I++)\
           Limit = mSequenceMinSubset##Type(Proc,Data_in[I],I,NULL,NULL,Num_out);\
    }\
    else\
    {\
        for(I=0;I<Num_in;I++)\
           Limit = mSequenceMinSubset##Type(Proc,Data_in[I],Index_in[I],NULL,NULL,Num_out);\
    }\
    mSequenceMinSubsetResult##Type(Proc,Data_out,Index_out);\
    mObjectRelease(Proc);\
}
    
        double mMinSubsetD64(        double *data_in,int *index_in,int num_in,        double *data_out,int *index_out,int num_out) {        double limit=0;MinSubset(D64,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
         float mMinSubsetF32(         float *data_in,int *index_in,int num_in,         float *data_out,int *index_out,int num_out) {         float limit=0;MinSubset(F32,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
           int mMinSubsetS32(           int *data_in,int *index_in,int num_in,           int *data_out,int *index_out,int num_out) {           int limit=0;MinSubset(S32,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
  unsigned int mMinSubsetU32(  unsigned int *data_in,int *index_in,int num_in,  unsigned int *data_out,int *index_out,int num_out) {  unsigned int limit=0;MinSubset(U32,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
         short mMinSubsetS16(         short *data_in,int *index_in,int num_in,         short *data_out,int *index_out,int num_out) {         short limit=0;MinSubset(S16,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
unsigned short mMinSubsetU16(unsigned short *data_in,int *index_in,int num_in,unsigned short *data_out,int *index_out,int num_out) {unsigned short limit=0;MinSubset(U16,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
          char mMinSubsetS8(           char *data_in,int *index_in,int num_in,          char *data_out,int *index_out,int num_out) {          char limit=0;MinSubset(S8 ,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
 unsigned char mMinSubsetU8(  unsigned char *data_in,int *index_in,int num_in, unsigned char *data_out,int *index_out,int num_out) { unsigned char limit=0;MinSubset(U8 ,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}

 #define MaxSubset(Type,Data_in,Index_in,Num_in,Data_out,Index_out,Num_out,Limit) {\
    int I;\
    MObject *Proc;\
    Proc = mObjectCreate(NULL);\
    if((Index_in == NULL)||(Index_out==NULL))\
    {\
        for(I=0;I<Num_in;I++)\
           Limit = mSequenceMaxSubset##Type(Proc,Data_in[I],I,NULL,NULL,Num_out);\
    }\
    else\
    {\
        for(I=0;I<Num_in;I++)\
           Limit = mSequenceMaxSubset##Type(Proc,Data_in[I],Index_in[I],NULL,NULL,Num_out);\
    }\
    mSequenceMaxSubsetResult##Type(Proc,Data_out,Index_out);\
    mObjectRelease(Proc);\
}

        double mMaxSubsetD64(        double *data_in,int *index_in,int num_in,        double *data_out,int *index_out,int num_out) {        double limit=0;MaxSubset(D64,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
         float mMaxSubsetF32(         float *data_in,int *index_in,int num_in,         float *data_out,int *index_out,int num_out) {         float limit=0;MaxSubset(F32,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
           int mMaxSubsetS32(           int *data_in,int *index_in,int num_in,           int *data_out,int *index_out,int num_out) {           int limit=0;MaxSubset(S32,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
  unsigned int mMaxSubsetU32(  unsigned int *data_in,int *index_in,int num_in,  unsigned int *data_out,int *index_out,int num_out) {  unsigned int limit=0;MaxSubset(U32,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
         short mMaxSubsetS16(         short *data_in,int *index_in,int num_in,         short *data_out,int *index_out,int num_out) {         short limit=0;MaxSubset(S16,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
unsigned short mMaxSubsetU16(unsigned short *data_in,int *index_in,int num_in,unsigned short *data_out,int *index_out,int num_out) {unsigned short limit=0;MaxSubset(U16,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
          char mMaxSubsetS8(           char *data_in,int *index_in,int num_in,          char *data_out,int *index_out,int num_out) {          char limit=0;MaxSubset(S8 ,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
 unsigned char mMaxSubsetU8(  unsigned char *data_in,int *index_in,int num_in, unsigned char *data_out,int *index_out,int num_out) { unsigned char limit=0;MaxSubset(U8 ,data_in,index_in,num_in,data_out,index_out,num_out,limit);return limit;}
 
#define MidValue(Type,Data_in,Num_in) {\
    mException((INVALID_POINTER(Data_in))||(Num_in<1),EXIT,"invalid input");\
    if(Num_in<=2) return Data_in[0];\
    \
    int Num = Num_in;\
    Type *buff = mMalloc(Num*sizeof(Type));\
    memcpy(buff,Data_in,Num*sizeof(Type));\
    Data_in = buff;\
    \
    int left_num = 0;\
    int right_num = 0;\
    while(1)\
    {\
        Type Thresh = Data_in[0];\
        int I=0;int J=Num-1;\
        while(I<J)\
        {\
            while(Data_in[J]>=Thresh)\
            {\
                if(J==I)\
                    goto MidValue_next;\
                J=J-1;\
            }\
            \
            Data_in[I] = Data_in[J];\
            \
            while(Data_in[I]<=Thresh)\
            {\
                if(I==J)\
                    goto MidValue_next;\
                I=I+1;\
            }\
            \
            Data_in[J] = Data_in[I];\
        }\
        \
        MidValue_next:\
        if(left_num<right_num){left_num+=I;right_num= Num_in- left_num-1;}\
        else         {right_num+=(Num-I-1);left_num = Num_in-right_num-1;}\
        \
        if(left_num > right_num)\
        {\
            Num = I;\
            right_num += 1;\
        }\
        else if(left_num < right_num)\
        {\
            Data_in = Data_in+I+1;\
            Num = Num-I-1;\
            left_num += 1;\
        }\
        \
        if(ABS(left_num-right_num)<=1)\
        {\
            mFree(buff);\
            return Thresh;\
        }\
    }\
}

        double mMidValueD64(        double *data_in,int num_in) MidValue(D64,data_in,num_in)
         float mMidValueF32(         float *data_in,int num_in) MidValue(F32,data_in,num_in)
           int mMidValueS32(           int *data_in,int num_in) MidValue(S32,data_in,num_in)
  unsigned int mMidValueU32(  unsigned int *data_in,int num_in) MidValue(U32,data_in,num_in)
         short mMidValueS16(         short *data_in,int num_in) MidValue(S16,data_in,num_in)
unsigned short mMidValueU16(unsigned short *data_in,int num_in) MidValue(U16,data_in,num_in)
   signed char mMidValueS8(    signed char *data_in,int num_in) MidValue(S8, data_in,num_in)
 unsigned char mMidValueU8(  unsigned char *data_in,int num_in) MidValue(U8, data_in,num_in)