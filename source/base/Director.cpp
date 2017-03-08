#include "base/Director.h"

#include <algorithm>
#include <gl/glew.h>
#include <glfw/glfw3.h>

#include "common/NodeVisitor.h"
#include "common/Scene.h"
#include "renderer/RenderInterface.h"

namespace te
{
    Director* Director::_director = nullptr;

    Director* Director::getInstance()
    {
        if (!_director)
        {
            _director = new Director();
            _director->initialize();
        }

        return _director;
    }

    bool Director::initialize()
    {
        // init engine modules
        RenderInterface::get();

        return true;
    }

    void Director::setActiveScene(Scene* active_scene)
    {
        _active_scene = active_scene;
    }


    void Director::calculateDeltaTime()
    {
        _delta_time = _timer.getElapsedTime();
        std::cout << _delta_time << std::endl;
        _delta_time = std::max(0.0f, _delta_time);

        _timer.reset();
    }

    void Director::mainLoop()
    {
        while (true)
        {
            calculateDeltaTime();
            //TE_LOG("fps:%f", 1.0 / _delta_time);
            std::cout << "fps:" << 1.0 / _delta_time << std::endl;
            spacingUpdate();
            cullingUpdate();
            renderingUpdate();
            for (int i = 0; i < 1000000; ++i) int a = 1;

            // waiting events
            glfwSwapBuffers(_window);
            glfwPollEvents();
        }
    }

    void Director::cullingUpdate()
    {
        // culling scene
        CullingVisitor visitor(NodeVisitor::TraversalMode::TRAVERSE_CHILDREN, _active_scene->getActiveCamera());
        visitor.apply(_active_scene->getSceneRoot());
    }

    void Director::spacingUpdate()
    {
        // update space status
        SpacingVisitor visitor(NodeVisitor::TraversalMode::TRAVERSE_CHILDREN);
        visitor.apply(_active_scene->getSceneRoot());
    }

    void Director::renderingUpdate()
    {
        // rendering scene
        RenderingVisitor visitor(NodeVisitor::TraversalMode::TRAVERSE_CHILDREN, RenderInterface::get());
        visitor.apply(_active_scene->getSceneRoot());
    }

    void Director::start()
    {
        _timer.setEnabled(true);

        initWindow();
        RenderInterface::get()->init();
        initResource();

        mainLoop();
    }

    void Director::initWindow()
    {
        int width = 800;
        int height = 600;

        TE_ASSERT(width != 0 || height != 0, "Please set right window's size!");

        // init window
        if (!glfwInit())
        {
            TE_ERROR("Unable to initialize GLFW ... exiting");
            exit(EXIT_FAILURE);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        _window = glfwCreateWindow(width, height, "TinyEngine", NULL, NULL);
        if (!_window)
        {
            TE_ERROR("Unable to initialize window ... exiting");
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(_window);

        // init shader functions
        if (glewInit())
        {
            TE_ERROR("Unable to initialize GLEW ... exiting");
            exit(EXIT_FAILURE);
        }

    }

    void Director::initResource()
    {
        // init resource
    }

}
