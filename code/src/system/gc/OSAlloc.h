#pragma once
#include "system.h"
#include "system/gxtype.h"
#include "system/gc/GXBump.h"

s32 __OSCurrHeap;
static struct Heap* HeapArray;

typedef struct ListNode
{
    struct ListNode *prev;
    struct ListNode *next;
    s32 size;
};

typedef struct HeapCell {
	struct HeapCell* prev;
	struct HeapCell* next;
	u32 size;
} HeapCell;

typedef struct Heap {
	s32 size;
	struct HeapCell* free;      // linked list of free cells
	struct HeapCell* allocated; // linked list of allocated cells
} Heap;
