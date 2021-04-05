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
        ptr = list.addPrevOrNext (ptr, List::listElemSide_t::NEXT_,
                                  HashTableUnit_t{ "zalupa", 10 });

    for (int i = 0; i < LIST_SIZE_; i++)
    {
        printf ("%s" "\n", (list.getData (ptr)).data_);
        ptr = list.getPrevOrNext (ptr, List::listElemSide_t::PREV_);
    }

    list.dump (buff);

    printf ("%s", buff);

    free (buff);
}