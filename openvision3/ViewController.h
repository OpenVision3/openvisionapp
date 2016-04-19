//
//  ViewController.h
//  openvision3
//
//  Created by LIN on 2016-04-16.
//  Copyright © 2016 ZHIMAABC. All rights reserved.
//

#import <UIKit/UIKit.h>

#ifdef __cplusplus
#import <opencv2/videoio/cap_ios.h>
#include <opencv2/imgproc.hpp>
#endif

@interface ViewController : UIViewController<CvVideoCameraDelegate>

@property (nonatomic, retain) CvVideoCamera* videoCamera;
- (IBAction)switchCamera:(id)sender;

@end

