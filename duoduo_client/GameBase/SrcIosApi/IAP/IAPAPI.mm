//
//  IAPAPI.m
//  IAPtest
//
//  Created by macbook110 on 13-8-6.
//  Copyright (c) 2013年 macbook110. All rights reserved.
//

#import "IAPAPI.h"
#import "MBProgressHUD.h"

#include "ScenceManager.h"
#include "ShopScence.h"

@implementation IAPAPI

//@synthesize delegate;


static IAPAPI* shareIAPAPI;
+(IAPAPI *)getShareIAPAPI
{
    if (!shareIAPAPI) {
		shareIAPAPI = [[self alloc] init];
        [shareIAPAPI init];
	}
	
	return shareIAPAPI;
}

/**
 @Method 初始化，添加监听
 */
-(id)init
{
    if ((self = [super init])) {
        //---------------------
        //----监听购买结果
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        //申请购买
        /*
         enum{
         IAP0p99=10,
         IAP1p99,
         IAP4p99,
         IAP9p99,
         IAP24p99,
         }buyCoinsTag;
         */
//        [self buy:IAP0p99];
    }
    return self;
}

/**
 @method 请求得到所有的商品信息
 */
-(void)getAllProductData
{
    [self showHub];//显示菊花
    if ([SKPaymentQueue canMakePayments]) {
        //[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
        [self RequestAllProductData];
        ifgetAllProductData = YES;  //设置参数，是获取全部商品的信息
        NSLog(@"允许程序内付费购买");
    }
    else
    {
        NSLog(@"不允许程序内付费购买");
        UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                            message:@"You can‘t purchase in app store（Himi说你没允许应用程序内购买）"
                                                           delegate:nil cancelButtonTitle:NSLocalizedString(@"Close（关闭）",nil) otherButtonTitles:nil];
        
        [alerView show];
        [alerView release];
        
    }
}


/**
 @method 购买商品 
 @param type 购买的类型
 */
-(void)buy:(NSString*)type
{
    [self showHub];//显示菊花

    buyType = type;
    if ([SKPaymentQueue canMakePayments]) {
        //[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
        [self RequestAllProductData];
        ifgetAllProductData = NO;
        NSLog(@"允许程序内付费购买");
    }
    else
    {
        NSLog(@"不允许程序内付费购买");
        UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                            message:@"You can‘t purchase in app store（你没允许应用程序内购买）"
                                                           delegate:nil cancelButtonTitle:NSLocalizedString(@"Close（关闭）",nil) otherButtonTitles:nil];
        
        [alerView show];
        [alerView release];
        
    }
}

/**
 @method 能够支付
 */
-(bool)CanMakePay
{
    return [SKPaymentQueue canMakePayments];
}

/**
 @method 请求得到所有的商品数据
 */
-(void)RequestAllProductData
{
    NSLog(@"---------全部产品信息------------");
    NSArray *product = nil;
    product = [[NSArray alloc] initWithObjects:buyType, nil];
    NSSet *nsset = [NSSet setWithArray:product];
    SKProductsRequest *request=[[SKProductsRequest alloc] initWithProductIdentifiers: nsset];
    request.delegate=self;
    [request start];
    [product release];
}

/**
 @method 请求得到对应的产品信息
 */
-(void)RequestProductData
{

    NSLog(@"---------请求对应的产品信息------------");
    NSArray *product = nil;

                product=[[NSArray alloc] initWithObjects:buyType,nil];
    NSSet *nsset = [NSSet setWithArray:product];
    SKProductsRequest *request=[[SKProductsRequest alloc] initWithProductIdentifiers: nsset];
    request.delegate=self;
    [request start];
    [product release];

}

//<SKProductsRequestDelegate> 请求协议
//收到的产品信息
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    
    NSLog(@"-----------收到产品反馈信息--------------");
    NSArray *myProduct = response.products;
    NSLog(@"产品Product ID:%@",response.invalidProductIdentifiers);
    NSLog(@"产品付费数量: %d", [myProduct count]);
    
    if ([myProduct count]==0) {
        [self hideHub];//隐藏菊花
        alerViewfaild =  [[UIAlertView alloc] initWithTitle:@"警告"
                                                            message:@"没有获取到商品"
                                                           delegate:nil cancelButtonTitle:NSLocalizedString(@"Close（关闭）",nil) otherButtonTitles:nil];
        [alerViewfaild setDelegate:self];
        [alerViewfaild show];
        [alerViewfaild release];
        
        return;
    }

    // populate UI
    for(SKProduct *product in myProduct){
        NSLog(@"product info");
        NSLog(@"SKProduct 描述信息%@", [product description]);
        NSLog(@"产品标题 %@" , product.localizedTitle);
        NSLog(@"产品描述信息: %@" , product.localizedDescription);
        NSLog(@"价格: %@" , product.price);
        NSLog(@"Product id: %@" , product.productIdentifier);
        if (ifgetAllProductData) {
            NSLog(@"获取全部商品的信息");
        }
    }
    
    if (ifgetAllProductData) {
        NSLog(@"不购买，因为是获取商品信息");
    }
    else
    {
        SKPayment *payment = nil;
        payment = [SKPayment paymentWithProduct:[myProduct objectAtIndex:0]];
        NSLog(@"---------发送购买请求------------");
        [[SKPaymentQueue defaultQueue] addPayment:payment];
        [request autorelease];
    }
}
- (void)requestProUpgradeProductData
{
    NSLog(@"------请求升级数据---------");
    NSSet *productIdentifiers = [NSSet setWithObject:@"com.productid"];
    SKProductsRequest* productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    productsRequest.delegate = self;
    [productsRequest start];
    
}
//弹出错误信息
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error{
    NSLog(@"-------弹出错误信息----------");

    UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert",NULL) message:[error localizedDescription]
                                                       delegate:nil cancelButtonTitle:NSLocalizedString(@"Close",nil) otherButtonTitles:nil];
    [alerView show];
    [alerView release];
}

