
#ifndef LIST_HPP_INCL_
#define LIST_HPP_INCL_

#include "local-utils.hpp"

class List {

/* List data types */
public:

  /* List elements type */
    struct ListElem_t {

      /* This element data */
        HashTableUnit_t listElemData_;

      /* Prev & next ptrs */
        ListElem_t* prevP_ = nullptr;
        ListElem_t* nextP_ = nullptr;

    };

  /* To choose prev/next option in methods */
    enum class ListElemSide_t {

          PREV_             = 'P',
          NEXT_             = 'N'

    };

/* Fields */
private:

  /* head and tail of list */
    ListElem_t* headP_ = nullptr; 
    ListElem_t* tailP_ = nullptr;

  /* List size */
    size_t size_ = 0;

/* Methods */
public:

  /* Ctor & dtor */
    List();
   ~List();

  /* deleted stuff */
    List           ( const List& ) = delete;
    List operator =( const List& ) = delete;

  /* To get data by ListElem_t ptr */
    HashTableUnit_t& getData( ListElem_t* listElemP );

  /* To get List headP_ & tailP_ & size_ */
    ListElem_t* getHeadP();
    ListElem_t* getTailP();

    size_t getSize();

  /* To get ptr to prev/next list element */
    ListElem_t* getPrevOrNext( ListElem_t* listElemP, ListElemSide_t side );

  /* To add elements to tail/head */
    ListElem_t* add2Head( const HashTableUnit_t& newElemData );
    ListElem_t* add2Tail( const HashTableUnit_t& newElemData );

  /* To add new list element prev/next to choosed list element;
   * It adds head if list is empty
   */
    ListElem_t* addPrevOrNext( ListElem_t* listElemP, ListElemSide_t side,
                               const HashTableUnit_t& newElemData );
    
  /* To delete list element */
    Error_t delElem( ListElem_t* listElem2DelP );

  /* To delete all list elements */
    Error_t reset();

  /* Text dump */
    size_t dump( char* const buffP );
};

#endif