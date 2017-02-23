#include <iostream>
#include <memory>
#include "io/MeshLoader.h"
#include "common/Mesh.h"
#include "common/MeshFilter.h"
#include "common/Node.h"
#include "common/NodeVisitor.h"
#include "base/Director.h"

using namespace te;




int main(int argc, char** argv)
{ 
    Director::getInstance()->start();

    /*MeshRes mr;
    mr.load("cube_emptyTags.mesh");

    Mesh* m = new Mesh;
    m->init(&mr);

    Node* root = new Node;
    root->setName("root");
    root->addComponent<MeshFilter>(m);
    root->addComponent<Transform>(Vector3(0.0f, 0.0f, 0.0f));


    Node* n1 = new Node;
    n1->setName("n1");
    n1->addComponent<MeshFilter>(m);
    n1->addComponent<Transform>(0.0f, 0.0f, 0.0f);

    Node* n2 = new Node;
    n2->setName("n2");
    n2->addComponent<MeshFilter>(m);
    n2->addComponent<Transform>(0.0f, 0.0f, 0.0f);

    Node* m1 = new Node;
    m1->setName("m1");
    m1->addComponent<MeshFilter>(m);
    m1->addComponent<Transform>(0.0f, 0.0f, 0.0f);

    Node* m2 = new Node;
    m2->setName("m2");
    m2->addComponent<MeshFilter>(m);
    m2->addComponent<Transform>(0.0f, 0.0f, 0.0f);

    root->addChild(n1);
    root->addChild(n2);
    n1->addChild(m1);
    n2->addChild(m2);

    root->accept(new NodeVisitor(NodeVisitor::TraversalMode::TRAVERSE_CHILDREN,
        NodeVisitor::VisitorType::VISITOR_NODE));

    delete root;

    getchar();*/
    return 0;
}

