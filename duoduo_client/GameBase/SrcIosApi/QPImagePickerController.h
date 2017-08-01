//
//  QPImagePickerController.h
//  gdy_offline
//
//  Created by Chareice on 14/9/28.
//  Copyright (c) 2014年 Chareice. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface QPImagePickerController : UIImagePickerController <UIImagePickerControllerDelegate, UINavigationControllerDelegate>
+(void) selectImage;
+(void) upHeadImagic1;
+(void) upHeadImagic2;
+(UIImage*)scaleToSize:(CGSize)size withImage:(UIImage *)myImage;
@property int iUpType;
@end
