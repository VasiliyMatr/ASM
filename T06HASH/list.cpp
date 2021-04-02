
#include "list.hpp"

List::List()
{}

List::~List()
{
    listElem_t* currElem = head_;

    /* free all memory */
    while (currElem != nullptr)
    {
        listElem_t* freeElemAddr = currElem;
        currElem = currElem->next;
        free (freeElemAddr);
    }

    /* nulling all ptrs */
    head_ = nullptr;
    tail_ = nullptr;
}

data_t List::getData( listElem_t* listElemPtr )
{
    return listElemPtr->data;
}


List::listElem_t* List::getHead()
{
    return head_;
}

List::listElem_t* List::getTail()
{
    return tail_;
}

List::listElem_t* List::getPrevOrNext( listElem_t* listElemPtr, listElemSide_t side )
{
    if (listElemPtr == nullptr)
        return nullptr;

    if (side == listElemSide_t::NEXT_)
        return listElemPtr->next;

    return listElemPtr->prev;
}

List::listElem_t* List::addPrevOrNext( listElem_t* listElemPtr, listElemSide_t side,
                               data_t newElemData )
{
    if (listElemPtr == nullptr)
    {
        if (head_ == nullptr)
        {
            head_ = (listElem_t* )calloc (1, sizeof (listElem_t));
            if (head_ == nullptr) return nullptr;

            head_->data = newElemData;
            head_->next = nullptr;
            head_->prev = nullptr;

            tail_ = head_;
            return tail_;
        }

        return nullptr;
    }

  /* mem allocation */
    listElem_t* newElem = (listElem_t* )calloc (1, sizeof (listElem_t));
    if (newElem == nullptr) return nullptr;

  /* writing new data */
    newElem->data = newElemData;

  /* interconnect ptrs */
    if (side == listElemSide_t::NEXT_)
    {
        newElem->next = listElemPtr->next;
        newElem->prev = listElemPtr;
        listElemPtr->next = newElem;
        
        if (newElem->next != nullptr)
            newElem->next->prev = newElem;

        if (listElemPtr == tail_)
            tail_ = newElem;
    }

    else
    {
        newElem->next = listElemPtr;
        newElem->prev = listElemPtr->prev;
        listElemPtr->prev = newElem;

        if (newElem->prev != nullptr)
            newElem->prev->next = newElem;

        if (listElemPtr == head_)
            head_ = newElem;
    }

    return newElem;
}

List::listError_t List::delElem( listElem_t* listElem2DelPtr )
{
    if (listElem2DelPtr == nullptr)
        return listError_t::PTR_ERR_;

    if (listElem2DelPtr->next != nullptr)
        listElem2DelPtr->next->prev = listElem2DelPtr->prev;
    else
        tail_ = listElem2DelPtr->prev;
    
    if (listElem2DelPtr->prev != nullptr)
        listElem2DelPtr->prev->next = listElem2DelPtr->next;
    else
        head_ = listElem2DelPtr->next;

    listElem2DelPtr->next = nullptr;
    listElem2DelPtr->prev = nullptr;

    free (listElem2DelPtr);

    return listError_t::OK_;
}

void List::dump( char* buff )
{
    listElem_t* currElemAddr = head_;
    int shift = 0;

    sprintf (buff, "\tLIST:" "\n" "%n", &shift);

    while (currElemAddr != nullptr)
        shift += printData (buff + shift, currElemAddr->data);
}