-(void) requestDidFinish:(SKRequest *)request
{
    NSLog(@"----------反馈信息结束--------------");
    
}

-(void) PurchasedTransaction: (SKPaymentTransaction *)transaction{
    
    NSLog(@"-----PurchasedTransaction----");
    NSArray *transactions =[[NSArray alloc] initWithObjects:transaction, nil];
    [self paymentQueue:[SKPaymentQueue defaultQueue] updatedTransactions:transactions];
    [transactions release];
}

//<SKPaymentTransactionObserver> 千万不要忘记绑定，代码如下：
//----监听购买结果
//[[SKPaymentQueue defaultQueue] addTransactionObserver:self];

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions//交易结果
{
    NSLog(@"-----paymentQueue--------");

    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased://交易完成
            {
                [self completeTransaction:transaction];
                NSLog(@"-----交易完成 --------");
                [self hideHub];//隐藏菊花
                
                NSData *receipt = transaction.transactionReceipt;
                if (!receipt){ return ;}
                std::string kTxt =[[receipt base64EncodedStringWithOptions:0] UTF8String];
                ShopScence::Instance().callHTTPValidation(kTxt);
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
            }
                break;
            case SKPaymentTransactionStateFailed://交易失败
            {
                [self failedTransaction:transaction];
                NSLog(@"-----交易失败 --------");
                [self hideHub];//隐藏菊花
                alerViewfaild =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                                     message:@"购买失败，请重新尝试购买"
                                                                    delegate:nil cancelButtonTitle:NSLocalizedString(@"Close（关闭）",nil) otherButtonTitles:nil];
                [alerViewfaild setDelegate:self];
                [alerViewfaild show];
                [alerViewfaild release];
            }
                break;
            case SKPaymentTransactionStateRestored://已经购买过该商品
            {
                [self restoreTransaction:transaction];
                NSLog(@"-----已经购买过该商品 --------");
            case SKPaymentTransactionStatePurchasing:      //商品添加进列表
                NSLog(@"-----商品添加进列表 --------");
            }
                break;
            default:
                break;
        }
    }
}
- (void) completeTransaction: (SKPaymentTransaction *)transaction

{
    NSLog(@"-----completeTransaction--------");
    // Your application should implement these two methods.
    NSString *product = transaction.payment.productIdentifier;
    if ([product length] > 0) {
        
        NSArray *tt = [product componentsSeparatedByString:@"."];
        NSString *bookid = [tt lastObject];
        if ([bookid length] > 0) {
            [self recordTransaction:bookid];
            [self provideContent:bookid];
        }
    }
    
    // Remove the transaction from the payment queue.
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
}

//记录交易
-(void)recordTransaction:(NSString *)product{
    NSLog(@"-----记录交易--------");
}

//处理下载内容
-(void)provideContent:(NSString *)product{
    NSLog(@"-----下载--------");
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction{
    NSLog(@"失败");
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
    
}
-(void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction{
    
}

- (void) restoreTransaction: (SKPaymentTransaction *)transaction

{
    NSLog(@" 交易恢复处理");
    
}

-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error{
    NSLog(@"paymentQueue=%@",paymentQueue);
    NSLog(@"-------paymentQueue----");
}

#pragma mark -UIAlertViewDelegate-
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (alertView==alerViewsuccese) {
    }
    if (alertView==alerViewfaild) {
        
    }
}


#pragma mark connection delegate
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSLog(@"%@",  [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease]);
}
- (void)connectionDidFinishLoading:(NSURLConnection *)connection{
    
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response{
    switch([(NSHTTPURLResponse *)response statusCode]) {
        case 200:
        case 206:
            break;
        case 304:
            break;
        case 400:
            break;
        case 404:
            break;
        case 416:
            break;
        case 403:
            break;
        case 401:
        case 500:
            break;
        default:
            break;
    }
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    NSLog(@"test");
}
-(void)showHub
{
    ScenceManager::Instance().ShowWait(true,"正在处理，请稍后。。。");
    
}
-(void)hideHub
{
    ScenceManager::Instance().ShowWait(false,"");
}

-(void)dealloc
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];//解除监听
    [super dealloc];
}


@end
