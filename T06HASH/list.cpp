
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
        currElem = currElem->next_;
        free (freeElemAddr);
    }

    /* nulling all ptrs */
    head_ = nullptr;
    tail_ = nullptr;
}

HashTableUnit_t List::getData( listElem_t* listElemPtr )
{
    return listElemPtr->data_;
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
        return listElemPtr->next_;

    return listElemPtr->prev_;
}

List::listElem_t* List::addPrevOrNext( listElem_t* listElemPtr, listElemSide_t side,
                                       HashTableUnit_t newElemData )
{
    if (listElemPtr == nullptr)
    {
        if (head_ == nullptr)
        {
            head_ = (listElem_t* )calloc (1, sizeof (listElem_t));
            if (head_ == nullptr) return nullptr;

            head_->data_ = newElemData;
            head_->next_ = nullptr;
            head_->prev_ = nullptr;

            tail_ = head_;
            return tail_;
        }

        return nullptr;
    }

  /* mem allocation */
    listElem_t* newElem = (listElem_t* )calloc (1, sizeof (listElem_t));
    if (newElem == nullptr) return nullptr;

  /* writing new data */
    newElem->data_ = newElemData;

  /* interconnect ptrs */
    if (side == listElemSide_t::NEXT_)
    {
        newElem->next_      = listElemPtr->next_;
        newElem->prev_      = listElemPtr;
        listElemPtr->next_  = newElem;
        
        if (newElem->next_ != nullptr)
            newElem->next_->prev_ = newElem;

        if (listElemPtr == tail_)
            tail_ = newElem;
    }

    else
    {
        newElem->next_      = listElemPtr;
        newElem->prev_      = listElemPtr->prev_;
        listElemPtr->prev_   = newElem;

        if (newElem->prev_ != nullptr)
            newElem->prev_->next_ = newElem;

        if (listElemPtr == head_)
            head_ = newElem;
    }

    return newElem;
}

Error_t List::delElem( listElem_t* listElem2DelPtr )
{
    if (listElem2DelPtr == nullptr)
        return Error_t::PTR_ERR_;

    if (listElem2DelPtr->next_ != nullptr)
        listElem2DelPtr->next_->prev_ = listElem2DelPtr->prev_;
    else
        tail_ = listElem2DelPtr->prev_;
    
    if (listElem2DelPtr->prev_ != nullptr)
        listElem2DelPtr->prev_->next_ = listElem2DelPtr->next_;
    else
        head_ = listElem2DelPtr->next_;

    listElem2DelPtr->next_ = nullptr;
    listElem2DelPtr->prev_ = nullptr;

    free (listElem2DelPtr);

    return Error_t::OK_;
}

size_t List::dump( char* buff )
{
    listElem_t* currElemAddr = head_;
    int shift = 0;

    sprintf (buff, "\tLIST:" "\n" "%n", &shift);

    while (currElemAddr != nullptr)
    {
        shift += printData (buff + shift, currElemAddr->data_);
        currElemAddr = currElemAddr->next_;
    }

    return shift;
}