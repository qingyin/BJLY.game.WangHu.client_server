//
//  QPProductsRequestObserver.cpp
//  cocos2d_tests
//
//  Created by Chareice on 14/10/15.
//
//

#include "QPProductsRequestObserver.h"
#include "QPProductsRequestManager.h"

QPProductsRequestObserver::QPProductsRequestObserver(){
    
}

QPProductsRequestObserver::~QPProductsRequestObserver(){
}

void QPProductsRequestObserver::setDelegate(QPProductsRequestObserverDelegate *delegate){
    _delegate = delegate;
}

void QPProductsRequestObserver::start(){
    [[QPProductsRequestManager sharedManager] startRequestProductList];
    [[NSNotificationCenter defaultCenter] addObserverForName:@"productsInited" object:nil queue:nil usingBlock:^(NSNotification *note){
        GoldMoneyList productList;
        
        NSArray *products = [note object];
        for(NSDictionary *info in products){
            NSString *product_name = info[@"product_name"];
            NSString *price = info[@"price"];
            NSString *identifier = info[@"identifier"];
            
            GoldMoneyStruct *product = new GoldMoneyStruct(*new std::string([product_name UTF8String]), *new std::string([price UTF8String]), *new std::string([identifier UTF8String]));
            productList.push_back(product);
        }
        if([products count] > 0){
            _delegate->productInited(productList);
        }
    }];
}

void QPProductsRequestObserver::buyItem(int index){
    SKProduct *product = [[QPProductsRequestManager sharedManager] products][index];
    SKPayment *payment = [SKPayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}
