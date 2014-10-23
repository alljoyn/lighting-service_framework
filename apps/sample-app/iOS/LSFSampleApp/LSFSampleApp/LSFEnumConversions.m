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
    
    switch (whichMake)
    {
        case MAKE_INVALID:
            result = @"Make Invalid";
            break;
        case MAKE_LIFX:
            result = @"LIFX";
            break;
        case MAKE_OEM1:
            result = @"Qualcomm Technologies Inc.";
            break;
        case MAKE_LASTVALUE:
            result = @"Make Last Value";
            break;
        default:
            result = @"Make Unknown";
            break;
    }
    
    return result;
}

+(NSString *)convertLampModelToString: (LampModel)whichModel
{
    NSString *result = nil;
    
    switch (whichModel)
    {
        case MODEL_INVALID:
            result = @"Model Invalid";
            break;
        case MODEL_LED:
            result = @"LED";
            break;
        case MODEL_LASTVALUE:
            result = @"Model Last Value";
            break;
        default:
            result = @"Model Unknown";
            break;
    }
    
    return result;
}

+(NSString *)convertDeviceTypeToString: (DeviceType)whichDeviceType
{
    NSString *result = nil;
    
    switch (whichDeviceType)
    {
        case TYPE_INVALID:
            result = @"Type Invalid";
            break;
        case TYPE_LAMP:
            result = @"Lamp";
            break;
        case TYPE_OUTLET:
            result = @"Outlet";
            break;
        case TYPE_LUMINAIRE:
            result = @"Luminaire";
            break;
        case TYPE_SWITCH:
            result = @"Switch";
            break;
        case TYPE_LASTVALUE:
            result = @"Type Last Value";
            break;
        default:
            result = @"Type Unknown";
            break;
    }
    
    return result;
}

