
// Copyright (c) 2011-2012, Daniel M�ller <dm@g4t3.de>
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

#include "starsgeode.h"

#include "proceduralhimmel.h"
#include "shadermodifier.h"
#include "abstractastronomy.h"
#include "brightstars.h"

#include "mathmacros.h"

#include <osg/Geometry>
#include <osg/Point>
#include <osg/BlendFunc>


StarsGeode::StarsGeode(const ProceduralHimmel &himmel)
:   osg::Geode()
,   m_himmel(himmel)

,   m_program(new osg::Program)
,   m_vShader(new osg::Shader(osg::Shader::VERTEX))
,   m_gShader(new osg::Shader(osg::Shader::GEOMETRY))
,   m_fShader(new osg::Shader(osg::Shader::FRAGMENT))

,   u_starWidth(NULL)
,   u_maxVMag(NULL)
,   u_glareIntensity(NULL)
,   u_glareScale(NULL)
{
    setName("Stars");

    osg::StateSet* stateSet = getOrCreateStateSet();

    setupNode(stateSet);
    setupUniforms(stateSet);
    setupShader(stateSet);
    setupTextures(stateSet);
};


StarsGeode::~StarsGeode()
{
};



void StarsGeode::update()
{
    float fov = m_himmel.getCameraFovHint();
    float height = m_himmel.getViewSizeHeightHint();
    
    static const float TWO_TIMES_SQRT2 = 2.0 * sqrt(2.0);

    u_starWidth->set(static_cast<float>(tan(_rad(fov) / height) * TWO_TIMES_SQRT2));
}


void StarsGeode::setupUniforms(osg::StateSet* stateSet)
{
    u_starWidth = new osg::Uniform("starWidth", 0.0f);
    stateSet->addUniform(u_starWidth);

    u_glareIntensity = new osg::Uniform("glareIntensity", 1.0f);
    stateSet->addUniform(u_glareIntensity);

    u_glareScale = new osg::Uniform("glareScale", 1.0f);
    stateSet->addUniform(u_glareScale);

    u_maxVMag = new osg::Uniform("maxVMag", defaultMaxVMag());
    stateSet->addUniform(u_maxVMag);
}



#include "coords.h"
#include "earth.h"
#include "sideraltime.h"
#include "stars.h"


void StarsGeode::createAndAddDrawable()
{
    osg::Geometry *g = new osg::Geometry;
    addDrawable(g);

    std::vector<s_BrightStar> bss;
    brightstars_readFromFile("resources/brightstars", bss);

    // TODO: use actual time
    t_aTime aTime(2012, 17, 1, 12, 0, 0);

    // TODO: replace this by the equ to horizontal matrix
    float o = earth_trueObliquity(jd(aTime));
    float s = siderealTime(aTime);
    float la = 52.5000;
    float lo = 13.4167;


    osg::Vec4Array* cAry = new osg::Vec4Array;
    osg::Vec3Array* vAry = new osg::Vec3Array;

    for(int i = 0; i < bss.size(); ++i)
    {
        t_equf equ;
        equ.right_ascension = _rightascd(bss[i].RA, 0, 0);
        equ.declination = bss[i].DE;

        osg::Vec3f vec = equ.toHorizontal(s, la, lo).toEuclidean();
        vAry->push_back(vec);
        cAry->push_back(osg::Vec4(bss[i].sRGB_R, bss[i].sRGB_G, bss[i].sRGB_B, bss[i].Vmag));
    }

    g->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    g->setColorArray(cAry);
    g->setVertexArray(vAry);

    g->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, vAry->size()));


    // If things go wrong, fall back to big points.
    g->getOrCreateStateSet()->setAttribute(new osg::Point(2));
}


void StarsGeode::setupNode(osg::StateSet* stateSet)
{
    createAndAddDrawable();


    osg::BlendFunc *blend  = new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE);
    stateSet->setAttributeAndModes(blend, osg::StateAttribute::ON);

    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

    stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
}


void StarsGeode::setupShader(osg::StateSet* stateSet)
{
    m_vShader->setShaderSource(getVertexShaderSource());
    m_gShader->setShaderSource(getGeometryShaderSource());
    m_fShader->setShaderSource(getFragmentShaderSource());

    m_program->addShader(m_vShader);
    m_program->addShader(m_gShader);
    m_program->addShader(m_fShader);

    stateSet->setAttributeAndModes(m_program, osg::StateAttribute::ON);

#ifdef OSGHIMMEL_ENABLE_SHADERMODIFIER
    if(m_himmel.shaderModifier())
    {
        m_himmel.shaderModifier()->registerShader(getName(), m_fShader);
        m_himmel.shaderModifier()->registerShader(getName(), m_gShader);
        m_himmel.shaderModifier()->registerShader(getName(), m_vShader);
    }
#endif // OSGHIMMEL_ENABLE_SHADERMODIFIER
}


void StarsGeode::setupTextures(osg::StateSet* stateSet)
{

}




const float StarsGeode::setGlareIntensity(const float intensity)
{
    u_glareIntensity->set(intensity);
    return getGlareIntensity();
}

