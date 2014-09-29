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

#import "LSFSceneDataModel.h"
#import "LSFScene.h"
#import "LSFStateTransitionEffect.h"
#import "LSFStatePulseEffect.h"

@interface LSFSceneDataModel()

@end

@implementation LSFSceneDataModel

@synthesize noEffects = _noEffects;
@synthesize transitionEffects = _transitionEffects;
@synthesize pulseEffects = _pulseEffects;

-(id)init
{
    self = [self initWithSceneID: @""];

    if (self)
    {
        // Empty Constructor
    }

    return self;
}

-(id)initWithSceneID:(NSString *)theID
{
    self = [super initWithID: theID andName: @"[Loading scene info...]"];

    if (self)
    {
        self.noEffects = [[NSMutableArray alloc] init];
        self.transitionEffects = [[NSMutableArray alloc] init];
        self.pulseEffects = [[NSMutableArray alloc] init];
    }

    return self;
}

-(void)updateNoEffect: (LSFNoEffectDataModel *)elementModel
{
    BOOL updated = NO;

    for (int i = 0; !updated && i < self.noEffects.count; i++)
    {
        LSFNoEffectDataModel *nedm = [self.noEffects objectAtIndex: i];

        if ([elementModel.theID isEqualToString: nedm.theID])
        {
            [self.noEffects replaceObjectAtIndex: i withObject: elementModel];
            updated = YES;
        }
    }

    if (!updated)
    {
        [self.noEffects addObject: elementModel];
    }
}

-(void)updateTransitionEffect: (LSFTransitionEffectDataModel *)elementModel
{
    BOOL updated = NO;

    for (int i = 0; !updated && i < self.transitionEffects.count; i++)
    {
        LSFTransitionEffectDataModel *tedm = [self.transitionEffects objectAtIndex: i];

        if ([elementModel.theID isEqualToString: tedm.theID])
        {
            [self.transitionEffects replaceObjectAtIndex: i withObject: elementModel];
            updated = YES;
        }
    }

    if (!updated)
    {
        [self.transitionEffects addObject: elementModel];
    }
}

-(void)updatePulseEffect: (LSFPulseEffectDataModel *)elementModel
{
    BOOL updated = NO;

    for (int i = 0; !updated && i < self.pulseEffects.count; i++)
    {
        LSFPulseEffectDataModel *pedm = [self.pulseEffects objectAtIndex: i];

        if ([elementModel.theID isEqualToString: pedm.theID])
        {
            [self.pulseEffects replaceObjectAtIndex: i withObject: elementModel];
            updated = YES;
        }
    }

    if (!updated)
    {
        [self.pulseEffects addObject: elementModel];
    }
}

-(BOOL)removeElement: (NSString *)elementID
{
    BOOL removed = NO;

    //Check NoEffects Array
    for (int i = 0; !removed && i < self.noEffects.count; i++)
    {
        LSFNoEffectDataModel *nedm = [self.noEffects objectAtIndex: i];

        if ([elementID isEqualToString: nedm.theID])
        {
            [self.noEffects removeObjectAtIndex: i];
            removed = YES;
        }
    }

    //Check TransitionEffects Array
    for (int i = 0; !removed && i < self.transitionEffects.count; i++)
    {
        LSFTransitionEffectDataModel *tedm = [self.transitionEffects objectAtIndex: i];

        if ([elementID isEqualToString: tedm.theID])
        {
            [self.transitionEffects removeObjectAtIndex: i];
            removed = YES;
        }
    }

    //Check PulseEffects Array
    for (int i = 0; !removed && i < self.pulseEffects.count; i++)
    {
        LSFPulseEffectDataModel *pedm = [self.pulseEffects objectAtIndex: i];

        if ([elementID isEqualToString: pedm.theID])
        {
            [self.pulseEffects removeObjectAtIndex: i];
            removed = YES;
        }
    }

    return removed;
}

