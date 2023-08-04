#include "OSAlloc.h"

/*
#define InRange(addr, start, end)                                             \
    ((u8*) (start) <= (u8*) (addr) && (u8*) (addr) < (u8*) (end))
#define OFFSET(addr, align) (((uintptr_t) (addr) & ((align) -1)))

#define ALIGNMENT 32
#define MINOBJSIZE 64

#define NULL ((any_t) 0)

void unlink_node(struct Heap *heap, struct ListNode *node)
{
    struct ListNode *head = heap->head;  
        
        if (node->next != NULL)
            node->next->prev = node->prev;

        if (node->prev == NULL)
            head = node->next;
        else
            node->prev->next = node->next;
        heap->head = head;
}

void *OSAllocFromHeap(OSHeapHandle handle, u32 size)
{
    struct Heap *heap = &HeapArray[handle];
    s32 sizeAligned = OSRoundUp32B(32 + size);
    struct ListNode *node;
    struct ListNode *oldTail;
    u32 leftoverSpace;
    
    // find first node with enough capacity
    for (node = heap->head; node != NULL; node = node->next)
    {
        if (sizeAligned <= node->size)
            break;
    }
    if (node == NULL)
        return NULL;

    leftoverSpace = node->size - sizeAligned;
    if (leftoverSpace < 64)
    {
        // remove this node from list
        unlink_node(heap, node);
    }
    else
    {
        // make a new node out of the remaining space and remove the current node
        struct ListNode *newNode;
        newNode = (void *)((u8 *)node + sizeAligned);        
        node->size = sizeAligned;
        newNode->size = leftoverSpace;
        newNode->prev = node->prev;
        newNode->next = node->next;
        if (newNode->next != NULL)
            newNode->next->prev = newNode;
        if (newNode->prev != NULL)
            newNode->prev->next = newNode;
        else
            heap->head = newNode;
    }
    // add this node to the end?
    oldTail = heap->tail;
    node->next = oldTail;
    node->prev = NULL;
    if (oldTail != NULL)
        oldTail->prev = node;
    heap->tail = node;
    return (u8 *)node + 32;
}


size_t OSCheckHeap(OSHeapHandle heap)
{
    Heap* hd;
    HeapCell* cell;
    int total = 0;
    int totalFree = 0;

#define CHECK(line, condition)                                                \
    if (!(condition)) {                                                       \
        OSReport("OSCheckHeap: Failed " #condition " in %d", line);           \
        return -1;                                                            \
    }

    CHECK(893, HeapArray)
    CHECK(894, 0 <= heap && heap < NumHeaps)
    hd = &HeapArray[heap];
    CHECK(897, 0 <= hd->size)
    CHECK(899, hd->allocated == NULL || hd->allocated->prev == NULL)

    for (cell = hd->allocated; cell != NULL; cell = cell->next) {
        CHECK(902, InRange(cell, ArenaStart, ArenaEnd))
        CHECK(903, OFFSET(cell, ALIGNMENT) == 0)
        CHECK(904, cell->next == NULL || cell->next->prev == cell)
        CHECK(905, MINOBJSIZE <= cell->size)
        CHECK(906, OFFSET(cell->size, ALIGNMENT) == 0)
        total += cell->size;
        CHECK(909, 0 < total && total <= hd->size)
    }

    CHECK(917, hd->free == NULL || hd->free->prev == NULL)
    for (cell = hd->free; cell != NULL; cell = cell->next) {
        CHECK(920, InRange(cell, ArenaStart, ArenaEnd))
        CHECK(921, OFFSET(cell, ALIGNMENT) == 0)
        CHECK(922, cell->next == NULL || cell->next->prev == cell)
        CHECK(923, MINOBJSIZE <= cell->size)
        CHECK(924, OFFSET(cell->size, ALIGNMENT) == 0)
        CHECK(925, cell->next == NULL ||
                       (char*) cell + cell->size < (char*) cell->next)
        total += cell->size;
        totalFree += cell->size - 32;
        CHECK(929, 0 < total && total <= hd->size)
    }

    CHECK(936, total == hd->size);

#undef CHECK

    return totalFree;
}



void OSFreeToHeap(OSHeapHandle heap, any_t ptr)
{
    HeapCell* cell = (any_t) ((u8*) ptr - ALIGNMENT);
    Heap* hd = &HeapArray[heap];
    HeapCell* list = hd->allocated;

    // remove cell from the allocated list
    // hd->allocated = DLExtract(hd->allocated, cell);
    if (cell->next != NULL) {
        cell->next->prev = cell->prev;
    }
    if (cell->prev == NULL) {
        list = cell->next;
    } else {
        cell->prev->next = cell->next;
    }

    hd->allocated = list;
    hd->free = DLInsert(hd->free, cell); //hd->free = DLInsert(hd->free, cell, list);
}

static HeapCell* DLInsert(HeapCell* list, HeapCell* cell)
{
    HeapCell* before = NULL;
    HeapCell* after = list;

    while (after != NULL) {
        if (cell <= after) {
            break;
        }
        before = after;
        after = after->next;
    }
    cell->next = after;
    cell->prev = before;
    if (after != NULL) {
        after->prev = cell;
        if ((u8*) cell + cell->size == (u8*) after) {
            cell->size += after->size;
            after = after->next;
            cell->next = after;
            if (after != NULL) {
                after->prev = cell;
            }
        }
    }
    if (before != NULL) {
        before->next = cell;
        if ((u8*) before + before->size == (u8*) cell) {
            before->size += cell->size;
            before->next = after;
            if (after != NULL) {
                after->prev = before;
            }
        }
        return list;
    }
    return cell;
}

OSHeapHandle OSSetCurrentHeap(OSHeapHandle heap)
{
    OSHeapHandle old = __OSCurrHeap;

    __OSCurrHeap = heap;
    return old;
}

any_t OSInitAlloc(any_t arenaStart, any_t arenaEnd, int maxHeaps)
{
    u32 totalSize = maxHeaps * sizeof(Heap);
    int i;

    HeapArray = arenaStart;
    NumHeaps = maxHeaps;

    for (i = 0; i < NumHeaps; i++) {
        Heap* heap = &HeapArray[i];

        heap->size = -1;
        heap->free = heap->allocated = NULL;
    }

    __OSCurrHeap = -1;

    arenaStart = (u8*) HeapArray + totalSize;
    arenaStart = (any_t) OSRoundUp32B(arenaStart);

    ArenaStart = arenaStart;
    ArenaEnd = (any_t) OSRoundDown32B(arenaEnd);

    return arenaStart;
}

OSHeapHandle OSCreateHeap(any_t start, any_t end)
{
    int i;
    HeapCell* cell = (any_t) OSRoundUp32B(start);

    end = (any_t) OSRoundDown32B(end);
    for (i = 0; i < NumHeaps; i++) {
        Heap* hd = &HeapArray[i];

        if (hd->size < 0) {
            hd->size = (u8*) end - (u8*) cell;
            cell->prev = NULL;
            cell->next = NULL;
            cell->size = hd->size;
            hd->free = cell;
            hd->allocated = NULL;
            return i;
        }
    }
    return -1;
}

void OSDestroyHeap(size_t idx)
{
    *(s32*) &HeapArray[idx] = -1;
}

*/
