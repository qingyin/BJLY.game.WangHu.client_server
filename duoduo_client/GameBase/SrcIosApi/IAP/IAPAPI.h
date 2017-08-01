//
//  IAPAPI.h
//  IAPtest
//
//  Created by macbook110 on 13-8-6.
//  Copyright (c) 2013年 macbook110. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import <StoreKit/StoreKit.h>

enum{
    IAP0p99=12,
    IAP1p99,
    IAP4p99,
    IAP9p99,
    IAP24p99,
}buyCoinsTag;

//@protocol IAPAPIDelegate <NSObject>
//@optional
///**
// @brief 开始请求商品数据信息
// */
//- (void)startToRequestProductData;
//
///**
// @brief 请求商品信息返回结果
// */
//
//@end

@interface IAPAPI : NSObject<SKProductsRequestDelegate,SKPaymentTransactionObserver,UIAlertViewDelegate>
{
    NSString * buyType;
    bool ifgetAllProductData;
    UIAlertView *alerViewsuccese;
    UIAlertView *alerViewfaild;
}
//@property (nonatomic ,assign)id<IAPAPIDelegate>delegate;
//+(CCScene *) scene;
+(IAPAPI *)getShareIAPAPI;

- (void) requestProUpgradeProductData;
-(void)RequestProductData;
-(void)RequestAllProductData;
-(bool)CanMakePay;
-(void)buy:(NSString*)type;
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
-(void) PurchasedTransaction: (SKPaymentTransaction *)transaction;
- (void) completeTransaction: (SKPaymentTransaction *)transaction;
- (void) failedTransaction: (SKPaymentTransaction *)transaction;
-(void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction;
-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error;
- (void) restoreTransaction: (SKPaymentTransaction *)transaction;
-(void)provideContent:(NSString *)product;
-(void)recordTransaction:(NSString *)product;

@end
