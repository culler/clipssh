#include <tcl.h>
#include <tk.h>
#import <AppKit/NSPasteboard.h>
#import <CoreFoundation/CoreFoundation.h>
#import <Cocoa/Cocoa.h>

@interface pasteboardOwner: NSObject <NSPasteboardTypeOwner>

@property(retain) NSString *clip;
@property NSTimeInterval delay;

@end

@implementation pasteboardOwner

// Clang claims that the NSPasteboard is not an NSObject.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wobjc-method-access"

// This method is called by the general NSPasteboard under the
// following conditions:
// * this object owns the NSPasteboardTypeString type;
// * the NSPasteboardTypeString contents are empty;
// * the general NSPasteboard needs a value for the string type,
//   for example because the pasteboard is being read due to a
//   paste event.

- (void) pasteboard: (NSPasteboard *) sender
 provideDataForType: (NSString *) type
{
    // A paste is underway, so we provide our clip to the pasteboard.
    [sender setString:self.clip forType:type];
    //Clear our clip.
    [self setClip: nil];
    // Clear the pasteboard too, after a short delay.
    [sender  performSelector: @selector(clearContents) 
		  withObject: nil 
		  afterDelay: 0.1
     ];
    Tk_SendVirtualEvent(0, "ClipsshPaste", NULL);
}

// Our goal is to write a transient value to the pasteboard, which should
// persist only until the next paste, without alerting any clipboard manager
// to read the value from the pasteboard before it disappears.
//
// Typically, a clipboard manager polls the general NSPasteboard at a fixed
// time interval (usually 500ms).  It checks whether its saved value of the
// changeCount property is different from the current value.  If so, the
// string value has changed and the clipboard manager will read the current
// value and save it in its cache.
//
// Clearing the pasteboard contents or writing a value to the pasteboard
// causes the changeCount to be incremented.  However, the NSPasteboard
// class provides a method [NSPasteboard addTypes: owner:] which allows
// an object which supports the NSPasteboardTypeOwner protocol to become
// the "owner" of a specified type.  Becoming the owner means that the
// owner object promises to provide a value of the specified type when
// needed by the pasteboard. The pasteboard calls the owner's method
// [NSPasteboardTypeOwner pasteboard: provideDataForType] when it
// needs the value.  It is important that assigning an owner to a type
// does *not* cause the changeCount to be incremented.

- (void) becomeOwner
{
    NSPasteboard *pb = [NSPasteboard generalPasteboard];
    // This does not increment the changeCount!
    [pb addTypes:[NSArray arrayWithObject:NSPasteboardTypeString]
	   owner:self];
}

#pragma clang diagnostic pop

@end

static pasteboardOwner *owner = nil;

void initPasteboard() {
    NSPasteboard *pb = [NSPasteboard generalPasteboard];
    // Create our singleton NSPasteboardTypeOwner object.
    if (owner == nil) {
	owner = [[pasteboardOwner alloc] init];
	[owner retain];
	// This clears the pasteboard, which increments the changeCount.
	[pb declareTypes:[NSArray arrayWithObject:NSPasteboardTypeString]
		   owner:nil];
    }
}

void addTransientClip(const char *clip, NSTimeInterval delay) {    
    NSPasteboard *pb = [NSPasteboard generalPasteboard];
    [owner setClip: [[NSString alloc] initWithUTF8String:clip]];
    [owner setDelay: delay];

    // First clear the pasteboard.  (When the clipboard is not empty, the
    // pasteboard will not ask our owner object to provide its data.)  The
    // clear operation increments the changeCount, so clipboard managers which
    // poll the changeCount will also clear their cached copy of the
    // clipboard.

    [pb clearContents];

    // After a delay, to allow clipboard managers time to notice the clear
    // operation, make a promise to provide our clip when needed (i.e. on the
    // next paste).  This does not increment the changeCount, so the clipboard
    // manager will not notice that we have made the promise, nor will it know
    // that a paste has been done when it happens..

    [owner performSelector: @selector(becomeOwner) 
		withObject: nil
		afterDelay: owner.delay];
}

/*
 * Local Variables:
 * mode: objc
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
