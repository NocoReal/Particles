#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Vector3.h"
#include <iostream>
#include <vector>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <random> // For generating random values
#include <cmath>

using namespace std;

struct Particle {
    Vector3 Position = Vector3(0, 0);
    Vector3 Velocity = Vector3(0, 0);
    float Mass = 1.0f;
    float Size = 0.005f; // Fixed size for all particles as requested

    // Constructor with parameters for easier initialization
    Particle(Vector3 pos, Vector3 vel, float mass = 1.0f)
        : Position(pos), Velocity(vel), Mass(mass) {}
};

const double HOST_FRAMTETIME = 0.016666666666;
const GLint WIDTH = 1280, HEIGHT = 720;
const float ASPECT_RATIO = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);

void RenderCircle(int resolution, vector<Vector3>& outVertices, vector<GLuint>& outIndices) {
    resolution = abs(resolution);
    if (resolution <= 2) resolution = 3; // Prevent invalid resolutions

    vector<Vector3> vertexPos(resolution);
    vertexPos[0] = Vector3::Up(); // First vertex position

    float theta = 360.0f / resolution; // Ensure float division

    outVertices.resize(resolution);

    // Set vertex positions
    for (int i = 1; i < resolution; i++) {
        vertexPos[i] = vertexPos[i - 1].RotatedZ(theta);
    }

    outIndices.clear();

    // Generate the base triangles first
    for (int i = 1; i < resolution - 1; i++) {
        outIndices.push_back(0);
        outIndices.push_back(i);
        outIndices.push_back(i + 1);
    }

    // Last triangle connecting back to the start
    outIndices.push_back(0);
    outIndices.push_back(resolution - 1);
    outIndices.push_back(1);

    // Now store the vertex positions into outVertices
    for (int i = 0; i < resolution; i++) {
        outVertices[i] = vertexPos[i];
    }
}

// Structure to hold the geometry for a particle shape
struct ParticleShape {
    vector<Vector3> localVertices;
    vector<GLuint> indices;

    // Constructor to initialize with a circle of given resolution
    ParticleShape(int resolution = 1) {
        RenderCircle(resolution, localVertices, indices);
    }
};

// Update all particles in the system
void UpdateParticles(vector<Particle>& particles, float deltaTime) {
    for (auto& particle : particles) {
        particle.Position = particle.Position + particle.Velocity * deltaTime;
    }
}

// Generate vertices for all particles, with aspect ratio correction
void GenerateParticleVertices(const vector<Particle>& particles, const ParticleShape& shape,
    vector<Vector3>& outVertices) {
    outVertices.clear();

    // For each particle, add its vertices
    for (const auto& particle : particles) {
        for (const auto& localVertex : shape.localVertices) {
            // Apply size scaling and position offset
            // Correct for aspect ratio by scaling X coordinates
            Vector3 scaledVertex = localVertex * particle.Size;
            scaledVertex.y *= ASPECT_RATIO; // Correct for aspect ratio

            Vector3 worldVertex = scaledVertex + particle.Position;
            outVertices.push_back(worldVertex);
        }
    }
}

// Generate indices for all particles
void GenerateParticleIndices(int particleCount, const ParticleShape& shape, vector<GLuint>& outIndices) {
    outIndices.clear();
    int verticesPerParticle = shape.localVertices.size();

    for (int p = 0; p < particleCount; p++) {
        int baseIndex = p * verticesPerParticle;

        for (size_t i = 0; i < shape.indices.size(); i++) {
            outIndices.push_back(baseIndex + shape.indices[i]);
        }
    }
}

