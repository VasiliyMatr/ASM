#include "list.hpp"

const size_t LIST_SIZE_ = 100;

int main()
{
    char* buff = (char* )calloc (1, LIST_SIZE_ * 100);
    if (buff == nullptr)
    {
        printf ("no mem for dump!" "\n");
        return -1;
    }

    List list;

    List::listElem_t* ptr = nullptr;

    for (int i = 0; i < LIST_SIZE_; i++)
        ptr = list.addPrevOrNext (ptr, List::listElemSide_t::NEXT_, i);

    for (int i = 0; i < LIST_SIZE_; i++)
    {
        printf ("%d" "\n", list.getData (ptr));
        ptr = list.getPrevOrNext (ptr, List::listElemSide_t::PREV_);
    }
}