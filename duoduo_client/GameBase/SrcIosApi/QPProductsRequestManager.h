//
//  QPProductsRequestManager.h
//  cocos2d_tests
//
//  Created by Chareice on 14/10/15.
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface QPProductsRequestManager : NSObject<SKProductsRequestDelegate>

+(QPProductsRequestManager *) sharedManager;
-(void) startRequestProductList;
@property (nonatomic, strong) NSArray *products;

@end
