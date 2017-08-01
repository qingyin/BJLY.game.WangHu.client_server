//
//  NSArray+mappedArrayUsingBlock.h
//  cocos2d_tests
//
//  Created by Chareice on 14/10/15.
//
//

#import <Foundation/Foundation.h>

@interface NSArray (mappedArrayUsingBlock)
- (NSArray *)mappedArrayUsingBlock:(id (^)(id object))block;
@end
