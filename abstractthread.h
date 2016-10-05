#ifndef ABSTRACTTHREAD
#define ABSTRACTTHREAD

class AbstractThread {

protected:

    static const int            MAX = 1000;
    pthread_t                   threads[MAX];
    int                         threadId[MAX];

    static void *               Callback          ( void * arg );
    virtual void *              ThreadWorker      ( void * arg ) = 0;
    virtual AbstractThread *    Copy              ( void ) const = 0;

public:

    int                         threadCounter;
    
                                AbstractThread    ( void ) { threadCounter = 0; }
    virtual                     ~AbstractThread    () {}
    AbstractThread &            operator=         ( const AbstractThread & at );
    void                        Start             ( int n = 1 );
    void **                     Wait              ( void );

};

#endif