// Create random particles with normalized positions accounting for aspect ratio
vector<Particle> CreateRandomParticles(int count) {
    random_device rd;
    mt19937 gen(rd());

    // Adjust X range to account for aspect ratio
    uniform_real_distribution<float> posXDist(-0.8f , 0.8f );
    uniform_real_distribution<float> posYDist(-0.8f, 0.8f);
    uniform_real_distribution<float> velDist(-2.f, 2.f);

    vector<Particle> particles;

    for (int i = 0; i < count; i++) {
        Vector3 pos(posXDist(gen), posYDist(gen), 0.0f);
        Vector3 vel(velDist(gen), velDist(gen), 0.0f);

        particles.emplace_back(pos, vel);
    }

    return particles;
}

int main() {
    
    ParticleShape particleShape(6);

    // Create multiple particles
    int particleCount = pow(2,10);  // Number of particles
    vector<Particle> particles = CreateRandomParticles(particleCount);

    // Vectors for rendered geometry
    vector<Vector3> particleVertices;
    vector<GLuint> particleIndices;

    // Generate initial vertices and indices for all particles
    GenerateParticleVertices(particles, particleShape, particleVertices);
    GenerateParticleIndices(particleCount, particleShape, particleIndices);

#pragma region Initialize Window
    if (!glfwInit()) {
        glfwTerminate();
        cout << "glfwInit failed" << endl;
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Multiple Particles", NULL, NULL);
    glfwSetWindowSizeLimits(window, WIDTH, HEIGHT, WIDTH, HEIGHT);
    if (!window) {
        glfwTerminate();
        cout << "window creation failed" << endl;
        return 2;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(window);
        glfwTerminate();
        cout << "glewInit failed" << endl;
        return 3;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);
#pragma endregion

    Shader shaderProgram("default.vert", "default.frag");

    // Flatten vertices for OpenGL
    vector<GLfloat> vertexData;
    for (const auto& vertex : particleVertices) {
        vertexData.push_back(vertex.x);
        vertexData.push_back(vertex.y);
        vertexData.push_back(vertex.z);
    }

    // Convert particleIndices into data for EBO
    GLuint* indexData = particleIndices.data();

    // Set up VBO, VAO, and EBO
    VAO VAO1;
    VAO1.Bind();

    // Create VBO with initial data
    VBO VBO1(vertexData.data(), vertexData.size() * sizeof(GLfloat));

    // Create EBO with initial data
    EBO EBO1(indexData, particleIndices.size() * sizeof(GLuint));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0);

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    double prevTime = glfwGetTime();
    int frameCount = 0;

    // Add boundary conditions (normalized for aspect ratio)
    auto checkBoundaries = [](Particle& p) {
        // Simple boundary check and bounce, normalized for aspect ratio
        if (p.Position.x > 1.0f || p.Position.x < -1.0f) {
            p.Position.x = -p.Position.x;
        }
        if (p.Position.y > 1.0f || p.Position.y < -1.0f) {
            p.Position.y = -p.Position.y;
        }
        };

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Render
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();
        VAO1.Bind();

        // Update particle physics
        double crntTime = glfwGetTime();
        double deltaTime = crntTime - prevTime;

        if (crntTime - prevTime >= HOST_FRAMTETIME) {
            // Update particle positions
            UpdateParticles(particles, HOST_FRAMTETIME);

            // Apply boundary checks for each particle
            for (auto& particle : particles) {
                checkBoundaries(particle);
            }

            // Regenerate vertices based on new particle positions
            GenerateParticleVertices(particles, particleShape, particleVertices);

            // Flatten vertices for OpenGL
            vector<GLfloat> updatedVertexData;
            for (const auto& vertex : particleVertices) {
                updatedVertexData.push_back(vertex.x);
                updatedVertexData.push_back(vertex.y);
                updatedVertexData.push_back(vertex.z);
            }

            // Update VBO with new vertex data
            VBO1.UpdateData(updatedVertexData.data(), updatedVertexData.size() * sizeof(GLfloat));

            prevTime = crntTime;
            frameCount++;
        }

        // Draw the particles
        glDrawElements(GL_TRIANGLES, particleIndices.size(), GL_UNSIGNED_INT, 0);

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}