const float StarsGeode::getGlareIntensity() const
{
    float intensity;
    u_glareIntensity->get(intensity);

    return intensity;
}


const float StarsGeode::setGlareScale(const float scale)
{
    u_glareScale->set(scale);
    return getGlareScale();
}

const float StarsGeode::getGlareScale() const
{
    float scale;
    u_glareScale->get(scale);

    return scale;
}


const float StarsGeode::setMaxVMag(const float vMag)
{
    u_maxVMag->set(vMag);
    return getMaxVMag();
}

const float StarsGeode::getMaxVMag() const
{
    float vMag;
    u_maxVMag->get(vMag);

    return vMag;
}

const float StarsGeode::defaultMaxVMag() 
{
    return 6.5f;
}




// VertexShader

const std::string StarsGeode::getVertexShaderSource()
{
    return 
        "#version 150 compatibility\n"
        "\n"
        "uniform float starWidth;\n"
        "uniform float maxVMag;\n"
        "\n"
        "out vec4 m_color;\n"
        "\n"
        "const float minB = pow(2.512, -6.5);\n"
        "\n"
        "void main(void)\n"
        "{\n"
	    "    float vMag = gl_Color.w;\n"
        "\n"
	    "    float estB = pow(2.512, maxVMag - vMag);\n"
	    "    float scaledB = minB * estB / starWidth * 0.1;\n"
        "\n"
	    "    m_color = vec4(gl_Color.rgb, scaledB);\n"
        "\n"
        "    gl_Position = vec4(gl_Vertex.xyz, 1.0);\n"
        "}\n\n";
}


// GeometryShader

const std::string StarsGeode::getGeometryShaderSource()
{
    return
        "#version 150 compatibility\n"
        "\n"
        "#extension GL_EXT_geometry_shader4 : enable\n"
        "\n"
        "layout (points) in;\n"
        "layout (triangle_Strip, max_vertices = 4) out;\n"
        "\n"
        "uniform float starWidth;\n"
        "uniform float glareScale;\n"
        "\n"
        "in vec4 m_color[];\n"
        "out vec4 m_c;\n"
        "\n"
        "void main()\n"
        "{\n"
	    "    vec3 p = normalize(gl_in[0].gl_Position.xyz);\n"
        "\n"
	    "    vec3 u = cross(p, vec3(1));\n"
	    "    vec3 v = cross(u, p);\n"
        "\n"
	    "    float scaledB = m_color[0].w;\n"
        "\n"
	    "    m_c = vec4(m_color[0].rgb, scaledB);\n"
        "\n"
	    "    gl_TexCoord[0].z = (1.0 + sqrt(scaledB)) * max(1.0, glareScale);\n"
        "\n"

	    "    float k = starWidth * gl_TexCoord[0].z;\n"
        "\n"
	    "    gl_Position = gl_ModelViewProjectionMatrix * vec4(p - normalize(-u -v) * k, 1.0);\n"
	    "    gl_TexCoord[0].xy = vec2(-1.0, -1.0);\n"
	    "    EmitVertex();\n"
        "\n"
	    "    gl_Position = gl_ModelViewProjectionMatrix * vec4(p - normalize(-u +v) * k, 1.0);\n"
	    "    gl_TexCoord[0].xy = vec2(-1.0,  1.0);\n"
	    "    EmitVertex();\n"
        "\n"
	    "    gl_Position = gl_ModelViewProjectionMatrix  * vec4(p - normalize(+u -v) * k, 1.0);\n"
	    "    gl_TexCoord[0].xy = vec2( 1.0, -1.0);\n"
	    "    EmitVertex();\n"
        "\n"
        "    gl_Position = gl_ModelViewProjectionMatrix * vec4(p - normalize(+u +v) * k, 1.0);\n"
	    "    gl_TexCoord[0].xy = vec2( 1.0,  1.0);\n"
	    "    EmitVertex();\n"
        "}\n\n";
}


// FragmentShader

const std::string StarsGeode::getFragmentShaderSource()
{
    return 

        "#version 150 compatibility\n"
        "\n"
        "uniform float starWidth;\n"
        "uniform float glareIntensity;\n"
        "\n"
        "in vec4 m_c;\n"
        "\n"
        "void main(void)\n"
        "{\n"
	    "    float x = gl_TexCoord[0].x;\n"
	    "    float y = gl_TexCoord[0].y;\n"
        "\n"
	    "    float radius = 0.98;\n"
	    "    float zz = (radius * radius - x * x - y * y);\n"
        "\n"
	    "    if(zz < 0)\n"
		"        discard;\n"
        "\n"
	    "    float s =  gl_TexCoord[0].z;\n"
        "\n"
	    "    float l = length(vec2(x, y));\n"
        "\n"
	    "    float t = smoothstep(1.0, 0.0, l * s);\n"
	    "    float g = smoothstep(1.0, 0.0, pow(l, 0.125)) * glareIntensity;\n"
        "\n"
	    "    gl_FragColor = m_c * (t + g);\n"
        "}\n\n";
}