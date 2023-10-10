#include "core.h"

#include <iostream>

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Log.h"
#include "Application.h"
#include "Texture.h"
#include "RenderTexture.h"


class MyApp : public Application {

    // render vars
    Renderer ren;
    ptr<Mesh> quad;
    ptr<Mesh> box;

    ptr<Camera> cam;

    // event vars
    bool isDragging = false;
    glm::vec2 lastMousePos = glm::vec2(0, 0);
    

protected:
    virtual void Start() override
    {
        ren.Init(window);
        ren.SetClearColor(.2, .2, .2);
        cam = ren.GetMainCamera();

        quad = Mesh::Quad();
        box = Mesh::Cube();

        
        ptr<Shader> defaultShader;
        defaultShader = ren.LoadShader("defaultShader", "../shaders/default.vert", "../shaders/default.frag");

        ptr<Shader> testShader;
        testShader = ren.LoadShader("test", "../shaders/fun/test.vert", "../shaders/fun/test.frag");

        ptr<Shader> texShader;
        texShader = ren.LoadShader("texture", "../shaders/texture.vert", "../shaders/texture.frag");

        ptr<Texture> tex;
        tex = std::make_shared<Texture>();
        tex->Init("../res/textures/checkers-2.png");


        ptr<RenderObject> object = std::make_shared<RenderObject>(
            std::make_shared<Transform>(
                glm::vec3(0, 0, 0),
                glm::vec3(45, 45, 45),
                glm::vec3(2)
            ),
            Mesh::Sphere(50, 70),
            std::make_shared<Material>("default", testShader, tex)
        );
        ren.PushObject(object);


        //ptr<RenderObject> object1 = std::make_shared<RenderObject>(
        //    std::make_shared<Transform>(
        //        glm::vec3(0, -1, 0),
        //        glm::vec3(0),
        //        glm::vec3(1.5)
        //    ),
        //    box,
        //    std::make_shared<Material>("default", test, tex)
        //);
        //ren.PushObject(object1);

        ptr<RenderObject> object2 = std::make_shared<RenderObject>(
            std::make_shared<Transform>(
                glm::vec3(0, -5, 0),
                glm::vec3(-90, 0, 0),
                glm::vec3(5)
            ),
            quad,
            std::make_shared<Material>("default", texShader, tex)
        );
        ren.PushObject(object2);
    }

    virtual void Update(double dt) override
    {
        ren.Render();
    }

    void RotateCamera(float xOffset, float yOffset) {
        glm::vec3 pos = cam->GetPosition();
        float len = glm::length(pos);

        // find the new position for the camera
        float rotspd = .05f * len;
        glm::vec3 rightOffset = cam->GetRight() * -xOffset * rotspd;
        glm::vec3 upOffset = cam->GetUp() * yOffset * rotspd;
        glm::vec3 newPos = glm::normalize(pos + upOffset + rightOffset) * len;

        // ensure there isn't flipping at the poles
        float d = glm::dot(glm::normalize(newPos), glm::vec3(0, 1, 0));
        if (1 - abs(d) > 0.01f)
        {
            cam->SetPosition(newPos);
            cam->LookAt({ 0, 0, 0 });
        }
    }

    virtual void OnKey(int key, int scancode, int action, int mods) override {
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (key == GLFW_KEY_R)
        {
            cam->SetPosition({ 0, 0, -5 });
            cam->LookAt({ 0, 0, 0 });
        }
    }

    virtual void OnMousePos(double xpos, double ypos) override {
        if (isDragging) {
            // Calculate the offset of the cursor from the last position
            float xOffset = xpos - lastMousePos.x;
            float yOffset = ypos - lastMousePos.y; // Invert Y-axis if needed

            // Update the position of the dragged object (e.g., camera, object, etc.)
            RotateCamera(xOffset, yOffset);

            // Update the last cursor position for the next frame
            lastMousePos = glm::vec2(xpos, ypos);
        }
    }

    virtual void OnMouseButton(int button, int action, int mods) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                isDragging = true;
                lastMousePos = GetCursorPosition();
            }
            else if (action == GLFW_RELEASE) {
                isDragging = false;
            }
        }
    }

    virtual void OnScroll(double xoffset, double yoffset) override {
        float len = glm::length(cam->GetPosition());
        cam->SetPosition(glm::normalize(cam->GetPosition()) * glm::clamp(len + -1.0f * (float)yoffset, 1.0f, 100.0f));

    }
};

int main() {
    MyApp app;
    

    if (app.Init("Shader Ideas"))
    {
        app.Run();
        app.Cleanup();
    }
    else
    {
        Console::Error("Unable to run the app");
    }
}
