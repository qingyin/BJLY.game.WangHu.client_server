//
//  QPProductsRequestManager.m
//  cocos2d_tests
//
//  Created by Chareice on 14/10/15.
//
//

#import "QPProductsRequestManager.h"
#import "NSArray+mappedArrayUsingBlock.h"

@implementation QPProductsRequestManager{
    NSSet *itemIdentifiers;
}

+(QPProductsRequestManager *) sharedManager{
    static QPProductsRequestManager *sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedManager = [[self alloc] init];
    });
    return sharedManager;
}

-(id) init{
    self = [super init];
    if(!self) return nil;
    itemIdentifiers = [NSSet setWithArray:@[@"12000_coin",
                                            @"18000_coin",
                                            @"298000_coin",
                                            @"30000_coin",
                                            @"488000_coin",
                                            @"6000_coin",
                                            @"98000_coin",
                                            @"50000_coin"]];
    _products = [[NSArray alloc] init];
    return self;
}

-(void) startRequestProductList{
    SKProductsRequest *req = [[SKProductsRequest alloc] initWithProductIdentifiers:itemIdentifiers];
    [req setDelegate:self];
    [req start];
}

-(void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    NSArray *products = response.products;
    [self itemListInitWithProducts:products];
}

-(void) itemListInitWithProducts:(NSArray *) products{
    NSArray *productsSorted = [products sortedArrayUsingComparator:^(id a, id b) {
        NSDecimalNumber *first = [(SKProduct*)a price];
        NSDecimalNumber *second = [(SKProduct*)b price];
        return [first compare:second];
    }];
    
    [self setProducts:productsSorted];

    NSArray *dictProducts = [productsSorted mappedArrayUsingBlock:^NSDictionary*(SKProduct *product){
        NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
        [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [numberFormatter setLocale:product.priceLocale];
        NSString *formattedString = [numberFormatter stringFromNumber:product.price];
        
        return [self makeProduct:product.localizedTitle price:formattedString identifier:product.productIdentifier];
    }];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:@"productsInited" object:dictProducts];
}

-(NSDictionary *) makeProduct:(NSString *) name price:(NSString *) price identifier:(NSString *) identifier{
    return @{@"product_name" : name, @"price": price, @"identifier" : identifier};
}

@end
