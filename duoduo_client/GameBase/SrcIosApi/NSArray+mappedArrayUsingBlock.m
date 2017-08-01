//
//  NSArray+mappedArrayUsingBlock.m
//  cocos2d_tests
//
//  Created by Chareice on 14/10/15.
//
//

#import "NSArray+mappedArrayUsingBlock.h"

@implementation NSArray (mappedArrayUsingBlock)
- (NSArray *)mappedArrayUsingBlock:(id (^)(id object))block
{
    if (block)
    {
        NSMutableArray *array = [NSMutableArray arrayWithCapacity:[self count]];
        for (id object in self)
        {
            id replacement = block(object);
            if (replacement) [array addObject:replacement];
        }
        return array;
    }
    return [NSArray arrayWithArray:self];
}
@end
