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

#import "LSFEnumConversions.h"

@implementation LSFEnumConversions

+(NSString *)convertLampMakeToString: (LampMake)whichMake
{
    NSString *result = nil;
    
    switch (whichMake) {
        case MAKE_LIFX:
            result = @"MAKE_LIFX";
            break;
        default:
            result = @"unknown";
            break;
    }
    
    return result;
}

+(NSString *)convertLampModelToString: (LampModel)whichModel
{
    NSString *result = nil;
    
    switch (whichModel) {
        case MODEL_LED:
            result = @"MODEL_LED";
            break;
        default:
            result = @"unknown";
            break;
    }
    
    return result;
}

+(NSString *)convertDeviceTypeToString: (DeviceType)whichDeviceType
{
    NSString *result = nil;
    
    switch (whichDeviceType) {
        case TYPE_LAMP:
            result = @"TYPE_LAMP";
            break;
        case TYPE_OUTLET:
            result = @"TYPE_OUTLET";
            break;
        case TYPE_LUMINAIRE:
            result = @"TYPE_LUMINAIRE";
            break;
        case TYPE_SWITCH:
            result = @"TYPE_SWITCH";
            break;
        default:
            result = @"unknown";
            break;
    }
    
    return result;
}

+(NSString *)convertLampTypeToString: (LampType)whichLampType
{
    NSString *result = nil;
    
    switch (whichLampType) {
        case LAMPTYPE_A15:
            result = @"LAMPTYPE_A15";
            break;
        case LAMPTYPE_A17:
            result = @"LAMPTYPE_A17";
            break;
        case LAMPTYPE_A19:
            result = @"LAMPTYPE_A19";
            break;
        case LAMPTYPE_A20:
            result = @"LAMPTYPE_A20";
            break;
        case LAMPTYPE_A21:
            result = @"LAMPTYPE_A21";
            break;
        case LAMPTYPE_A23:
            result = @"LAMPTYPE_A23";
            break;
        case LAMPTYPE_AR70:
            result = @"LAMPTYPE_AR70";
            break;
        case LAMPTYPE_AR111:
            result = @"LAMPTYPE_AR111";
            break;
        case LAMPTYPE_B8:
            result = @"LAMPTYPE_B8";
            break;
        case LAMPTYPE_B10:
            result = @"LAMPTYPE_B10";
            break;
        case LAMPTYPE_B11:
            result = @"LAMPTYPE_B11";
            break;
        case LAMPTYPE_B13:
            result = @"LAMPTYPE_B13";
            break;
        case LAMPTYPE_BR25:
            result = @"LAMPTYPE_BR25";
            break;
        case LAMPTYPE_BR30:
            result = @"LAMPTYPE_BR30";
            break;
        case LAMPTYPE_BR38:
            result = @"LAMPTYPE_BR38";
            break;
        case LAMPTYPE_BR40:
            result = @"LAMPTYPE_BR40";
            break;
        case LAMPTYPE_BT15:
            result = @"LAMPTYPE_BT15";
            break;
        case LAMPTYPE_BT28:
            result = @"LAMPTYPE_BT28";
            break;
        case LAMPTYPE_BT37:
            result = @"LAMPTYPE_BT37";
            break;
        case LAMPTYPE_BT56:
            result = @"LAMPTYPE_BT56";
            break;
        case LAMPTYPE_C6:
            result = @"LAMPTYPE_C6";
            break;
        case LAMPTYPE_C7:
            result = @"LAMPTYPE_C7";
            break;
        case LAMPTYPE_C9:
            result = @"LAMPTYPE_C9";
            break;
        case LAMPTYPE_C11:
            result = @"LAMPTYPE_C11";
            break;
        case LAMPTYPE_C15:
            result = @"LAMPTYPE_C15";
            break;
        case LAMPTYPE_CA5:
            result = @"LAMPTYPE_CA5";
            break;
        case LAMPTYPE_CA7:
            result = @"LAMPTYPE_CA7";
            break;
        case LAMPTYPE_CA8:
            result = @"LAMPTYPE_CA8";
            break;
        case LAMPTYPE_CA10:
            result = @"LAMPTYPE_CA10";
            break;
        case LAMPTYPE_CA11:
            result = @"LAMPTYPE_CA11";
            break;
        case LAMPTYPE_E17:
            result = @"LAMPTYPE_E17";
            break;
        case LAMPTYPE_E18:
            result = @"LAMPTYPE_E18";
            break;
        case LAMPTYPE_E23:
            result = @"LAMPTYPE_E23";
            break;
        case LAMPTYPE_E25:
            result = @"LAMPTYPE_E25";
            break;
        case LAMPTYPE_E37:
            result = @"LAMPTYPE_E37";
            break;
        case LAMPTYPE_ED17:
            result = @"LAMPTYPE_ED17";
            break;
        case LAMPTYPE_ED18:
            result = @"LAMPTYPE_ED18";
            break;
        case LAMPTYPE_ED23:
            result = @"LAMPTYPE_ED23";
            break;
        case LAMPTYPE_ED28:
            result = @"LAMPTYPE_ED28";
            break;
        case LAMPTYPE_ED37:
            result = @"LAMPTYPE_ED37";
            break;
        case LAMPTYPE_F10:
            result = @"LAMPTYPE_F10";
            break;
        case LAMPTYPE_F15:
            result = @"LAMPTYPE_F15";
            break;
        case LAMPTYPE_F20:
            result = @"LAMPTYPE_F20";
            break;
        case LAMPTYPE_G9:
            result = @"LAMPTYPE_G9";
            break;
        case LAMPTYPE_G11:
            result = @"LAMPTYPE_G11";
            break;
        case LAMPTYPE_G12:
            result = @"LAMPTYPE_G12";
            break;
        case LAMPTYPE_G16:
            result = @"LAMPTYPE_G16";
            break;
        case LAMPTYPE_G19:
            result = @"LAMPTYPE_G19";
            break;
        case LAMPTYPE_G25:
            result = @"LAMPTYPE_G25";
            break;
        case LAMPTYPE_G30:
            result = @"LAMPTYPE_G30";
            break;
        case LAMPTYPE_G40:
            result = @"LAMPTYPE_G40";
            break;
        case LAMPTYPE_T2:
            result = @"LAMPTYPE_T2";
            break;
        case LAMPTYPE_T3:
            result = @"LAMPTYPE_T3";
            break;
        case LAMPTYPE_T4:
            result = @"LAMPTYPE_T4";
            break;
        case LAMPTYPE_T5:
            result = @"LAMPTYPE_T5";
            break;
        case LAMPTYPE_T6:
            result = @"LAMPTYPE_T6";
            break;
        case LAMPTYPE_T7:
            result = @"LAMPTYPE_T7";
            break;
        case LAMPTYPE_T8:
            result = @"LAMPTYPE_T8";
            break;
        case LAMPTYPE_T9:
            result = @"LAMPTYPE_T9";
            break;
        case LAMPTYPE_T10:
            result = @"LAMPTYPE_T10";
            break;
            
        case LAMPTYPE_T12:
            result = @"LAMPTYPE_T12";
            break;
        case LAMPTYPE_T14:
            result = @"LAMPTYPE_T14";
            break;
        case LAMPTYPE_T20:
            result = @"LAMPTYPE_T20";
            break;
        case LAMPTYPE_MR8:
            result = @"LAMPTYPE_MR8";
            break;
        case LAMPTYPE_MR11:
            result = @"LAMPTYPE_MR11";
            break;
        case LAMPTYPE_MR16:
            result = @"LAMPTYPE_MR16";
            break;
        case LAMPTYPE_MR20:
            result = @"LAMPTYPE_MR20";
            break;
        case LAMPTYPE_PAR14:
            result = @"LAMPTYPE_PAR14";
            break;
        case LAMPTYPE_PAR16:
            result = @"LAMPTYPE_PAR16";
            break;
        case LAMPTYPE_PAR20:
            result = @"LAMPTYPE_PAR20";
            break;
        case LAMPTYPE_PAR30:
            result = @"LAMPTYPE_PAR30";
            break;
        case LAMPTYPE_PAR36:
            result = @"LAMPTYPE_PAR36";
            break;
        case LAMPTYPE_PAR38:
            result = @"LAMPTYPE_PAR38";
            break;
        case LAMPTYPE_PAR46:
            result = @"LAMPTYPE_PAR46";
            break;
        case LAMPTYPE_PAR56:
            result = @"LAMPTYPE_PAR56";
            break;
        case LAMPTYPE_PAR64:
            result = @"LAMPTYPE_PAR64";
            break;
        case LAMPTYPE_PS25:
            result = @"LAMPTYPE_PS25";
            break;
        case LAMPTYPE_PS35:
            result = @"LAMPTYPE_PS35";
            break;
        case LAMPTYPE_R12:
            result = @"LAMPTYPE_R12";
            break;
        case LAMPTYPE_R14:
            result = @"LAMPTYPE_R14";
            break;
        case LAMPTYPE_R16:
            result = @"LAMPTYPE_R16";
            break;
        case LAMPTYPE_R20:
            result = @"LAMPTYPE_R20";
            break;
        case LAMPTYPE_R25:
            result = @"LAMPTYPE_R25";
            break;
        case LAMPTYPE_R30:
            result = @"LAMPTYPE_R30";
            break;
        case LAMPTYPE_R40:
            result = @"LAMPTYPE_R40";
            break;
        case LAMPTYPE_RP11:
            result = @"LAMPTYPE_RP11";
            break;
        case LAMPTYPE_S6:
            result = @"LAMPTYPE_S6";
            break;
        case LAMPTYPE_S8:
            result = @"LAMPTYPE_S8";
            break;
        case LAMPTYPE_S11:
            result = @"LAMPTYPE_S11";
            break;
        case LAMPTYPE_S14:
            result = @"LAMPTYPE_S14";
            break;
        case LAMPTYPE_ST18:
            result = @"LAMPTYPE_ST18";
            break;
        default:
            result = @"unknown";
            break;
    }
    
    return result;
}

+(NSString *)convertBaseTypeToString: (BaseType)whichBaseType
{
    NSString *result = nil;
    
    switch (whichBaseType) {
        case BASETYPE_E5:
            result = @"BASETYPE_E5";
            break;
        case BASETYPE_E10:
            result = @"BASETYPE_E10";
            break;
        case BASETYPE_E11:
            result = @"BASETYPE_E11";
            break;
        case BASETYPE_E12:
            result = @"BASETYPE_E12";
            break;
        case BASETYPE_E14:
            result = @"BASETYPE_E14";
            break;
        case BASETYPE_E17:
            result = @"BASETYPE_E17";
            break;
        case BASETYPE_E26:
            result = @"BASETYPE_E26";
            break;
        case BASETYPE_E27:
            result = @"BASETYPE_E27";
            break;
        case BASETYPE_E29:
            result = @"BASETYPE_E29";
            break;
        case BASETYPE_E39:
            result = @"BASETYPE_E39";
            break;
        default:
            result = @"unknown";
            break;
    }
    
    return result;
}

@end
