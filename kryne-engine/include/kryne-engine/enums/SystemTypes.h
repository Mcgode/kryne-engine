/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once

enum SystemTypes {

    /// Systems that will be run at the start of the loop.
    LoopStart = 0,

    /// Systems that support the direct game logic (like running the #LogicComponent instances).
    GameLogic,

    /// Systems that must be run after the game logic, but before the pre-rendering
    PostLogic,

    /// Systems that can must be run after the logic, and before the rendering.
    PreRendering,

    /// Systems that will be run after the rendering, at the end of the loop.
    PostRendering,

    /// The number of enum entries
    COUNT,

};