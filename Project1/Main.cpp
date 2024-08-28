#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Copy&Paste shader scene
// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

using namespace std;

int main() {

	//iniciar glfw usada criar janela
	glfwInit();

	//dizer a glfw qual versão (major e minor) do OpenGL esta sendo utilizada, e o perfil(pacote de funções do openGL) desejado
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	/*criar um objeto janela passando os parâmetros de largura, altura, nome, se é fullscreen ou não, e um quinto parâmetro irrelevante 
	para o projeto no momento por isso NULL*/
	GLFWwindow* window = glfwCreateWindow(800, 800, "First Project", NULL, NULL);

	//Condição para caso a abertura da janela falhe
	if (window == NULL) {
		cout<<"Failed to create GLW window"<<endl;
		glfwTerminate();
		return -1;
	}

	//mandar usar a janela criada, fazendo a janela fazer parte do contexto atual
	glfwMakeContextCurrent(window);

	// Carregar configurações necessárias para OpenGL(para adicionar cor a janela)
	gladLoadGL();

	// Setar área da janela que desejo renderizar glViewport(bottom-left coordenadas e top-left coordenadas)
	glViewport(0, 0, 800, 800);


	// Create a value(reference) to store our vertex shader in, GLuint is the opengl version of an unsigned integer(positive)
	// glCreateShader() is to create the shader and get the reference value
	// GL_VERTEX_SHADER is to specify the type of shader we want
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Fit our shader the source code we copy&paste
	// glShaderSource(reference value, specify we're only using one string for the whole shader, point to the source code, doesn't matter here);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// GPU can't understand the source code so we'll have to compile it right now into machine code
	glCompileShader(vertexShader);

	// Doing the same thing for the fragment shader only replace "vertex" with "fragment"
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Wrap our shaders into a shader program to use them
	GLuint shaderProgram = glCreateProgram(); // no need specify because there is just one type of shader program
	// Attach shader to a shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//wrap up the shader program
	glLinkProgram(shaderProgram);
	// Delete the shaders we've created before because they're already in the program itself
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Make a array of data type gl float (normal is allowed but can differ in size from the floats that openGL uses, so is safer)
	// Every 3 gl floats will represent one coordinate, in this case the coordinates were choose to create a equilateral triangle, we can choose whatever
	// as long it is between -1 and 1
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		- 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
	};

	GLuint indices[] = {
		0,3,5, // Lower left triangle
		3,2,4, // Lower right triangle
		5,4,1 // Upper triangle
	};


	// Tell opengl how interpret our vertices
	// creating a vertex buffer object where we’ll store our vertex data, VBO is a array of vertices, but here we just need one
	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO); // has to be generate always before the VBO!!

	//create the buffer object, 1 as the first object because we only have 1 3d object
	glGenBuffers(1,&VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Bind the object, GL_ARRAY_BUFFER is the type we need to use for the vertex buffer (there's other type in the opengl documentation).
	// glBindBuffer(type of buffer, VBO,total size of the data in bytes, actual data itself, specify the use of this data);
	// We chosse between STREAM (means that the vertices will be modified once and used a few times), 
	//  STATIC(means that the vertices will be modified once and used many times) 
	//  and DYNAMIC(means that the vertices will be modified multiple times and used many many times). We need to specify these to improve performance.
	// We also choose between DRAW(means that the vertices will be modified and be used to draw an image on the screen), READ and COPY.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Tell opengl where to find our packed object
	// This stores pointers to one or more VBOs, and tells opengl how to interpret them.
	// VAOs exist in order to quickly be able to switch between different VBOs.
	// *Added back with VBO object*

	//Configurate it so that opengl knows how to read the VBO
	// A Vertex Attribute is a way of communicating with a Vertex Shader from the outside.
	// Stride of our vertices = amount of data between each vertex
	// Offset = pointer to where our vertices begin in the array
	// glVertexAttribPointer(index of the Vertex Atribute we want to use,how many values we have per vertex,type of values we have, if we have the coordinates as integers, stride of our vertices, offset-our vertices begin at the start of the array)
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3*sizeof(float), (void*)0);

	// enable to use our vertex
	glEnableVertexAttribArray(0);

	// *Optional: to be 100% sure you won't accidentally change VBO or VBA with a function
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	/**/
	// front buffer = where information is being read to display in the screen
	// back buffer = where information is being writen

	// preparar para limpar a cor do buffer e colocar uma nova cor
	// nesse caso navy blue, utilizando decimal RGB unidade, o último valor é o alpha, onde 1 é opaco e 0 é completamente transparente
	glClearColor(0.07f, 0.13f, 0.17f, 01.0f);

	// Executar o comando que dizemos para preparar, nesse caso especificamos o comando do buffer de cor (existem tipos de buffer)
	glClear(GL_COLOR_BUFFER_BIT);
	
	// A cor esta no buffer que não esta sendo mostrado na tela, entao temos que trocar os buffers
	glfwSwapBuffers(window);

	
	//criar um loop para a janela fechar somente ao clicar no botão de fechar(quando uma função especificamente dizer pra fechar), senão ela aparecerá e sumirá rapidamente
	while (!glfwWindowShouldClose(window)) {
		
		glClearColor(0.07f, 0.13f, 0.17f, 01.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram); //ativate the shader program we created
		// Bind the VAO so that we're telling OpenGL that we want to use this one, it's more usefull when we have more VAOs
		glBindVertexArray(VAO);
		// For the drawing function itself, specifying the type of primitive(triangles), starting index of the vertices(0) and the amount 
		// of vertices we want to draw(3)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Make sure we swap the buffers so that the image gets updated each frame.
		glfwSwapBuffers(window);


		//processar todos os PollEvents (janela aparecer, resize, etc), senão a janela não responderá
		glfwPollEvents();
	}

	// Delete objects that we created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);


	//deletar a janela criada antes da função terminar
	glfwDestroyWindow(window);

	//finalizar glfw antes da função terminar
	glfwTerminate();
	return 0;
}