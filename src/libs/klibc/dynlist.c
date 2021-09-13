#include <types.h>
#include <libs/dynlist.h>
#include <kernel/mem/pmm.h>



int dl_total(dynlist *v)
{
    int totalCount = UNDEFINE;
    if(v)
    {
        totalCount = v->dl.total;
    }
    return totalCount;
}



int dl_resize(dynlist *v, int capacity)
{
    int  status = UNDEFINE;
    if(v)
    {
        void **items = pmm_realloc(v->dl.items, sizeof(void *) * capacity);
        if (items)
        {
            v->dl.items = items;
            v->dl.capacity = capacity;
            status = SUCCESS;
        }
    }
    return status;
}


int dl_push(dynlist *v, void *item)
{
    int  status = UNDEFINE;
    if(v)
    {
        if (v->dl.capacity == v->dl.total)
        {
            status = dl_resize(v, v->dl.capacity * 2);
            if(status != UNDEFINE)
            {
                v->dl.items[v->dl.total++] = item;
            }
        }
        else
        {
            v->dl.items[v->dl.total++] = item;
            status = SUCCESS;
        }
    }
    return status;
}
int dl_set(dynlist *v, int index, void *item)
{
    int  status = UNDEFINE;
    if(v)
    {
        if ((index >= 0) && (index < v->dl.total))
        {
            v->dl.items[index] = item;
            status = SUCCESS;
        }
    }
    return status;
}

void *dl_get(dynlist *v, int index)
{
    void *readData = NULL;
    if(v)
    {
        if ((index >= 0) && (index < v->dl.total))
        {
            readData = v->dl.items[index];
        }
    }
    return readData;
}
int dl_pop(dynlist *v, int index)
{
    int  status = UNDEFINE;
    int i = 0;
    if(v)
    {
        if ((index < 0) || (index >= v->dl.total))
            return status;
        v->dl.items[index] = NULL;
        for (i = index; (i < v->dl.total - 1); ++i)
        {
            v->dl.items[i] = v->dl.items[i + 1];
            v->dl.items[i + 1] = NULL;
        }
        v->dl.total--;
        if ((v->dl.total > 0) && ((v->dl.total) == (v->dl.capacity / 4)))
        {
            dl_resize(v, v->dl.capacity / 2);
        }
        status = SUCCESS;
    }
    return status;
}
int dl_free(dynlist *v)
{
    int  status = UNDEFINE;
    if(v)
    {
        pmm_free(v->dl.items);
        v->dl.items = NULL;
        status = SUCCESS;
    }
    return status;
}


void dl_init(dynlist *v)
{

    v->total = dl_total;
    v->resize = dl_resize;
    v->push = dl_push;
    v->set = dl_set;
    v->get = dl_get;
    v->free = dl_free;
    v->pop = dl_pop;
    v->dl.capacity = DL_INIT_CAPACITY;
    v->dl.total = 0;
    v->dl.items = pmm_alloc(sizeof(void *) * v->dl.capacity);
}