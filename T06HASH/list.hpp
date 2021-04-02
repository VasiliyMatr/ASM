
#include <cstdio>
#include <cstdlib>

typedef int data_t;

class List{

/* List data types */
public:

  /* List elements type */
    struct listElem_t {

      /* This element data */
        data_t data;

      /* Prev & next ptrs */
        listElem_t* prev = nullptr;
        listElem_t* next = nullptr;

    };

  /* To choose prev/next option in methods */
    enum class listElemSide_t {
          PREV_             = 'P',
          NEXT_             = 'N'
    };

  /* List error type - for errors output */
    enum class listError_t {
        OK_                 =   0       ,
        PTR_ERR_            =  -0xFFFF  ,
    };

/* Fields */
private:

  /* head and tail of list */
    listElem_t* head_ = nullptr; 
    listElem_t* tail_ = nullptr;

/* Methods */
public:

  /* Ctor & dtor */
   ~List();

  /* deleted stuff */
    List           ( const List& ) = delete;
    List operator =( const List& ) = delete;

  /* To get data by listElem_t ptr */
    data_t getData( listElem_t* listElemPtr );

  /* To get ptr to prev/next list element */
    listElem_t* getPrevOrNext( listElem_t* listElemPtr, listElemSide_t side );

  /* To add new list element prev/next to choosed list element */
    listElem_t* addPrevOrNext( listElem_t* listElemPtr, listElemSide_t side,
                               data_t newElemData );
    
  /* To delete list element */
    listError_t delElem( listElem_t* listElem2DelPtr );
};