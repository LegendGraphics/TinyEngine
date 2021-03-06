
#include "base/Director.h"
#include "common/Scene.h"
#include "common/MeshFilter.h"
#include "base/Singleton.hpp"
#include "io/ResourceLoader.h"
using namespace te;

int main(int argc, char** argv)
{ 
    // create a scene
    Scene* scene = new Scene;
    // add a mesh filter to scene root node
    //Mesh* m = new Mesh;
    //ResourceLoader::load(m, "cube_emptyTags.mesh");
    scene->getSceneRoot()->addComponent<MeshFilter>();
    scene->getSceneRoot()->getComponent<MeshFilter>()->load("cube_emptyTags.mesh");

    Scene* scene2 = new Scene;
    scene2->getSceneRoot()->addComponent<MeshFilter>();
    scene2->getSceneRoot()->getComponent<MeshFilter>()->load("cube_emptyTags.mesh");


    Director::getInstance()->setActiveScene(scene);
    Director::getInstance()->start();

    return 0;
}