+(NSString *)convertLampTypeToString: (LampType)whichLampType
{
    NSString *result = nil;
    
    switch (whichLampType)
    {
        case LAMPTYPE_INVALID:
            result = @"Lamp Type Invalid";
            break;
        case LAMPTYPE_A15:
            result = @"A15";
            break;
        case LAMPTYPE_A17:
            result = @"A17";
            break;
        case LAMPTYPE_A19:
            result = @"A19";
            break;
        case LAMPTYPE_A20:
            result = @"A20";
            break;
        case LAMPTYPE_A21:
            result = @"A21";
            break;
        case LAMPTYPE_A23:
            result = @"A23";
            break;
        case LAMPTYPE_AR70:
            result = @"AR70";
            break;
        case LAMPTYPE_AR111:
            result = @"AR111";
            break;
        case LAMPTYPE_B8:
            result = @"B8";
            break;
        case LAMPTYPE_B10:
            result = @"B10";
            break;
        case LAMPTYPE_B11:
            result = @"B11";
            break;
        case LAMPTYPE_B13:
            result = @"B13";
            break;
        case LAMPTYPE_BR25:
            result = @"BR25";
            break;
        case LAMPTYPE_BR30:
            result = @"BR30";
            break;
        case LAMPTYPE_BR38:
            result = @"BR38";
            break;
        case LAMPTYPE_BR40:
            result = @"BR40";
            break;
        case LAMPTYPE_BT15:
            result = @"BT15";
            break;
        case LAMPTYPE_BT28:
            result = @"BT28";
            break;
        case LAMPTYPE_BT37:
            result = @"BT37";
            break;
        case LAMPTYPE_BT56:
            result = @"BT56";
            break;
        case LAMPTYPE_C6:
            result = @"C6";
            break;
        case LAMPTYPE_C7:
            result = @"C7";
            break;
        case LAMPTYPE_C9:
            result = @"C9";
            break;
        case LAMPTYPE_C11:
            result = @"C11";
            break;
        case LAMPTYPE_C15:
            result = @"C15";
            break;
        case LAMPTYPE_CA5:
            result = @"CA5";
            break;
        case LAMPTYPE_CA7:
            result = @"CA7";
            break;
        case LAMPTYPE_CA8:
            result = @"CA8";
            break;
        case LAMPTYPE_CA10:
            result = @"CA10";
            break;
        case LAMPTYPE_CA11:
            result = @"CA11";
            break;
        case LAMPTYPE_E17:
            result = @"E17";
            break;
        case LAMPTYPE_E18:
            result = @"E18";
            break;
        case LAMPTYPE_E23:
            result = @"E23";
            break;
        case LAMPTYPE_E25:
            result = @"E25";
            break;
        case LAMPTYPE_E37:
            result = @"E37";
            break;
        case LAMPTYPE_ED17:
            result = @"ED17";
            break;
        case LAMPTYPE_ED18:
            result = @"ED18";
            break;
        case LAMPTYPE_ED23:
            result = @"ED23";
            break;
        case LAMPTYPE_ED28:
            result = @"ED28";
            break;
        case LAMPTYPE_ED37:
            result = @"ED37";
            break;
        case LAMPTYPE_F10:
            result = @"F10";
            break;
        case LAMPTYPE_F15:
            result = @"F15";
            break;
        case LAMPTYPE_F20:
            result = @"F20";
            break;
        case LAMPTYPE_G9:
            result = @"G9";
            break;
        case LAMPTYPE_G11:
            result = @"G11";
            break;
        case LAMPTYPE_G12:
            result = @"G12";
            break;
        case LAMPTYPE_G16:
            result = @"G16";
            break;
        case LAMPTYPE_G19:
            result = @"G19";
            break;
        case LAMPTYPE_G25:
            result = @"G25";
            break;
        case LAMPTYPE_G30:
            result = @"G30";
            break;
        case LAMPTYPE_G40:
            result = @"G40";
            break;
        case LAMPTYPE_T2:
            result = @"T2";
            break;
        case LAMPTYPE_T3:
            result = @"T3";
            break;
        case LAMPTYPE_T4:
            result = @"T4";
            break;
        case LAMPTYPE_T5:
            result = @"T5";
            break;
        case LAMPTYPE_T6:
            result = @"T6";
            break;
        case LAMPTYPE_T7:
            result = @"T7";
            break;
        case LAMPTYPE_T8:
            result = @"T8";
            break;
        case LAMPTYPE_T9:
            result = @"T9";
            break;
        case LAMPTYPE_T10:
            result = @"T10";
            break;
        case LAMPTYPE_T12:
            result = @"T12";
            break;
        case LAMPTYPE_T14:
            result = @"T14";
            break;
        case LAMPTYPE_T20:
            result = @"T20";
            break;
        case LAMPTYPE_MR8:
            result = @"MR8";
            break;
        case LAMPTYPE_MR11:
            result = @"MR11";
            break;
        case LAMPTYPE_MR16:
            result = @"MR16";
            break;
        case LAMPTYPE_MR20:
            result = @"MR20";
            break;
        case LAMPTYPE_PAR14:
            result = @"PAR14";
            break;
        case LAMPTYPE_PAR16:
            result = @"PAR16";
            break;
        case LAMPTYPE_PAR20:
            result = @"PAR20";
            break;
        case LAMPTYPE_PAR30:
            result = @"PAR30";
            break;
        case LAMPTYPE_PAR36:
            result = @"PAR36";
            break;
        case LAMPTYPE_PAR38:
            result = @"PAR38";
            break;
        case LAMPTYPE_PAR46:
            result = @"PAR46";
            break;
        case LAMPTYPE_PAR56:
            result = @"PAR56";
            break;
        case LAMPTYPE_PAR64:
            result = @"PAR64";
            break;
        case LAMPTYPE_PS25:
            result = @"PS25";
            break;
        case LAMPTYPE_PS35:
            result = @"PS35";
            break;
        case LAMPTYPE_R12:
            result = @"R12";
            break;
        case LAMPTYPE_R14:
            result = @"R14";
            break;
        case LAMPTYPE_R16:
            result = @"R16";
            break;
        case LAMPTYPE_R20:
            result = @"R20";
            break;
        case LAMPTYPE_R25:
            result = @"R25";
            break;
        case LAMPTYPE_R30:
            result = @"R30";
            break;
        case LAMPTYPE_R40:
            result = @"R40";
            break;
        case LAMPTYPE_RP11:
            result = @"RP11";
            break;
        case LAMPTYPE_S6:
            result = @"S6";
            break;
        case LAMPTYPE_S8:
            result = @"S8";
            break;
        case LAMPTYPE_S11:
            result = @"S11";
            break;
        case LAMPTYPE_S14:
            result = @"S14";
            break;
        case LAMPTYPE_ST18:
            result = @"ST18";
            break;
        case LAMPTYPE_LASTVALUE:
            result = @"Lamp Type Last Value";
            break;
        default:
            result = @"Lamp Type Unknown";
            break;
    }
    
    return result;
}

+(NSString *)convertBaseTypeToString: (BaseType)whichBaseType
{
    NSString *result = nil;
    
    switch (whichBaseType)
    {
        case BASETYPE_INVALID:
            result = @"Base Type Invalid";
            break;
        case BASETYPE_E5:
            result = @"E5";
            break;
        case BASETYPE_E10:
            result = @"E10";
            break;
        case BASETYPE_E11:
            result = @"E11";
            break;
        case BASETYPE_E12:
            result = @"E12";
            break;
        case BASETYPE_E14:
            result = @"E14";
            break;
        case BASETYPE_E17:
            result = @"E17";
            break;
        case BASETYPE_E26:
            result = @"E26";
            break;
        case BASETYPE_E27:
            result = @"E27";
            break;
        case BASETYPE_E29:
            result = @"E29";
            break;
        case BASETYPE_E39:
            result = @"E39";
            break;
        case BASETYPE_LASTVALUE:
            result = @"Base Typw Last Value";
            break;
        default:
            result = @"Base Type Unknown";
            break;
    }
    
    return result;
}

@end
