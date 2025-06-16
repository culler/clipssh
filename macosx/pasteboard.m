#include <tcl.h>
#include <AppKit/NSPasteboard.h>

@interface pasteboardOwner: NSObject <NSPasteboardTypeOwner>

@property(retain) NSString *clip;

@end

@implementation pasteboardOwner

// Clang claims that the NSPasteboard is not an NSObject.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wobjc-method-access"

- (void) pasteboard: (NSPasteboard *) sender
 provideDataForType: (NSString *) type
{
    // Provide our clip to the pasteboard, as requested.
    [sender setString:self.clip forType:type];
    // Clear our clip.
    [self setClip: nil];
    // Clear the pasteboard after a short delay.
    [sender  performSelector: @selector(clearContents) 
		  withObject: nil 
		  afterDelay: 0.1
     ];
}

#pragma clang diagnostic pop

@end

static pasteboardOwner *owner = nil;

void initPasteboard() {
    NSPasteboard *pb = [NSPasteboard generalPasteboard];
    if (owner == nil) {
	owner = [[pasteboardOwner alloc] init];
	[owner retain];
	[pb declareTypes:[NSArray arrayWithObject:NSPasteboardTypeString]
		   owner:nil];
	[pb setString:[[NSString alloc] initWithUTF8String:""]
	      forType:NSPasteboardTypeString];
    }
}

void addTransientClip(const char *clip) {    
    NSPasteboard *pb = [NSPasteboard generalPasteboard];
    [owner setClip: [[NSString alloc] initWithUTF8String:clip]];
    [pb addTypes:[NSArray arrayWithObject:NSPasteboardTypeString]
	   owner:owner];
}

/*
 * Local Variables:
 * mode: objc
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
