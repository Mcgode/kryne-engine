/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once

enum SystemTypes {

    /// Systems that will be run at the start of the loop.
    LoopStart,

    /// Systems that support the direct game logic (like running the #LogicComponent instances).
    GameLogic,

    /// Systems that can must be run after the logic, and before the rendering.
    PreRendering,

    /// Systems that will be run after the rendering, at the end of the loop.
    PostRendering,

};