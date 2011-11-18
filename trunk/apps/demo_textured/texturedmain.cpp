
// Copyright (c) 2011, Daniel M�ller <dm@g4t3.de>
// Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright 
//     notice, this list of conditions and the following disclaimer in the 
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of the Computer Graphics Systems Group at the 
//     Hasso-Plattner-Institute (HPI), Germany nor the names of its 
//     contributors may be used to endorse or promote products derived from 
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#include "include/polarmappedhimmel.h"
#include "include/cubemappedhimmel.h"
#include "include/paraboloidmappedhimmel.h"
#include "include/spheremappedhimmel.h"
#include "include/horizonband.h"

#include "include/timef.h"


#include <osg/TextureCubeMap>
#include <osg/Texture2D>

#include <osgDB/ReadFile>


// scenes

TimeF *g_timef(new TimeF(0.f, 60.f));


osg::ref_ptr<AbstractHimmel> createPolarMappedDemo()
{
    osg::ref_ptr<PolarMappedHimmel> himmel = new PolarMappedHimmel(PolarMappedHimmel::MM_Half, true);

    himmel->hBand()->setBottomColor(osg::Vec4(0.30f, 0.30f, 0.30f, 1.00f));
    himmel->hBand()->setColor(      osg::Vec4(0.20f, 0.20f, 0.20f, 1.00f));
    himmel->hBand()->setScale(0.3f);

    himmel->assignTime(g_timef);
    himmel->setTransitionDuration(0.05f);

    himmel->getOrCreateTexture2D(0)->setImage(osgDB::readImageFile("resources/polar_half_art_1.jpg"));
    himmel->getOrCreateTexture2D(1)->setImage(osgDB::readImageFile("resources/polar_half_art_2.jpg"));
    himmel->getOrCreateTexture2D(2)->setImage(osgDB::readImageFile("resources/polar_half_gen_3.jpg"));
    himmel->getOrCreateTexture2D(3)->setImage(osgDB::readImageFile("resources/polar_half_pho_1.jpg"));
    himmel->getOrCreateTexture2D(4)->setImage(osgDB::readImageFile("resources/polar_half_pho_7.jpg"));

    himmel->pushTextureUnit(0, 0.0f);
    himmel->pushTextureUnit(1, 0.2f);
    himmel->pushTextureUnit(2, 0.4f);
    himmel->pushTextureUnit(3, 0.6f);
    himmel->pushTextureUnit(4, 0.8f);

    return himmel;
}