-(LSFScene *)toScene
{
    // TODO - Handle preset effects properly

    LSFScene *scene = [[LSFScene alloc] init];

    NSMutableArray *stateTransitionEffects = [[NSMutableArray alloc] init];
    NSMutableArray *statePulseEffects = [[NSMutableArray alloc] init];

    //Add no effects
    for (int i = 0; i < self.noEffects.count; i++)
    {
        LSFNoEffectDataModel *nedm = [self.noEffects objectAtIndex: i];
        LSFStateTransitionEffect *ste = [[LSFStateTransitionEffect alloc] initWithLampIDs: nedm.members.lamps lampGroupIDs: nedm.members.lampGroups lampState: nedm.state andTransitionPeriod: 0];
        [stateTransitionEffects addObject: ste];
    }

    //Add transition effects
    for (int i = 0; i < self.transitionEffects.count; i++)
    {
        LSFTransitionEffectDataModel *tedm = [self.transitionEffects objectAtIndex: i];
        LSFStateTransitionEffect *ste = [[LSFStateTransitionEffect alloc] initWithLampIDs: tedm.members.lamps lampGroupIDs: tedm.members.lampGroups lampState: tedm.state andTransitionPeriod: tedm.duration];
        [stateTransitionEffects addObject: ste];
    }

    //Add pulse effects
    for (int i = 0; i < self.pulseEffects.count; i++)
    {
        LSFPulseEffectDataModel *pedm = [self.pulseEffects objectAtIndex: i];
        LSFStatePulseEffect *spe = [[LSFStatePulseEffect alloc] initWithLampIDs: pedm.members.lamps lampGroupIDs: pedm.members.lampGroups fromState: pedm.state toState: pedm.endState period: pedm.period duration: pedm.duration andNumPulses: pedm.numPulses];
        [statePulseEffects addObject: spe];
    }

    //Set Scene
    scene.transitionToStateComponent = stateTransitionEffects;
    scene.pulseWithStateComponent = statePulseEffects;

    return scene;
}

-(void)fromScene: (LSFScene *)scene
{
    NSArray *stateTransitionEffects = scene.transitionToStateComponent;
    NSArray *statePulseEffects = scene.pulseWithStateComponent;

    [self.noEffects removeAllObjects];
    [self.transitionEffects removeAllObjects];
    [self.pulseEffects removeAllObjects];

    for (int i = 0; i < stateTransitionEffects.count; i++)
    {
        LSFStateTransitionEffect *ste = [stateTransitionEffects objectAtIndex: i];

        if (ste.transitionPeriod == 0)
        {
            LSFNoEffectDataModel *nedm = [[LSFNoEffectDataModel alloc] init];
            nedm.members.lamps = ste.lamps;
            nedm.members.lampGroups = ste.lampGroups;
            nedm.state = ste.lampState;

            [self.noEffects addObject: nedm];
        }
        else
        {
            LSFTransitionEffectDataModel *tedm = [[LSFTransitionEffectDataModel alloc] init];
            tedm.members.lamps = ste.lamps;
            tedm.members.lampGroups = ste.lampGroups;
            tedm.state = ste.lampState;
            tedm.duration = ste.transitionPeriod;

            [self.transitionEffects addObject: tedm];
        }
    }

    for (int i = 0; i < statePulseEffects.count; i++)
    {
        LSFStatePulseEffect *spe = [statePulseEffects objectAtIndex: i];
        LSFPulseEffectDataModel *pedm = [[LSFPulseEffectDataModel alloc] init];

        pedm.members.lamps = spe.lamps;
        pedm.members.lampGroups = spe.lampGroups;
        pedm.state = spe.fromLampState;
        pedm.endState = spe.toLampState;
        pedm.period = spe.period;
        pedm.duration = spe.duration;
        pedm.numPulses = spe.numPulses;

        [self.pulseEffects addObject: pedm];
    }
}

@end
