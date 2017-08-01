//
//  QPPaymentTransactionObserver.h
//  gdy_offline
//
//  Created by Chareice on 9/22/14.
//  Copyright (c) 2014 Chareice. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
@interface QPPaymentTransactionObserver : NSObject <SKPaymentTransactionObserver>

+(QPPaymentTransactionObserver *) sharedObserver;

@end
