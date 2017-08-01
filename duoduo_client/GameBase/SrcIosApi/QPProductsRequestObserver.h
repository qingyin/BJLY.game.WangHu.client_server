//
//  QPProductsRequestObserver.h
//  cocos2d_tests
//
//  Created by Chareice on 14/10/15.
//
//

#ifndef __cocos2d_tests__QPProductsRequestObserver__
#define __cocos2d_tests__QPProductsRequestObserver__

#include "GoldMoneyStruct.h"

class QPProductsRequestObserverDelegate{
public:
    QPProductsRequestObserverDelegate(){};
    virtual ~QPProductsRequestObserverDelegate(){};
    virtual void productInited(GoldMoneyList list){}
};

class QPProductsRequestObserver{
    QPProductsRequestObserverDelegate *  _delegate;
public:
    QPProductsRequestObserver();
    ~QPProductsRequestObserver(void);
    void setDelegate(QPProductsRequestObserverDelegate *delegate);
    void start();
    void buyItem(int index);
};
#endif /* defined(__cocos2d_tests__QPProductsRequestObserver__) */
