﻿

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "engine_common.h"
#include "util.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200


class Tutorial26 : public ICallbacks
{
public:

    Tutorial26()
    {
        m_pLightingTechnique = NULL;
        m_pGameCamera = NULL;
        m_pSphereMesh = NULL;
        m_scale = 0.0f;
        m_pTexture = NULL;
        m_pNormalMap = NULL;
        m_pTrivialNormalMap = NULL;

        m_dirLight.AmbientIntensity = 0.2f;
        m_dirLight.DiffuseIntensity = 0.8f;
        m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        m_dirLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);

        m_persProjInfo.FOV = 60.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 100.0f;

        m_bumpMapEnabled = true;
    }


    ~Tutorial26()
    {
        SAFE_DELETE(m_pLightingTechnique);
        SAFE_DELETE(m_pGameCamera);
        SAFE_DELETE(m_pSphereMesh);
        SAFE_DELETE(m_pTexture);
        SAFE_DELETE(m_pNormalMap);
        SAFE_DELETE(m_pTrivialNormalMap);
    }


    bool Init()
    {
        Vector3f Pos(0.5f, 1.025f, 0.25f);
        Vector3f Target(0.0f, -0.5f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

        m_pLightingTechnique = new LightingTechnique();

        if (!m_pLightingTechnique->Init()) {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pLightingTechnique->Enable();
        m_pLightingTechnique->SetDirectionalLight(m_dirLight);
        m_pLightingTechnique->SetColorTextureUnit(0);
        m_pLightingTechnique->SetNormalMapTextureUnit(2);

        m_pSphereMesh = new Mesh();

        if (!m_pSphereMesh->LoadMesh("D:/labs/Graphics/lab4.26/Content/box.obj")) {
            return false;
        }

        m_pTexture = new Texture(GL_TEXTURE_2D, "D:/labs/Graphics/lab4.26/Content/bricks.jpg");

        if (!m_pTexture->Load()) 
        {
            return false;
        }

        m_pTexture->Bind(COLOR_TEXTURE_UNIT);

        m_pNormalMap = new Texture(GL_TEXTURE_2D, "D:/labs/Graphics/lab4.26/Content/normal_map.jpg");

        if (!m_pNormalMap->Load()) {
            return false;
        }

        m_pTrivialNormalMap = new Texture(GL_TEXTURE_2D, "D:/labs/Graphics/lab4.26/Content/normal_up.jpg");

        if (!m_pTrivialNormalMap->Load()) {
            return false;
        }

        return true;
    }


    void Run()
    {
        GLUTBackendRun(this);
    }


    virtual void RenderSceneCB()
    {
        m_pGameCamera->OnRender();
        m_scale += 0.01f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_pLightingTechnique->Enable();

        Pipeline p;
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 3.0f);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        p.SetPerspectiveProj(m_persProjInfo);

        m_pTexture->Bind(COLOR_TEXTURE_UNIT);

        if (m_bumpMapEnabled)
        {
            m_pNormalMap->Bind(NORMAL_TEXTURE_UNIT);
        }
        else
        {
            m_pTrivialNormalMap->Bind(NORMAL_TEXTURE_UNIT);
        }

        m_pLightingTechnique->SetWVP(p.GetWVPTrans());
        m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
        m_pSphereMesh->Render();

        glutSwapBuffers();
    }


    virtual void IdleCB()
    {
        RenderSceneCB();
    }


    virtual void SpecialKeyboardCB(int Key, int x, int y)
    {
        m_pGameCamera->OnKeyboard(Key);
    }


    virtual void KeyboardCB(unsigned char Key, int x, int y)
    {
        switch (Key) {
        case 'q':
            glutLeaveMainLoop();
            break;

        case 'b':
            m_bumpMapEnabled = !m_bumpMapEnabled;
            break;
        }
    }


    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

private:

    LightingTechnique* m_pLightingTechnique;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_dirLight;
    Mesh* m_pSphereMesh;
    Texture* m_pTexture;
    Texture* m_pNormalMap;
    Texture* m_pTrivialNormalMap;
    PersProjInfo m_persProjInfo;
    bool m_bumpMapEnabled;
};


int main(int argc, char** argv)
{
    GLUTBackendInit(argc, argv);
    Magick::InitializeMagick(nullptr); // добавили

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Tutorial 26")) {
        return 1;
    }

    Tutorial26* pApp = new Tutorial26();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;

    return 0;
}