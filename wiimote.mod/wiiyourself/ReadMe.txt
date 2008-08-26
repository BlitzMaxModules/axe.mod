__________________________________________________________________

     - WiiYourself! - native C++ Wiimote library  v0.99b.
       (c) gl.tter 2007 - http://wiiyourself.gl.tter.org
__________________________________________________________________

This marks the release of my totally-free & fully-featured
Wiimote native C++ library for (currently) Windows.

Originally ported from Brian Peek's 'Managed Wiimote Library'
(http://blogs.msdn.com/coding4fun/archive/2007/03/14/1879033.aspx)
I've since rewritten and extended it considerably.

There's no documentation currently - check Brian's article for a
good overview and general 'Wiimote with Windows' info - but the
source code has extensive comments, and the demo app should help
you make sense of it all (it's not that hard).  Any questions,
join my mailing list (below).

Check the License.txt file for the (few) conditions of use.

Notes:

  - VC8 C++ projects are included.
  - The WinDDK is required to build (for HID API), get it here:
    http://www.microsoft.com/whdc/devtools/ddk/default.mspx
     then add its 'inc/wxp' dir to your include paths.
  - The library is Unicode-capable (the 'U' build options).
  - You may need to adapt the 'Runtime Library' settings 
    (Project Properties -> C/C++ -> Code Generation) to match
    your application to avoid link errors.
    
Wiimote installation notes:
  
  The Wiimote needs to be 'paired' (Bluetooth connected) with the
   PC before you can install/use it.  Pressing 1 & 2 simultaneously
   puts it into 'discoverable' mode for a few seconds (LEDs will
   flash - the number of LEDs reflects the battery level).
   
  It will be detected as 'Nintendo RVL-CNT-01'.
     
  Stack-specific instructions:

  - Windows' built-in Bluetooth stack:

    1) open up the Bluetooth control panel.
    2) press _and hold_ 1&2 on the wiimote (LEDs flash) until the
       installation is complete (otherwise the wiimote usually times
       out half-way through the procedure, and although it may seem
       to have installed is never 'connected' and doesn't work).
    3) add a new device - it should find it. don't use a password.
    4) when the installation is fully complete, let go of 1&2. The
       Bluetooth panel should now show it 'connected'.
    
    if something goes wrong you need to uninstall it and try again.
    
    if you un-pair the wiimote later (see below), it seems you need
     to remove and install it all over again to get it to work (if you
     know a workaround, let me know).
       
  - Toshiba stack:

    straight forward, press 1 & 2 on the Wiimote (you don't need to
    hold them if you're quick) and click 'New Connection'.
    
    once found, you can (un)pair it anytime again by right-clicking its
     device icon (1 & 2 to connect as before) - or even set up a desktop
     shortcut that enters pairing mode immediately.
     
  - Other stacks
  
    similar to the above (contribute instructions?)
    
  
  - Disconnect (un-pair) to save power (any stack):

    hold the Wiimote Power button for a few seconds - the Wiimote
    automatically unpairs itself, re-enters pairing mode for a few
    seconds (flashing LEDs), then times out and (effecively) switches
    off.
         

My online ToDo list has the known issues, stuff I'm planning
to do and stuff I'd like help on:
http://wiiyourself.gl.tter.org/todo.htm

Sign up to the mailing list there to stay in the loop, give
feedback, exchange ideas or get involved.

** Let me know what you're using it for & I'll link to
   your project.  Have fun. **
__

gl.tter  (http://gl.tter.org | glATr-i-lDOTnet)


