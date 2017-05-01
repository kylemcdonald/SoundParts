/* Copyright 2017 Kyle McDonald */
#pragma once

#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDManager.h>

class HIDKeyboardReceiver {
public:
    virtual void HIDKeyboardEvent(int charcode, bool press)=0;
};
HIDKeyboardReceiver* app;

void myHIDKeyboardCallback(void* context,  IOReturn result,  void* sender,  IOHIDValueRef value) {
    typedef int (*HIDCallbackFunction)(int,int);
    IOHIDElementRef elem = IOHIDValueGetElement(value);
    UInt32 usagePage = IOHIDElementGetUsagePage(elem);
    uint32_t scancode = IOHIDElementGetUsage( elem );
    uint32_t intvalue = IOHIDValueGetIntegerValue(value);
    if(scancode >= 4 && scancode <= 29) {
        int charcode = 'a' + scancode - 4;
        app->HIDKeyboardEvent(charcode, intvalue);
    }
}

CFMutableDictionaryRef myCreateDeviceMatchingDictionary( UInt32 usagePage,  UInt32 usage ) {
    CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0
                                                            , & kCFTypeDictionaryKeyCallBacks
                                                            , & kCFTypeDictionaryValueCallBacks );
    if ( ! dict )
        return NULL;
    CFNumberRef pageNumberRef = CFNumberCreate( kCFAllocatorDefault, kCFNumberIntType, & usagePage );
    if ( ! pageNumberRef ) {
        CFRelease( dict );
        return NULL;
    }
    CFDictionarySetValue( dict, CFSTR(kIOHIDDeviceUsagePageKey), pageNumberRef );
    CFRelease( pageNumberRef );
    CFNumberRef usageNumberRef = CFNumberCreate( kCFAllocatorDefault, kCFNumberIntType, & usage );
    if ( ! usageNumberRef ) {
        CFRelease( dict );
        return NULL;
    }
    CFDictionarySetValue( dict, CFSTR(kIOHIDDeviceUsageKey), usageNumberRef );
    CFRelease( usageNumberRef );
    return dict;
}

void setupHIDCallback(HIDKeyboardReceiver* receiver) {
    app = receiver;
    
    IOHIDManagerRef hidManager = IOHIDManagerCreate( kCFAllocatorDefault, kIOHIDOptionsTypeNone );
    CFArrayRef matches;
    {
        CFMutableDictionaryRef keyboard = myCreateDeviceMatchingDictionary( 0x01, 6 );
        CFMutableDictionaryRef matchesList[] = {keyboard};
        matches = CFArrayCreate( kCFAllocatorDefault, (const void **)matchesList, 1, NULL );
    }
    IOHIDManagerSetDeviceMatchingMultiple( hidManager, matches );
    IOHIDManagerRegisterInputValueCallback( hidManager, myHIDKeyboardCallback, NULL );
    IOHIDManagerScheduleWithRunLoop( hidManager, CFRunLoopGetMain(), kCFRunLoopDefaultMode );
    IOHIDManagerOpen( hidManager, kIOHIDOptionsTypeNone );
}
