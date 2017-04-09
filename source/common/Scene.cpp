//#include "common/Scene.h"
//#include "../3rdparty/rapid_xml/include/utXML.h"
//
//namespace te
//{
//    void SceneGraphRes::release()
//    {
//    }
//
//
//    bool SceneGraphRes::load(const char *data, int size)
//    {
//        if (!Resource::load(data, size)) return false;
//
//        XMLDoc doc;
//        doc.parseBuffer(data, size);
//        if (doc.hasError())
//        {
//            return false;
//        }
//
//        // Parse scene nodes and load resources
//        XMLNode root = doc.getRootNode();
//        if (!root.isEmpty())
//        {
//            parseNode(root, 0x0);
//        }
//        else
//        {
//            return false;
//        }
//
//        return true;
//    }
//
//    void SceneGraphRes::parseBaseAttributes(XMLNode &xmlNode, Node &nodeTpl)
//    {
//        /*nodeTpl.name = xmlNode.getAttribute("name", "");
//        nodeTpl.trans.x = (float)atof(xmlNode.getAttribute("tx", "0"));
//        nodeTpl.trans.y = (float)atof(xmlNode.getAttribute("ty", "0"));
//        nodeTpl.trans.z = (float)atof(xmlNode.getAttribute("tz", "0"));
//        nodeTpl.rot.x = (float)atof(xmlNode.getAttribute("rx", "0"));
//        nodeTpl.rot.y = (float)atof(xmlNode.getAttribute("ry", "0"));
//        nodeTpl.rot.z = (float)atof(xmlNode.getAttribute("rz", "0"));
//        nodeTpl.scale.x = (float)atof(xmlNode.getAttribute("sx", "1"));
//        nodeTpl.scale.y = (float)atof(xmlNode.getAttribute("sy", "1"));
//        nodeTpl.scale.z = (float)atof(xmlNode.getAttribute("sz", "1"));
//
//        XMLNode node1 = xmlNode.getFirstChild("Attachment");
//        if (!node1.isEmpty())
//        {
//            nodeTpl.attachmentString.clear();
//            rapidxml::print(std::back_inserter(nodeTpl.attachmentString), *node1.getRapidXMLNode(), 0);
//        }*/
//    }
//
//
//    void SceneGraphRes::parseNode(XMLNode& xmlNode, Node* parentTpl)
//    {
//        Node *nodeTpl = 0x0;
//
//        if (xmlNode.getName() != 0x0)	// Ignore clear tags like DOCTYPE
//        {
//            if (strcmp(xmlNode.getName(), "Reference") == 0)
//            {
//                if (strcmp(xmlNode.getAttribute("sceneGraph", ""), "") != 0)
//                {
//                    Resource *res = Modules::resMan().resolveResHandle(Modules::resMan().addResource(
//                        ResourceTypes::SceneGraph, xmlNode.getAttribute("sceneGraph"), 0, false));
//                    if (res != 0x0) nodeTpl = new ReferenceNodeTpl("", (SceneGraphResource *)res);
//                }
//            }
//            else
//            {
//                NodeRegEntry *entry = Modules::sceneMan().findType(xmlNode.getName());
//                if (entry != 0x0)
//                {
//                    map< string, string > attribs;
//
//                    // Parse custom attributes
//                    XMLAttribute attrib = xmlNode.getFirstAttrib();
//                    while (!attrib.isEmpty())
//                    {
//                        if (strcmp(attrib.getName(), "name") != 0 &&
//                            strcmp(attrib.getName(), "tx") != 0 &&
//                            strcmp(attrib.getName(), "ty") != 0 &&
//                            strcmp(attrib.getName(), "tz") != 0 &&
//                            strcmp(attrib.getName(), "rx") != 0 &&
//                            strcmp(attrib.getName(), "ry") != 0 &&
//                            strcmp(attrib.getName(), "rz") != 0 &&
//                            strcmp(attrib.getName(), "sx") != 0 &&
//                            strcmp(attrib.getName(), "sy") != 0 &&
//                            strcmp(attrib.getName(), "sz") != 0)
//                        {
//                            attribs[attrib.getName()] = attrib.getValue();
//                        }
//                        attrib = attrib.getNextAttrib();
//                    }
//
//                    // Call function pointer
//                    nodeTpl = (*entry->parsingFunc)(attribs);
//                }
//            }
//
//            if (nodeTpl != 0x0)
//            {
//                // Parse base attributes
//                parseBaseAttributes(xmlNode, *nodeTpl);
//
//                // Add to parent
//                if (parentTpl != 0x0)
//                {
//                    parentTpl->children.push_back(nodeTpl);
//                }
//                else
//                {
//                    delete _rootNode;	// Delete default root
//                    _rootNode = nodeTpl;
//                }
//            }
//            else if (strcmp(xmlNode.getName(), "Attachment") != 0)
//            {
//                Modules::log().writeWarning("SceneGraph resource '%s': Unknown node type or missing attribute for '%s'",
//                    _name.c_str(), xmlNode.getName());
//                return;
//            }
//        }
//
//        // Parse children
//        XMLNode xmlNode1 = xmlNode.getFirstChild();
//        while (!xmlNode1.isEmpty())
//        {
//            if (xmlNode1.getName() == 0x0 || strcmp(xmlNode1.getName(), "Attachment") != 0)
//                parseNode(xmlNode1, nodeTpl);
//
//            xmlNode1 = xmlNode1.getNextSibling();
//        }
//    }
//}


#include "common/Scene.h"
#include "common/Camera.h"
#include "common/SpaceState.h"
#include "base/EventDispatcher.h"

namespace te
{
    Scene::Scene()
        :_scene_root(new SceneTree),
        _camera(new Camera)
    {
    }

    Scene::~Scene(){}

    Scene::Scene(const Scene& scene, const CopyOperator& copyop)
    {}

    void Scene::setActiveCamera(Camera* camera)
    {
        _camera = camera;
    }


    SceneTree::SceneTree()
    {
        initComponents();
    }

    SceneTree::~SceneTree(){}

    void SceneTree::initComponents()
    {
        addComponent<SpaceState>();
    }
}
