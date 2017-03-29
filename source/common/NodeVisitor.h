#ifndef COMMON_NODE_VISITOR_H
#define COMMON_NODE_VISITOR_H

#include "base/Refptr.hpp"
#include "base/Ref.h"
#include "common/Clone.h"

namespace te
{

    class Node;
    class Camera;
    class Scene;
    class RenderInterface;

    // consider two things: traversal mode and visitor type
    class NodeVisitor
    {
    public:
        enum class TraversalMode
        {
            TRAVERSE_NONE,
            TRAVERSE_PARENTS,
            TRAVERSE_CHILDREN
        };

        enum class VisitorType
        {
            NONE_UPDATE,
            CULLING_UPDATE,
            SPACING_UPDATE,
            RENDERING_UPDATE
        };

        NodeVisitor();
        NodeVisitor(const TraversalMode& tm, const VisitorType& vt);
        NodeVisitor(const NodeVisitor& node_visitor, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);

        virtual ~NodeVisitor();

        virtual void apply(Node* node);

        void traverse(Node* node);

    protected:
        TraversalMode   _traversal_mode;
        VisitorType     _visitor_type;
    };

    class CullingVisitor : public NodeVisitor
    {
    public:
        CullingVisitor();
        CullingVisitor(const TraversalMode& tm, Camera* camera);
        CullingVisitor(const CullingVisitor& node_visitor, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);

        virtual ~CullingVisitor();

        virtual void apply(Node* node);

    protected:
        Camera*     _camera;
    };

    class SpacingVisitor : public NodeVisitor
    {
    public:
        SpacingVisitor();
        SpacingVisitor(const TraversalMode& tm);
        SpacingVisitor(const SpacingVisitor& node_visitor, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);

        virtual ~SpacingVisitor();

        virtual void apply(Node* node);

    protected:
        Scene*      _scene;
    };

    class RenderWorld;
    class RenderCamera;
    class RenderQueueItem;
    class PipelineResource;

    class RenderingVisitor : public NodeVisitor
    {
    public:
        RenderingVisitor();
        RenderingVisitor(const TraversalMode& tm, RenderInterface* renderer);
        RenderingVisitor(const RenderingVisitor& node_visitor, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);

        virtual ~RenderingVisitor();

        virtual void apply(Node* node);

    private:
        void testRenderingPipeline(Node* node);

    protected:
        RenderInterface*    _renderer;

    };

    class RenderResourceVisitor : public RenderingVisitor
    {
    public:
        RenderResourceVisitor();
        RenderResourceVisitor(const TraversalMode& tm, RenderInterface* renderer);
        RenderResourceVisitor(const RenderResourceVisitor& node_visitor, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);
        virtual ~RenderResourceVisitor();

        virtual void apply(Node* node);
    };


}

#endif // COMMON_NODE_VISITOR_H