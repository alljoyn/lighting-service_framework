/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#import "LSFModel.h"
#import "LSFNoEffectDataModel.h"
#import "LSFTransitionEffectDataModel.h"
#import "LSFPulseEffectDataModel.h"
#import "LSFScene.h"

@interface LSFSceneDataModel : LSFModel

@property (nonatomic, strong) NSMutableArray *noEffects;
@property (nonatomic, strong) NSMutableArray *transitionEffects;
@property (nonatomic, strong) NSMutableArray *pulseEffects;

-(id)initWithSceneID: (NSString *)theID;
-(void)updateNoEffect: (LSFNoEffectDataModel *)elementModel;
-(void)updateTransitionEffect: (LSFTransitionEffectDataModel *)elementModel;
-(void)updatePulseEffect: (LSFPulseEffectDataModel *)elementModel;
-(BOOL)removeElement: (NSString *)elementID;
-(LSFScene *)toScene;
-(void)fromScene: (LSFScene *)scene;

@end
