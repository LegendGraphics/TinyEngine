
#include "common/NodeVisitor.h"
#include "common/Node.h"
#include "common/Camera.h"
#include "common/SpaceState.h"
#include "renderer/RenderInterface.h"
#include "renderer/RenderWorld.h"
#include "renderer/Runtime/RenderCamera.h"
#include "renderer/Resource/PipelineResource.h"


namespace te
{
    NodeVisitor::NodeVisitor()
        :_traversal_mode(TraversalMode::TRAVERSE_NONE),
        _visitor_type(VisitorType::NONE_UPDATE)
    {
    }

    NodeVisitor::NodeVisitor(const TraversalMode& tm, const VisitorType& vt)
        :_traversal_mode(tm),
        _visitor_type(vt)
    {}

    NodeVisitor::NodeVisitor(const NodeVisitor& node_visitor, const CopyOperator& copyop)
        :_traversal_mode(node_visitor._traversal_mode),
        _visitor_type(node_visitor._visitor_type)
    {

    }

    NodeVisitor::~NodeVisitor()
    {}

    void NodeVisitor::apply(Node* node)
    {
        std::cout << node->getName() << std::endl;
        traverse(node);
    }

    void NodeVisitor::traverse(Node* node)
    {
        if (_traversal_mode == TraversalMode::TRAVERSE_PARENTS) node->ascend(this);
        else if (_traversal_mode == TraversalMode::TRAVERSE_CHILDREN) node->descend(this);

    }

    CullingVisitor::CullingVisitor()
    {
    }

    CullingVisitor::CullingVisitor(const TraversalMode& tm, Camera* camera)
        :NodeVisitor(tm, VisitorType::CULLING_UPDATE),
        _camera(camera)
    {}

    CullingVisitor::CullingVisitor(const CullingVisitor& node_visitor, const CopyOperator& copyop)
        :NodeVisitor(node_visitor, copyop)
    {

    }

    CullingVisitor::~CullingVisitor()
    {}

    void CullingVisitor::apply(Node* node)
    {
        // Now simply frustum culling for every node
        node->setVisible(!_camera->cull(node));
        traverse(node);
    }

    SpacingVisitor::SpacingVisitor()
    {
    }

    SpacingVisitor::SpacingVisitor(const TraversalMode& tm)
        :NodeVisitor(tm, VisitorType::SPACING_UPDATE)
    {}

    SpacingVisitor::SpacingVisitor(const SpacingVisitor& node_visitor, const CopyOperator& copyop)
        : NodeVisitor(node_visitor, copyop)
    {

    }

    SpacingVisitor::~SpacingVisitor()
    {}

    void SpacingVisitor::apply(Node* node)
    {
        // Now simply update space status for every node
        if (node->hasComponent<SpaceState>()) node->getComponent<SpaceState>()->update();
        traverse(node);
    }

    RenderingVisitor::RenderingVisitor()
    {
        
    }

    RenderingVisitor::RenderingVisitor(const TraversalMode& tm, RenderInterface* renderer)
        :NodeVisitor(tm, VisitorType::RENDERING_UPDATE),
        _renderer(renderer)
    {}

    RenderingVisitor::RenderingVisitor(const RenderingVisitor& node_visitor, const CopyOperator& copyop)
        : NodeVisitor(node_visitor, copyop)
    {

    }

    RenderingVisitor::~RenderingVisitor()
    {}

    void RenderingVisitor::apply(Node* node)
    {
        // Now simply rendering every node

        testRenderingPipeline(node); // for test

        traverse(node);
    }

    void RenderingVisitor::testRenderingPipeline(Node* node)
    {
        RenderMsg msg;
        msg.rwm.camera = wrapRenderCamera();
        msg.rwm.world = wrapRenderWorld();
        msg.rwm.rQueue = wrapRenderQueueItem(node);
        msg.rwm.pipeline = wrapPipelineResource();
        msg.rwm.numQueue = 1;

        _renderer->renderWorld(&msg);
    }

    RenderWorld* RenderingVisitor::wrapRenderWorld()
    {
        RenderWorld* rw = new RenderWorld;
        return rw;
    }

    RenderCamera* RenderingVisitor::wrapRenderCamera()
    {
        RenderCamera* rc = new RenderCamera;
        return rc;
    }

    te::RenderQueueItem* RenderingVisitor::wrapRenderQueueItem(Node* node)
{
        RenderQueueItem* rqi = new RenderQueueItem;
        return rqi;
    }

    PipelineResource* RenderingVisitor::wrapPipelineResource()
    {
        PipelineResource* pr = new PipelineResource;
        pr->load("forward.pipeline.xml");
        return pr;
    }

}
