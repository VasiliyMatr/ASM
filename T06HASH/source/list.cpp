
#include "../headers/list.hpp"

List::List()
{}

List::~List()
{
    ListElem_t* currElemP = headP_;

    /* Free all memory */
    while (currElemP != nullptr)
    {
        ListElem_t* freeElemP = currElemP;
        currElemP = currElemP->nextP_;
        free (freeElemP);
    }

    /* Nulling all ptrs */
    headP_ = nullptr;
    tailP_ = nullptr;
    size_  = 0;
}

HashTableUnit_t List::getData( ListElem_t* listElemP )
{
    return listElemP->listElemData_;
}


List::ListElem_t* List::getHeadP()
{
    return headP_;
}

List::ListElem_t* List::getTailP()
{
    return tailP_;
}


size_t List::getSize()
{
    return size_;
}

List::ListElem_t* List::getPrevOrNext( ListElem_t* listElemP, ListElemSide_t side )
{
    if (isBadPtr (listElemP))
        return nullptr;

    if (side == ListElemSide_t::NEXT_)
        return listElemP->nextP_;

    return listElemP->prevP_;
}

List::ListElem_t* List::add2Head ( HashTableUnit_t newElemData )
{
    /* Allocating */
    ListElem_t* newElemP = (ListElem_t* )calloc (1, sizeof (ListElem_t));
    if (newElemP == nullptr)
        return nullptr;

    newElemP->listElemData_ = newElemData;
    newElemP->prevP_ = nullptr;

    if (headP_ == nullptr)
    {
        headP_ = newElemP;

        /* Initing head & tail */
        headP_->nextP_        = nullptr;
        tailP_ = headP_;
    }

    else
    {
        newElemP->nextP_ = headP_;
        headP_->prevP_ = newElemP;
        headP_ = newElemP;
    }

    ++size_;

    /* All is ok */
    return newElemP;
}

List::ListElem_t* List::add2Tail ( HashTableUnit_t newElemData )
{
    /* Allocating */
    ListElem_t* newElemP = (ListElem_t* )calloc (1, sizeof (ListElem_t));
    if (newElemP == nullptr)
        return nullptr;

    newElemP->listElemData_ = newElemData;
    newElemP->nextP_ = nullptr;

    if (tailP_ == nullptr)
    {
        headP_ = newElemP;

        /* Initing head & tail */
        headP_->nextP_        = nullptr;
        tailP_ = headP_;
    }

    else
    {
        newElemP->prevP_ = tailP_;
        tailP_->nextP_ = newElemP;
        tailP_ = newElemP;
    }

    ++size_;

    /* All is ok */
    return newElemP;
}


List::ListElem_t* List::addPrevOrNext( ListElem_t* listElemP, ListElemSide_t side,
                                       HashTableUnit_t newElemData )
{
    /* Empty list */
    if (size_ == 0)
    {
        if (listElemP == nullptr)
            return add2Head (newElemData);

        return nullptr;
    }

    /* Head or tail attach */
    if (listElemP == tailP_ && side == ListElemSide_t::NEXT_)
        return add2Tail (newElemData);
    if (listElemP == headP_ && side == ListElemSide_t::PREV_)
        return add2Head (newElemData);

  /* Allocation */
    ListElem_t* newElemP = (ListElem_t* )calloc (1, sizeof (ListElem_t));
    if (newElemP == nullptr)
        return nullptr;

  /* writing new data */
    newElemP->listElemData_ = newElemData;

  /* Interconnect ptrs */
    if (side == ListElemSide_t::NEXT_)
    {
        newElemP->nextP_         = listElemP->nextP_;
        newElemP->prevP_         = listElemP;
        listElemP->nextP_        = newElemP;
        newElemP->nextP_->prevP_ = newElemP;
    }

    else
    {
        newElemP->nextP_         = listElemP;
        newElemP->prevP_         = listElemP->prevP_;
        listElemP->prevP_        = newElemP;
        newElemP->prevP_->nextP_ = newElemP;
    }

    ++size_;

    return newElemP;
}

Error_t List::delElem( ListElem_t* listElem2DelP )
{
    if (isBadPtr (listElem2DelP))
        return Error_t::PTR_ERR_;

    if (listElem2DelP->nextP_ != nullptr)
        listElem2DelP->nextP_->prevP_ = listElem2DelP->prevP_;
    else
        tailP_ = listElem2DelP->prevP_;
    
    if (listElem2DelP->prevP_ != nullptr)
        listElem2DelP->prevP_->nextP_ = listElem2DelP->nextP_;
    else
        headP_ = listElem2DelP->nextP_;

    listElem2DelP->nextP_ = nullptr;
    listElem2DelP->prevP_ = nullptr;

    free (listElem2DelP);

    --size_;
    
    return Error_t::OK_;
}

Error_t List::reset()
{
    while (headP_ != nullptr)
    {
        Error_t error = delElem (headP_);
        if (error != Error_t::OK_)
            return error;
    }

    return Error_t::OK_;
}

size_t List::dump( char* const buffP )
{
    ListElem_t* currElemP = headP_;
    int buffShift = 0;

    sprintf (buffP, "\tLIST:" "\n" "%n", &buffShift);

    while (currElemP != nullptr)
    {
        buffShift += printData (buffP + buffShift, currElemP->listElemData_);
        currElemP = currElemP->nextP_;
    }

    return buffShift;
}