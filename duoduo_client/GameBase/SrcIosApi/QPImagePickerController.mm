//
//  QPImagePickerController.m
//  gdy_offline
//
//  Created by Chareice on 14/9/28.
//  Copyright (c) 2014年 Chareice. All rights reserved.
//

#import "QPImagePickerController.h"
#import "AppController.h"
#include "PlayerInfoScence.h"
#import "cocos2d.h"

@interface QPImagePickerController ()

@end

@implementation QPImagePickerController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillDisappear:(BOOL)animated
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
}

-(BOOL)shouldAutorotate
{
    return YES;
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationMaskAll;
}

-(NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskAll;
}

+(void) selectImage{
    
    QPImagePickerController *imagePickerController = [[QPImagePickerController alloc] init];
    imagePickerController.iUpType = 0;
    [imagePickerController setDelegate:imagePickerController];
    imagePickerController.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    imagePickerController.allowsEditing = NO;
    UIWindow *window = [UIApplication sharedApplication].keyWindow;
    UIViewController *rootViewController = window.rootViewController;
    [rootViewController presentViewController:imagePickerController animated:YES completion:nil];
}

+(void) upHeadImagic1{
    QPImagePickerController *imagePickerController = [[QPImagePickerController alloc] init];
    imagePickerController.iUpType = 1;
    [imagePickerController setDelegate:imagePickerController];
    imagePickerController.sourceType = UIImagePickerControllerSourceTypeCamera;
    imagePickerController.allowsEditing = YES;
    UIWindow *window = [UIApplication sharedApplication].keyWindow;
    UIViewController *rootViewController = window.rootViewController;
    [rootViewController presentViewController:imagePickerController animated:YES completion:nil];
}

+(void) upHeadImagic2{
    QPImagePickerController *imagePickerController = [[QPImagePickerController alloc] init];
    imagePickerController.iUpType = 1;
    [imagePickerController setDelegate:imagePickerController];
    imagePickerController.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    imagePickerController.allowsEditing = YES;
    UIWindow *window = [UIApplication sharedApplication].keyWindow;
    UIViewController *rootViewController = window.rootViewController;
    [rootViewController presentViewController:imagePickerController animated:YES completion:nil];
}
-(void) imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info{
    [picker dismissViewControllerAnimated:YES completion:nil];
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
    if (_iUpType != 0) {
        UIImage *image = [info objectForKey:UIImagePickerControllerEditedImage];
        image = [QPImagePickerController scaleToSize:CGSizeMake(256, 256) withImage:image];
        [self saveImage:image withName:@"feedBackImage"];
    }
    else{
        
        NSString *mediaType = [info objectForKey:UIImagePickerControllerMediaType];
        if ([mediaType isEqualToString:@"public.image"]){
            UIImage *image = [info objectForKey:@"UIImagePickerControllerOriginalImage"];
            [self saveImage:image withName:@"feedBackImage"];
        } 
    }
}
//等比例缩放
+(UIImage*)scaleToSize:(CGSize)size withImage:(UIImage *)myImage
{
    CGFloat width = myImage.size.width;// CGImageGetWidth(myImage);
    CGFloat height = myImage.size.height;//CGImageGetHeight(myImage);
    
    float verticalRadio = size.height*1.0/height;
    float horizontalRadio = size.width*1.0/width;
    
    float radio = 1;
    if(verticalRadio>1 && horizontalRadio>1)
    {
        radio = verticalRadio > horizontalRadio ? horizontalRadio : verticalRadio;
    }
    else
    {
        radio = verticalRadio < horizontalRadio ? verticalRadio : horizontalRadio;
    }
    
    width = width*radio;
    height = height*radio;
    
    int xPos = (size.width - width)/2;
    int yPos = (size.height-height)/2;
    
    // 创建一个bitmap的context
    // 并把它设置成为当前正在使用的context
    UIGraphicsBeginImageContext(size);
    
    // 绘制改变大小的图片
    [myImage drawInRect:CGRectMake(xPos, yPos, width, height)];
    
    // 从当前context中创建一个改变大小后的图片
    UIImage* scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    
    // 使当前的context出堆栈
    UIGraphicsEndImageContext();
    
    // 返回新的改变大小后的图片
    return scaledImage;
}

-(void) saveImage:(UIImage *)currentImage withName:(NSString *)imageName{
    NSData *imageData = UIImageJPEGRepresentation(currentImage, 0.5);
    
    NSString *fullPath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents"] stringByAppendingPathComponent:imageName];
    
    [imageData writeToFile:fullPath atomically:NO];
    
    NSString *imageName2 = [NSString stringWithFormat:@"%s.png","test"];
    
    NSString *fillPath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents"] stringByAppendingPathComponent:@""];
    
    NSString *fillPath2 = [fillPath stringByAppendingPathComponent:imageName2];
    
    [[NSFileManager defaultManager] moveItemAtPath:fullPath toPath:fillPath2 error:nil];
    NSLog(@"[fillPath2 cStringUsingEncoding:NSUTF8StringEncoding]=%s",[fillPath2 cStringUsingEncoding:NSUTF8StringEncoding]);
    PlayerInfoScence::Instance().upLoad(*new std::string([fillPath2 UTF8String]));
}

-(void) imagePickerControllerDidCancel:(UIImagePickerController *)picker{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
    [picker dismissViewControllerAnimated:YES completion:nil];
}

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
}

-(void) navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
}
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
    

}


@end
