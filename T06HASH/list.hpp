
#ifndef LIST_HPP_INCL_
#define LIST_HPP_INCL_

#include "local-utils.hpp"

class List {

/* List data types */
public:

  /* List elements type */
    struct ListElem_t {

      /* This element data */
        HashTableUnit_t data_;

      /* Prev & next ptrs */
        ListElem_t* prev_ = nullptr;
        ListElem_t* next_ = nullptr;

    };

  /* To choose prev/next option in methods */
    enum class listElemSide_t {
          PREV_             = 'P',
          NEXT_             = 'N'
    };

/* Fields */
private:

  /* head and tail of list */
    ListElem_t* head_ = nullptr; 
    ListElem_t* tail_ = nullptr;

/* Methods */
public:

  /* Ctor & dtor */
    List();
   ~List();

  /* deleted stuff */
    List           ( const List& ) = delete;
    List operator =( const List& ) = delete;

  /* To get data by ListElem_t ptr */
    HashTableUnit_t getData( ListElem_t* listElemPtr );

  /* To get List head & tail */
    ListElem_t* getHead();
    ListElem_t* getTail();

  /* To get ptr to prev/next list element */
    ListElem_t* getPrevOrNext( ListElem_t* listElemPtr, listElemSide_t side );

  /* To add new list element prev/next to choosed list element;
   * It adds head if list is empty
   */
    ListElem_t* addPrevOrNext( ListElem_t* listElemPtr, listElemSide_t side,
                               HashTableUnit_t newElemData );
    
  /* To delete list element */
    Error_t delElem( ListElem_t* listElem2DelPtr );

  /* To delete all list elements */
    Error_t reset();

  /* Text dump */
    size_t dump( char* buff );
};

#endif