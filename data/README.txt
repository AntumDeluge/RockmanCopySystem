Rockman Copy System                                Version 0.5 (2016/11/12)
by Benoît "BenoitRen" Renard
===========================================================================


What it is
----------

Rockman Copy System is a video game engine that aims to replicate the
physics of the Rockman (Mega Man in the West) games as accurately as
possible.

It's written in C++ using SDL 1.2 and SDL 2.0.


What has been implemented
-------------------------

Rockman can run, jump, shoot, and climb ladders. When he gets damaged by
the sole enemy that is present, he recoils. If he falls into a bottomless
pit, the stage restarts.

Screen transitions work just like in the original games.


What is planned
---------------

-Improving accuracy of teleporting into the stage and of climbing the top
 of ladders. They are currently only accurate in respect to the original
 Rockman.
-Linear interpolation of the outputted image to mimick the original
 games's aspect ratio.


What you will need
------------------

A Windows PC or a PlayStation Portable.

Rockman Copy System is available in two flavors: SDL 1.2 and SDL 2.0.

The SDL 1.2 build of Rockman Copy System supports Windows versions as far
back as Windows 95. DirectX 7 or newer has to be installed.

Users of Windows XP and up should use the SDL 2.0 build as it uses
hardware acceleration. DirectX 9 or newer has to be installed.

As far as hardware requirements, anything that's at least a Pentium
should do. If you use the SDL 1.2 build and decide to increase the window
size, though, a much faster processor will be required as drawing
operations won't be hardware accelerated.

Your PlayStation Portable should have the ability to run homebrew.


Installation
------------

On Windows, unzip the entire package into the folder of your choice.

On your PlayStation Portable, create a new folder on your Memory Stick
under PSP/GAME, and unzip the entire package into it.


Configuration
-------------

Rockman Copy System is configurable by editing the .ini files with the
text editor of your choice. They follow the standard Windows INI file
format.

settings.ini is the only one you should edit at the moment, unless you'd
like to play around with all the physics options to create a custom
physics configuration. It currently has one setting: pixelMultiplier. This
number specifies what the output image's dimensions should be multiplied
by to get the desired size. In other words, this resizes the output image
using nearest neighbor interpolation. Use it if you find the default size
too small.

Example: the default output image size is 256x240. If pixelMultiplier is
set to 3, the output image size becomes 768x720.

Note: the setting has no effect on the PlayStation Portable.


Controls
--------

Directional keys left and right or D-pad left and right:
  -Move Rockman left and right.

Directional keys up and down or D-pad up and down:
  -Move Rockman up and down a ladder.
  -Move cursor up and down in the menu.

Space bar or Cross button:
  -Make Rockman jump.
  -Go back to previous menu.

B key or Square button:
  -Make Rockman shoot.
  -Confirm menu selection.

Enter key or Start button
  -Pauses the game.

Tab key or Select button
  -Advances the game by one frame. Only applicable if the game is paused.


Starting screen
---------------

Upon running Rockman Copy System, you'll be presented with a couple
bare-bones menus. The menu on the left is a list of the available stages.
The menu on the right is a list of Rockman games of which physics have
been implemented.

Select a stage, then select a Rockman game to start playing.


Those numbers at the top left
-----------------------------

When you start playing, you'll notice that a bunch of numbers are
displayed on the top left at all times. These are, in order, Rockman's X
coordinate, Y coordinate, speed along the X axis, speed along the Y axis,
and a number that's a sum of flags based on what parts of Rockman touch
a ladder. Except for the latter, all of them consist of an integer value
and a fractional value, separated by a space.


Changelog
---------

-Added support for Rockman 6.
-Added support for larger maps.
-Rockman will now stand instead of land when getting down from a ladder.
-Minimum Y speed is now enforced at the right moment for accuracy.
-Regression fix: Rockman would no longer animate during scrolling.
-Fix the reeling state's animation.
-Improve the reeling state and invincible sub state handling for accuracy.
 They are now accurate for each game, not just the original, save for when
 the reeling animation of Rockman 6 starts.
-The "READY" text that appears when you start a stage has a configurable
 display duration and customisable flicker interval so it can behave
 differently depending on the game.


Known issues
------------

None at the moment.


Where to get the most recent version
------------------------------------

New versions of this software will be published at www.benoitren.be.


License
-------

The source code of this software is open source. It is subject to the
terms of the Mozilla Public License, v. 2.0. You can obtain a copy of the
license at https://mozilla.org/MPL/2.0/.


Copyright
---------

Rockman, Mega Man, and all related assets are copyrighted and/or
trademarked by Capcom Co., Ltd.