osg::ref_ptr<AbstractHimmel> createCubeMappedDemo()
{
    osg::ref_ptr<CubeMappedHimmel> himmel = new CubeMappedHimmel();

    himmel->assignTime(g_timef);
    himmel->setTransitionDuration(0.05f);

    std::string name[] = { "4", "6", "9", "17", "19" };
    osg::TextureCubeMap *tcm[5];

    const int n = 5;
    for(int i = 0; i < n; ++i)
    {
        tcm[i] = himmel->getOrCreateTextureCubeMap(i);

        tcm[i]->setImage(osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_px.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_nx.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_py.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_ny.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_pz.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_nz.jpg"));

      himmel->pushTextureUnit(i, (i * 1.f) / (n * 1.f));
    }
    return himmel;
}


osg::ref_ptr<AbstractHimmel> createParaboloidMappedDemo()
{
    osg::ref_ptr<ParaboloidMappedHimmel> himmel = new ParaboloidMappedHimmel();

    himmel->assignTime(g_timef);
    himmel->setTransitionDuration(0.05f);

    himmel->getOrCreateTexture2D(0)->setImage(osgDB::readImageFile("resources/paraboloid_gen_0.jpg"));
    himmel->getOrCreateTexture2D(1)->setImage(osgDB::readImageFile("resources/paraboloid_gen_1.jpg"));
    himmel->getOrCreateTexture2D(2)->setImage(osgDB::readImageFile("resources/paraboloid_gen_2.jpg"));
    himmel->getOrCreateTexture2D(3)->setImage(osgDB::readImageFile("resources/paraboloid_pho_0.jpg"));
    himmel->getOrCreateTexture2D(4)->setImage(osgDB::readImageFile("resources/paraboloid_pho_1.jpg"));

    himmel->pushTextureUnit(0, 0.0f);
    himmel->pushTextureUnit(1, 0.2f);
    himmel->pushTextureUnit(2, 0.4f);
    himmel->pushTextureUnit(3, 0.6f);
    himmel->pushTextureUnit(4, 0.8f);

    return himmel;
}


osg::ref_ptr<AbstractHimmel> createSphereMappedDemo()
{
    osg::ref_ptr<SphereMappedHimmel> himmel = new SphereMappedHimmel(SphereMappedHimmel::MM_TowardsNegY);

    himmel->assignTime(g_timef);
    himmel->setTransitionDuration(0.05f);

    himmel->getOrCreateTexture2D(0)->setImage(osgDB::readImageFile("resources/sphere_gen_0.jpg"));
    himmel->getOrCreateTexture2D(1)->setImage(osgDB::readImageFile("resources/sphere_gen_2.jpg"));

    himmel->pushTextureUnit(0, 0.00f);
    himmel->pushTextureUnit(1, 0.50f);

    return himmel;
}


// demo

#include <osgViewer/Viewer>
#include <osg/MatrixTransform>

#include <osg/PolygonMode>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/TerrainManipulator>

#include <iostream>

enum e_Demo
{
    D_PolarMappedHimmel       = 0
,   D_CubeMappedHimmel        = 1
,   D_ParaboloidMappedHimmel  = 2
,   D_SphereMappedHimmel      = 3
};


osg::ref_ptr<osg::MatrixTransform> g_scene;

e_Demo g_demo;
std::map<e_Demo, osg::ref_ptr<AbstractHimmel> > g_himmelsByDemo;


osgViewer::View *g_view = NULL;

const float g_fovBackup = 60.f;
float g_fov = g_fovBackup;


void activateDemo(e_Demo demo)
{
    g_himmelsByDemo[D_PolarMappedHimmel]     ->setNodeMask(D_PolarMappedHimmel      == demo);
    g_himmelsByDemo[D_CubeMappedHimmel]      ->setNodeMask(D_CubeMappedHimmel       == demo);
    g_himmelsByDemo[D_ParaboloidMappedHimmel]->setNodeMask(D_ParaboloidMappedHimmel == demo);
    g_himmelsByDemo[D_SphereMappedHimmel]    ->setNodeMask(D_SphereMappedHimmel     == demo);
}


void fovChanged();

class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:
    KeyboardEventHandler()
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
    {
        switch(ea.getEventType())
        {
        case(osgGA::GUIEventAdapter::FRAME):

            g_timef->update();
            break;

        case(osgGA::GUIEventAdapter::KEYDOWN):
            {
                if(ea.getKey() == osgGA::GUIEventAdapter::KEY_Space)
                {
                    g_demo = static_cast<e_Demo>((g_demo + 1) % 4);
                    activateDemo(g_demo);
                }
                else if(ea.getKey() == 'p' || ea.getKey() == 'P')
                {
                    g_timef->reset();
                    g_timef->setSecondsPerCycle(60.f);
                }
                else if(ea.getKey() == 'p' || ea.getKey() == 'P')
                {
                    if(g_timef->isRunning())
                        g_timef->pause();
                    else
                        g_timef->run();
                }
                else if(ea.getKey() == '-')
                {
                    g_timef->setSecondsPerCycle(g_timef->getSecondsPerCycle() + 1);
                }
                else if(ea.getKey() == '+')
                {
                    g_timef->setSecondsPerCycle(g_timef->getSecondsPerCycle() - 1);
                }
            }
            break;

        case(osgGA::GUIEventAdapter::SCROLL):
            {
                const float f = 1.00f
                    + (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN ? -0.08f : +0.08f);

                if(g_fov * f >= 1.f && g_fov * f <= 179.f)
                    g_fov *= f;

                fovChanged();

                return true;
            }
            break;

        case(osgGA::GUIEventAdapter::RELEASE):

            if(ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON
            && (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL) != 0)
            {
                g_fov = g_fovBackup;
                fovChanged();

                return true;
            }
            break;

        default:
            break;
        };
        return false;
    }
};


void fovChanged()
{
    const double aspectRatio(g_view->getCamera()->getViewport()->aspectRatio());
    g_view->getCamera()->setProjectionMatrixAsPerspective(g_fov, aspectRatio, 0.1f, 8.0f);
}


osg::Group *createHimmelScene()
{
    osg::Group *group = new osg::Group;

    g_himmelsByDemo[D_PolarMappedHimmel] = createPolarMappedDemo();
    group->addChild(g_himmelsByDemo[D_PolarMappedHimmel]);

    g_himmelsByDemo[D_CubeMappedHimmel] = createCubeMappedDemo();
    group->addChild(g_himmelsByDemo[D_CubeMappedHimmel]);

    g_himmelsByDemo[D_ParaboloidMappedHimmel] = createParaboloidMappedDemo();
    group->addChild(g_himmelsByDemo[D_ParaboloidMappedHimmel]);

    g_himmelsByDemo[D_SphereMappedHimmel] = createSphereMappedDemo();
    group->addChild(g_himmelsByDemo[D_SphereMappedHimmel]);

    return group;
}


// render to cube map code

#include <osg/TexMat>
#include <osg/TexGenNode>
#include <osg/Material>

