#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace std;

// Funci�n de devoluci�n de llamada para cambiar el tama�o del framebuffer
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    uniform mat4 transform;

    void main()
    {
        gl_Position = transform * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;

    uniform sampler2D texture1;

    void main()
    {
        FragColor = texture(texture1, TexCoord);
    }
)";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "VENTANA JUEGO", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // Compilar el vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Comprobar errores de compilaci�n del vertex shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Error al compilar el vertex shader: " << infoLog << endl;
    }

    // Compilar el fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Comprobar errores de compilaci�n del fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "Error al compilar el fragment shader: " << infoLog << endl;
    }

    // Crear el programa de shader
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Comprobar errores de enlace del programa de shader
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Error al enlazar el programa de shader: " << infoLog << endl;
    }

    // Eliminar los shaders una vez que se han enlazado al programa
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Coordenadas y v�rtices del cubo
    float vertices[] = {
        // Cara frontal
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Inferior izquierdo
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Inferior derecho
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Superior derecho
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Superior izquierdo
        // Cara trasera
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Inferior izquierdo
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Inferior derecho
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Superior derecho
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // Superior izquierdo
    };

    // �ndices de los tri�ngulos que forman el cubo
    unsigned int indices[] = {
        0, 1, 2,    // Primer tri�ngulo (cara frontal)
        2, 3, 0,    // Segundo tri�ngulo (cara frontal)
        4, 5, 6,    // Tercer tri�ngulo (cara trasera)
        6, 7, 4,    // Cuarto tri�ngulo (cara trasera)
        0, 3, 7,    // Quinto tri�ngulo (lado izquierdo)
        7, 4, 0,    // Sexto tri�ngulo (lado izquierdo)
        1, 2, 6,    // S�ptimo tri�ngulo (lado derecho)
        6, 5, 1,    // Octavo tri�ngulo (lado derecho)
        3, 2, 6,    // Noveno tri�ngulo (lado superior)
        6, 7, 3,    // D�cimo tri�ngulo (lado superior)
        0, 1, 5,    // Und�cimo tri�ngulo (lado inferior)
        5, 4, 0     // Duod�cimo tri�ngulo (lado inferior)
    };

    // Crear los objetos de Vertex Buffer y Element Buffer
    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Crear y enlazar el Vertex Array Object (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Enlazar el Vertex Buffer Object (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Enlazar el Element Buffer Object (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Especificar los atributos de los v�rtices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Especificar los atributos de las coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Desenlazar el VAO
    glBindVertexArray(0);

    // Establecer la velocidad de rotaci�n
    float rotationSpeed = 0.5f;

    // Cargar la textura
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Establecer los par�metros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Cargar la imagen de la textura
    int width, height, channels;
    string path = "plano.png";
    const char* pathChar = &path[0];
    stbi_set_flip_vertically_on_load(true);
    unsigned char* ndata = stbi_load("plano.png", &width, &height, &channels, 0);
    unsigned char* data = ndata;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        cout << "IMAGEN LISTA" << endl;
    }
    else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool STOP = true;

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        // Actualizar l�gica del juego
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("UI DEL JUEGO");
        ImGui::Text("ROTACION");
        // Renderizar
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Obtener la ubicaci�n de la transformaci�n en el shader
        int transformLoc = glGetUniformLocation(shaderProgram, "transform");

        // Incrementar el �ngulo de rotaci�n
        float angle = static_cast<float>(glfwGetTime()) * rotationSpeed;


        // Matriz de rotaci�n
        if (STOP) {
            float cosAngle = cos(angle);
            float sinAngle = sin(angle);

            float rotationMatrix[16] = {
                cosAngle, 0.0f, -sinAngle, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                sinAngle, 0.0f, cosAngle, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            // Pasar la matriz de transformaci�n al shader
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, rotationMatrix);
        }


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        ImGui::Checkbox("DETENER", &STOP);

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpiar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
