
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

using namespace std;
//este es el funcional.... NO LO BORRES

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 color;

    uniform mat4 transform;

    void main()
    {
        gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        color = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 color;
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(color, 1.0);
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

    // Comprobar errores de compilación del vertex shader
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

    // Comprobar errores de compilación del fragment shader
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

    // Coordenadas y vértices del cubo
    float vertices[] = {
        // Cara frontal
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Rojo
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // Verde
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // Azul
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // Amarillo
        // Cara trasera
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // Magenta
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // Cian
         0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, // Gris
        -0.5f,  0.5f, -0.5f,  0.8f, 0.2f, 0.5f  // Rosa
    };

    // Índices de los triángulos que forman el cubo
    unsigned int indices[] = {
        0, 1, 2,    // Primer triángulo (cara frontal)
        2, 3, 0,    // Segundo triángulo (cara frontal)
        4, 5, 6,    // Tercer triángulo (cara trasera)
        6, 7, 4,    // Cuarto triángulo (cara trasera)
        0, 3, 7,    // Quinto triángulo (lado izquierdo)
        7, 4, 0,    // Sexto triángulo (lado izquierdo)
        1, 2, 6,    // Séptimo triángulo (lado derecho)
        6, 5, 1,    // Octavo triángulo (lado derecho)
        3, 2, 6,    // Noveno triángulo (lado superior)
        6, 7, 3,    // Décimo triángulo (lado superior)
        0, 1, 5,    // Undécimo triángulo (lado inferior)
        5, 4, 0     // Duodécimo triángulo (lado inferior)
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

    // Especificar los atributos de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Especificar los atributos del color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Desenlazar el VAO
    glBindVertexArray(0);

    // Establecer la velocidad de rotación
    float rotationSpeed = 0.5f;

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        // Actualizar lógica del juego

        // Renderizar
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Obtener la ubicación de la transformación en el shader
        int transformLoc = glGetUniformLocation(shaderProgram, "transform");

        // Incrementar el ángulo de rotación
        float angle = glfwGetTime() * rotationSpeed;

        // Matriz de rotación
        float rotationMatrix[] = {
            cos(angle), 0.0f, -sin(angle), 0.0f,
            0.0f,       1.0f, 0.0f,        0.0f,
            sin(angle), 0.0f, cos(angle),  0.0f,
            0.0f,       0.0f, 0.0f,        1.0f
        };

        // Pasar la matriz de transformación al shader
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, rotationMatrix);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Intercambiar buffers y comprobar eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // Limpiar los recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