osg::Node *createReflector()
{
    osg::Node *node = osgDB::readNodeFile("resources/knot.obj");

    osg::ref_ptr<osg::Material> m = new osg::Material;
    m->setColorMode(osg::Material::DIFFUSE);
    m->setAmbient  (osg::Material::FRONT_AND_BACK, osg::Vec4(6.0f, 6.0f, 6.0f, 1.f));

    node->getOrCreateStateSet()->setAttributeAndModes(m.get(), osg::StateAttribute::ON);

    return node;
}
  
class UpdateCameraAndTexGenCallback : public osg::NodeCallback
{
    public:
        typedef std::vector< osg::ref_ptr<osg::Camera> >  t_Cameras;

        UpdateCameraAndTexGenCallback(
            osg::NodePath &reflectorNodePath
        ,   t_Cameras &cameras)
        :   m_reflectorNodePath(reflectorNodePath)
        ,   m_cameras(cameras)
        {
        }
       
        virtual void operator()(osg::Node *node, osg::NodeVisitor *nv)
        {
            traverse(node, nv);

            // compute the position of the center of the reflector subgraph
            osg::Matrixd worldToLocal = osg::computeWorldToLocal(m_reflectorNodePath);
            osg::BoundingSphere bs = m_reflectorNodePath.back()->getBound();

            osg::Vec3 position = bs.center();

            typedef std::pair<osg::Vec3, osg::Vec3> ImageData;
            const ImageData id[] =
            {
                ImageData( osg::Vec3( 1,  0,  0), osg::Vec3( 0, -1,  0))
            ,   ImageData( osg::Vec3(-1,  0,  0), osg::Vec3( 0, -1,  0))
            ,   ImageData( osg::Vec3( 0,  1,  0), osg::Vec3( 0,  0,  1))
            ,   ImageData( osg::Vec3( 0, -1,  0), osg::Vec3( 0,  0, -1))
            ,   ImageData( osg::Vec3( 0,  0,  1), osg::Vec3( 0, -1,  0))
            ,   ImageData( osg::Vec3( 0,  0, -1), osg::Vec3( 0, -1,  0)) 
            };

            for(unsigned int i = 0;  i < 6 && i < m_cameras.size(); ++i)
            {
                osg::Matrix localOffset;
                localOffset.makeLookAt(position, position + id[i].first, id[i].second);

                osg::Matrix viewMatrix = worldToLocal * localOffset;
            
                m_cameras[i]->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
                m_cameras[i]->setProjectionMatrixAsFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10000.0);
                m_cameras[i]->setViewMatrix(viewMatrix);
            }
        }

    protected:
        virtual ~UpdateCameraAndTexGenCallback() 
        {
        }

        osg::NodePath   m_reflectorNodePath;        
        t_Cameras       m_cameras;
};


class TexMatCullCallback : public osg::NodeCallback
{
    public:
    
        TexMatCullCallback(osg::TexMat *texmat)
        :   m_texmat(texmat)
        {
        }
       
        virtual void operator()(osg::Node *node, osg::NodeVisitor *nv)
        {
            traverse(node, nv);

            osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
            if(cv)
            {
                const osg::Quat quat = cv->getModelViewMatrix()->getRotate();
                m_texmat->setMatrix(osg::Matrix::rotate(quat.inverse()));
            }
        }
        
    protected:
        osg::ref_ptr<osg::TexMat> m_texmat;
};


osg::Group *createScene(
    osg::Node *scene
,   osg::Node *reflector
,   const unsigned int unit
,   const unsigned int texSize)
{
    osg::Group* group = new osg::Group;

    // Set up cube map.
    
    osg::TextureCubeMap *texture = new osg::TextureCubeMap;
    texture->setTextureSize(texSize, texSize);

    texture->setInternalFormat(GL_RGB);

    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    texture->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

    texture->setFilter(osg::TextureCubeMap::MIN_FILTER, osg::TextureCubeMap::LINEAR);
    texture->setFilter(osg::TextureCubeMap::MAG_FILTER, osg::TextureCubeMap::LINEAR);

    // Set up the render to texture cameras.

    UpdateCameraAndTexGenCallback::t_Cameras cameras;

    for(unsigned int i = 0; i < 6; ++i)
    {
        osg::Camera* camera = new osg::Camera;

        camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera->setViewport(0, 0, texSize, texSize);
        camera->setRenderOrder(osg::Camera::PRE_RENDER);

        // Tell the camera to use OpenGL frame buffer object where supported.
        camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

        // Attach the texture and use it as the color buffer.
        camera->attach(osg::Camera::COLOR_BUFFER, texture, 0, i);

        camera->addChild(scene);
        group->addChild(camera);

        cameras.push_back(camera);
    }


    // create the texgen node to project the tex coords onto the subgraph:
    osg::TexGenNode* texgenNode = new osg::TexGenNode;

    texgenNode->getTexGen()->setMode(osg::TexGen::REFLECTION_MAP);
    texgenNode->setTextureUnit(unit);

    group->addChild(texgenNode);

    // set the reflected subgraph so that it uses the texture and tex gen settings.    
    {
        osg::Node* reflectorNode = reflector;
        group->addChild(reflectorNode);

        osg::StateSet* stateset = reflectorNode->getOrCreateStateSet();
        stateset->setTextureAttributeAndModes(unit,texture,osg::StateAttribute::ON);
        stateset->setTextureMode(unit,GL_TEXTURE_GEN_S,osg::StateAttribute::ON);
        stateset->setTextureMode(unit,GL_TEXTURE_GEN_T,osg::StateAttribute::ON);
        stateset->setTextureMode(unit,GL_TEXTURE_GEN_R,osg::StateAttribute::ON);
        stateset->setTextureMode(unit,GL_TEXTURE_GEN_Q,osg::StateAttribute::ON);

        osg::TexMat* texmat = new osg::TexMat;
        stateset->setTextureAttributeAndModes(unit, texmat, osg::StateAttribute::ON);
        
        reflectorNode->setCullCallback(new TexMatCullCallback(texmat));
    }
    
    // add the reflector scene to draw just as normal
    group->addChild(scene);
    
    osg::NodePath nodeList;
    nodeList.push_back(reflector);

    // set an update callback to keep moving the camera and tex gen in the right direction.
    group->setUpdateCallback(new UpdateCameraAndTexGenCallback(nodeList, cameras));

    return group;
}


