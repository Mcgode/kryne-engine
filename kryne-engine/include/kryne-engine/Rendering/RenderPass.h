/**
 * @file
 * @author Max Godefroy
 * @date 06/03/2021.
 */

#pragma once


#include <iostream>
#include <utility>


using namespace std;


class RenderPass {

public:

    enum Order {

        /**
         * If render mode is deferred rendering, will run this render pass before the deferred shading pass.
         * If render pass mode is forward rendering, will run it before the other post processing passes.
         */
        BeforeDeferred,

        PostProcess,

    };


public:

    [[nodiscard]] Order getOrder() const { return order; }

    [[nodiscard]] bool hasCustomCulling() const { return customCull; }

    [[nodiscard]] bool needsRenderGeometry() const { return renderGeometry; }

protected:

    RenderPass(Order order, string name, bool customCull, bool renderGeometry);

protected:

    Order order;

    string name;

    bool customCull;

    bool renderGeometry;

};


