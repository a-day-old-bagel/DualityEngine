/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <limits.h>
#include <string.h>

//#include "ogldev_math_3d.h"
//#include "ogldev_util.h"
#include "../Headers/lighting_technique.h"


LightingTechnique::LightingTechnique()
{   
}

bool LightingTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "lighting.vs")) {
        return false;
    }


    if (!AddShader(GL_FRAGMENT_SHADER, "lighting.fs")) {
        return false;
    }

    if (!Finalize()) {
        //return false;
    }

    m_colorTextureLocation = GetUniformLocation("gColorMap");
    m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
    m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
    m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
    m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
    m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
    m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
    m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
    m_numPointLightsLocation = GetUniformLocation("gNumPointLights");
    m_numSpotLightsLocation = GetUniformLocation("gNumSpotLights");

    if (m_dirLightLocation.AmbientIntensity == INVALID_VALUE_32 ||
        m_colorTextureLocation == INVALID_VALUE_32 ||
        m_eyeWorldPosLocation == INVALID_VALUE_32 ||
        m_dirLightLocation.Color == INVALID_VALUE_32 ||
        m_dirLightLocation.DiffuseIntensity == INVALID_VALUE_32 ||
        m_dirLightLocation.Direction == INVALID_VALUE_32 ||
        m_matSpecularIntensityLocation == INVALID_VALUE_32 ||
        m_matSpecularPowerLocation == INVALID_VALUE_32 ||
        m_numPointLightsLocation == INVALID_VALUE_32 ||
        m_numSpotLightsLocation == INVALID_VALUE_32) {
        return false;
    }

    for (unsigned int i = 0 ; i < ARRAY_COUNT(m_pointLightsLocation) ; i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
        m_pointLightsLocation[i].Color = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
        m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Position", i);
        m_pointLightsLocation[i].Position = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
        m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
        m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
        m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
        m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

        if (m_pointLightsLocation[i].Color == INVALID_VALUE_32 ||
            m_pointLightsLocation[i].AmbientIntensity == INVALID_VALUE_32 ||
            m_pointLightsLocation[i].Position == INVALID_VALUE_32 ||
            m_pointLightsLocation[i].DiffuseIntensity == INVALID_VALUE_32 ||
            m_pointLightsLocation[i].Atten.Constant == INVALID_VALUE_32 ||
            m_pointLightsLocation[i].Atten.Linear == INVALID_VALUE_32 ||
            m_pointLightsLocation[i].Atten.Exp == INVALID_VALUE_32) {
            return false;
        }
    }

    for (unsigned int i = 0 ; i < ARRAY_COUNT(m_spotLightsLocation) ; i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
        m_spotLightsLocation[i].Color = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
        m_spotLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
        m_spotLightsLocation[i].Position = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
        m_spotLightsLocation[i].Direction = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
        m_spotLightsLocation[i].Cutoff = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
        m_spotLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
        m_spotLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
        m_spotLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
        m_spotLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

        if (m_spotLightsLocation[i].Color == INVALID_VALUE_32 ||
            m_spotLightsLocation[i].AmbientIntensity == INVALID_VALUE_32 ||
            m_spotLightsLocation[i].Position == INVALID_VALUE_32 ||
            m_spotLightsLocation[i].Direction == INVALID_VALUE_32 ||
            m_spotLightsLocation[i].Cutoff == INVALID_VALUE_32 ||
            m_spotLightsLocation[i].DiffuseIntensity == INVALID_VALUE_32 ||
            m_spotLightsLocation[i].Atten.Constant == INVALID_VALUE_32 ||
            m_spotLightsLocation[i].Atten.Linear == INVALID_VALUE_32 ||
            m_spotLightsLocation[i].Atten.Exp == INVALID_VALUE_32) {
            return false;
        }
    }

    for (unsigned int i = 0; i < ARRAY_COUNT(m_colorLocation) ; i++) {
        char Name[32];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gColor[%d]", i);
        
        m_colorLocation[i] = GetUniformLocation(Name);
        
        if (m_colorLocation[i] == INVALID_VALUE_32) {
            return false;            
        }
    }
    
    return true;
}


void LightingTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorTextureLocation, TextureUnit);
}


void LightingTechnique::SetDirectionalLight(const DirectionalLightx& Light)
{
    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
    glm::vec3 Direction = Light.Direction;
    glm::normalize(Direction);
    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}


void LightingTechnique::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
    glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}


void LightingTechnique::SetMatSpecularIntensity(float Intensity)
{
    glUniform1f(m_matSpecularIntensityLocation, Intensity);
}


void LightingTechnique::SetMatSpecularPower(float Power)
{
    glUniform1f(m_matSpecularPowerLocation, Power);
}


void LightingTechnique::SetPointLights(unsigned int NumLights, const PointLightx* pLights)
{
    glUniform1i(m_numPointLightsLocation, NumLights);
    
    for (unsigned int i = 0 ; i < NumLights ; i++) {
        glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
        glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
    }
}

void LightingTechnique::SetSpotLights(unsigned int NumLights, const SpotLightx* pLights)
{
    glUniform1i(m_numSpotLightsLocation, NumLights);

    for (unsigned int i = 0 ; i < NumLights ; i++) {
        glUniform3f(m_spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(m_spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(m_spotLightsLocation[i].Position,  pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glm::vec3 Direction = pLights[i].Direction;
        glm::normalize(Direction);
        glUniform3f(m_spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
        glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(glm::radians(pLights[i].Cutoff)));
        glUniform1f(m_spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(m_spotLightsLocation[i].Atten.Linear,   pLights[i].Attenuation.Linear);
        glUniform1f(m_spotLightsLocation[i].Atten.Exp,      pLights[i].Attenuation.Exp);
    }
}


void LightingTechnique::SetColor(unsigned int Index, const glm::vec4& Color)
{
    glUniform4f(m_colorLocation[Index], Color.x, Color.y, Color.z, Color.w);
}