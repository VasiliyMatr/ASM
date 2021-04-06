
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

List::ListElem_t* List::getPrevOrNext( ListElem_t* listElemP, ListElemSide_t side )
{
    if (isBadPtr (listElemP))
        return nullptr;

    if (side == ListElemSide_t::NEXT_)
        return listElemP->nextP_;

    return listElemP->prevP_;
}

List::ListElem_t* List::addPrevOrNext( ListElem_t* listElemP, ListElemSide_t side,
                                       HashTableUnit_t newElemData )
{
    /* Empty list */
    if (listElemP == nullptr)
    {
        if (headP_ == nullptr)
        {
            /* Allocating */
            headP_ = (ListElem_t* )calloc (1, sizeof (ListElem_t));
            if (headP_ == nullptr)
                return nullptr;

            /* Initing head & tail */
            headP_->listElemData_ = newElemData;
            headP_->nextP_        = nullptr;
            headP_->prevP_        = nullptr;
            tailP_ = headP_;

            /* All is ok */
            return tailP_;
        }

        /* Error */
        return nullptr;
    }

  /* Allocation */
    ListElem_t* newElemP = (ListElem_t* )calloc (1, sizeof (ListElem_t));
    if (newElemP == nullptr)
        return nullptr;

  /* writing new data */
    newElemP->listElemData_ = newElemData;

  /* Interconnect ptrs */
    if (side == ListElemSide_t::NEXT_)
    {
        newElemP->nextP_  = listElemP->nextP_;
        newElemP->prevP_  = listElemP;
        listElemP->nextP_ = newElemP;
        
        if (newElemP->nextP_ != nullptr)
            newElemP->nextP_->prevP_ = newElemP;

        /* Attached on tail */
        if (listElemP == tailP_)
            tailP_ = newElemP;
    }

    else
    {
        newElemP->nextP_  = listElemP;
        newElemP->prevP_  = listElemP->prevP_;
        listElemP->prevP_ = newElemP;

        if (newElemP->prevP_ != nullptr)
            newElemP->prevP_->nextP_ = newElemP;

        /* Attached on head */
        if (listElemP == headP_)
            headP_ = newElemP;
    }

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