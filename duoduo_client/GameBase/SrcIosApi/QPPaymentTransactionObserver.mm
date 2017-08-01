//
//  QPPaymentTransactionObserver.m
//  gdy_offline
//
//  Created by Chareice on 9/22/14.
//  Copyright (c) 2014 Chareice. All rights reserved.
//

#import "QPPaymentTransactionObserver.h"

#include "ScenceManager.h"
#include "ShopScence.h"

@implementation QPPaymentTransactionObserver

+(QPPaymentTransactionObserver *) sharedObserver{
    static QPPaymentTransactionObserver *sharedObserver = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedObserver = [[self alloc] init];
    });
    return sharedObserver;
}


-(void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions{
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
                ScenceManager::Instance().ShowWait("订单验证中...");
                [self transactionSuccess:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                ScenceManager::Instance().ShowPublicNotice("无法连接到iTunes Store 交易失败");
                ScenceManager::Instance().ShowWait(false);
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                CCLOG("交易恢复");
                break;
            case SKPaymentTransactionStateDeferred:
                CCLOG("交易推迟");
                break;
            case SKPaymentTransactionStatePurchasing:
                ScenceManager::Instance().ShowWait("购买中...");
                CCLOG("交易进行中");
            default:
                CCLOG("交易无行为");
                break;
        }
    }
}

-(void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error {
    
}

-(void) transactionSuccess:(SKPaymentTransaction *)transaction{
    NSData *receipt = transaction.transactionReceipt;
    if (!receipt){ return ;}

    
    std::map<std::string, std::string> params;
    params["receipt-data"] = [[receipt base64EncodedStringWithOptions:0] UTF8String];
    ShopScence::Instance().callHTTPValidation(params);
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}
@end