void initializeManipulators(osgViewer::View &view)
{
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

    keyswitchManipulator->addMatrixManipulator('1', "Terrain",   new osgGA::TerrainManipulator());
    keyswitchManipulator->addMatrixManipulator('2', "Trackball", new osgGA::TrackballManipulator());
    keyswitchManipulator->addMatrixManipulator('3', "Flight",    new osgGA::FlightManipulator());
    keyswitchManipulator->addMatrixManipulator('4', "Drive",     new osgGA::DriveManipulator());

    view.setCameraManipulator(keyswitchManipulator.get());
}


int main(int argc, char* argv[])
{
    osg::ArgumentParser arguments(&argc, argv);

    arguments.getApplicationUsage()->setDescription(
        arguments.getApplicationName() + " is the demo which demonstrates using of various texture mapped skies implemented in osghimmel");

    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName());

    arguments.getApplicationUsage()->addCommandLineOption("-h or --help", "Display this information.");
    arguments.getApplicationUsage()->addCommandLineOption("--polar", "Start with a himmel using polar mapped textures.");
    arguments.getApplicationUsage()->addCommandLineOption("--cube", "Start with a himmel using cube mapped textures.");
    arguments.getApplicationUsage()->addCommandLineOption("--paraboloid", "Start with a himmel using paraboloid mapped textures.");
    arguments.getApplicationUsage()->addCommandLineOption("--sphere", "Start with a himmel using sphere mapped textures.");

    osgViewer::Viewer viewer(arguments);

    if(arguments.read("-h") || arguments.read("--help"))
    {
        arguments.getApplicationUsage()->write(std::cout);
        return 1;
    }

    osg::notify(osg::NOTICE) << "Use [1] to [4] to select camera manipulator." << std::endl;
    osg::notify(osg::NOTICE) << "Use [space] to cycle mapping techniques." << std::endl;
    osg::notify(osg::NOTICE) << "Use [p] to pause/unpause time." << std::endl;
    osg::notify(osg::NOTICE) << "Use [+] and [-] to increase/decrease seconds per cycle." << std::endl;
    osg::notify(osg::NOTICE) << "Use [r] to reset the time." << std::endl;

    g_demo = D_CubeMappedHimmel;

    while(arguments.read("--polar")) 
        g_demo = D_PolarMappedHimmel;

    while(arguments.read("--cube")) 
        g_demo = D_CubeMappedHimmel;

    while(arguments.read("--paraboloid")) 
        g_demo = D_ParaboloidMappedHimmel;

    while(arguments.read("--sphere")) 
        g_demo = D_SphereMappedHimmel;

    g_view = dynamic_cast<osgViewer::View*>(&viewer);

    viewer.setUpViewInWindow(128, 128, 640, 480);

    initializeManipulators(viewer);
    

    osg::Camera* cam = g_view->getCamera();
    fovChanged();


    g_scene = new osg::MatrixTransform;

    osg::ref_ptr<osg::Group> himmel = createHimmelScene();

    osg::ref_ptr<osg::Group> scene = createScene(
        himmel.get(), createReflector(), 0, 128);

    g_scene->addChild(scene.get());
    g_view->setSceneData(g_scene.get());


    activateDemo(g_demo);

    viewer.addEventHandler(new KeyboardEventHandler);

    return viewer.run();
}