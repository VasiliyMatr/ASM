
#include "../headers/list.hpp"

List::List()
{}

List::~List()
{
    ListElem_t* currElem = head_;

    /* free all memory */
    while (currElem != nullptr)
    {
        ListElem_t* freeElemAddr = currElem;
        currElem = currElem->next_;
        free (freeElemAddr);
    }

    /* nulling all ptrs */
    head_ = nullptr;
    tail_ = nullptr;
}

HashTableUnit_t List::getData( ListElem_t* listElemPtr )
{
    return listElemPtr->data_;
}


List::ListElem_t* List::getHead()
{
    return head_;
}

List::ListElem_t* List::getTail()
{
    return tail_;
}

List::ListElem_t* List::getPrevOrNext( ListElem_t* listElemPtr, listElemSide_t side )
{
    if (listElemPtr == nullptr)
        return nullptr;

    if (side == listElemSide_t::NEXT_)
        return listElemPtr->next_;

    return listElemPtr->prev_;
}

List::ListElem_t* List::addPrevOrNext( ListElem_t* listElemPtr, listElemSide_t side,
                                       HashTableUnit_t newElemData )
{
    if (listElemPtr == nullptr)
    {
        if (head_ == nullptr)
        {
            head_ = (ListElem_t* )calloc (1, sizeof (ListElem_t));
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
    ListElem_t* newElem = (ListElem_t* )calloc (1, sizeof (ListElem_t));
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

Error_t List::delElem( ListElem_t* listElem2DelPtr )
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

Error_t List::reset()
{
    while (head_ != nullptr)
    {
        Error_t error = delElem (head_);
        if (error != Error_t::OK_)
            return error;
    }

    return Error_t::OK_;
}

size_t List::dump( char* buff )
{
    ListElem_t* currElemAddr = head_;
    int shift = 0;

    sprintf (buff, "\tLIST:" "\n" "%n", &shift);

    while (currElemAddr != nullptr)
    {
        shift += printData (buff + shift, currElemAddr->data_);
        currElemAddr = currElemAddr->next_;
    }

    return shift;
}