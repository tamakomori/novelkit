NovelKit
========

`NovelKit` is a game scripting environment for visual novels, designed
as a highly portable game engine that runs seamlessly on mobile,
desktop, and console platforms. It provides a flexible and efficient
development framework, enabling creators to build immersive
storytelling experiences with ease. With a strong focus on portability
and performance, NovelKit ensures a smooth, consistent experience
across various devices and operating systems.

Our core technologies, `MediaKit` and `Linguine`, enhance game
development efficiency, flexibility, and cross-platform compatibility.

`MediaKit` is a modern game foundation library that boosts our
engine's portability, allowing games to run effortlessly across
multiple platforms with minimal modifications.

`Linguine` is a versatile scripting language designed for both speed
and efficiency, enabling fast, seamless deployment to static native
code. It leverages a JIT compiler to accelerate iteration times,
allowing developers to prototype, test, and refine projects rapidly.

Together, MediaKit and Linguine provide a strong foundation for
building high-performance, portable games, equipping developers with
the tools to create optimized and scalable gaming experiences across
multiple platforms.


## Overview

Games built with NovelKit consist of three layers; scenario,
executive, and API.

```
+----------------------------+
|Scenario (Tagged Texts)     |
+----------------------------+
|Executive (Linguine Scripts)|
+----------------------------+
|API (C Program)             |
+----------------------------+
```

### Scenario

The upper layer, known as the scenario, consists of plain text files
with a tagged structure. A scenario file defines game directives,
functioning as a simple list of commands rather than a traditional
program.

Within a scenario file, users can call functions defined in the
executive using tag names, i.e., commands are defined as functions in
the executive.

### Executive

The middle layer, known as the executive, is composed of Linguine
scripts and serves as the core game system. NovelKit comes with a
default executive that users can utilize as is. However, the executive
also has the flexibility to be modified to better suit users' games.

A game built with NovelKit starts from a Linguine script named
`game.ls`. By default, `game.ls` creates a 1280x720 window and loads
the first scenario file, `chapter1.txt`. Users can customize `game.ls`
to implement additional initialization steps and define custom
functions to be called from scenario files.

### API

The lower layer, known as the API, is written in C and provides
functions that can be called from the executive. While users have the
option to modify the API by writing C code, this is generally not
required in most cases.


## List of Tag Commands

* Text
    * [@text - Show Text](#text)
    * [@select - Show Options)(#select)
* Graphic
    * [@enter - Enter Character](#enter)
    * [@leave - Leave Character](#leave)
    * [@sprite - Change Sprite](#sprite)
    * [@run - Apply Sprite Change](#run)
* Audio
    * [@music - Play Music](#music)
    * [@sound - Play Sound](#sound)
    * [@volume - Change Volume](#volume)
* Movie
    * [@movie - Play Movie](#movie)
* Wait
    * [@click - Wait For Click](#click)
    * [@time - Wait For Seconds](#time)
* Branch
    * [@label](#tag_label)
    * [@setvar - Set Variable](#setvar)
    * [@jump - Jump To Label](#jump)
    * [@return - Return From Macro](#return)

### @text

Show text on a layer.

### @select

Show options and jump to a label corresponding to a selected option.

### @enter

Let a new character enter the stage.

### @leave

Let a character leave the stage.

### @sprite

Change properties of a sprite.

### @run

Do sprite animation.

### @music

Play a background music.

### @sound

Play a sound effect.

### @volume

Change a sound volume.

### @movie

Play a movie.

### @click

Wait for a click.

### @time

Wait for specified seconds.

### @label

Define a jump target.

### @setvar

Assign a value to an achievement or flag variable.

### @jump

Jump to a label.

- Load a scenario file, if specified.
- Call a label instead of jump, if specified.

### @return

Return from a procedure.


## List of NovelKit API

### Save Data Management API

|Name                              |Description                                             |
|----------------------------------|--------------------------------------------------------|
|NovelKit.addAchievement()         |Adds an achievement variable with a name.               |
|NovelKit.setAchievement()         |Sets a value of an achievement variable.                |
|NovelKit.loadAchievementFile()    |Loads an achievement save file.                         |
|NovelKit.addFlag()                |Adds a flag variable with a name.                       |
|NovelKit.setFlag()                |Sets a value of an flag variable.                       |
|NovelKit.saveFlagFile()           |Saves flags to a flag file.                             |

### Scenario Management API

|Name                              |Description                                             |
|----------------------------------|--------------------------------------------------------|
|NovelKit.moveToScenarioFile()     |Loads a scenario file.                                  |
