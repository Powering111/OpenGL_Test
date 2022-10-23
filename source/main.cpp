#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Texture.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H


GLFWwindow* window;
struct Character {
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};
std::map<wchar_t, Character> characters;
glm::mat4 projection;
bool key_pressed[1024];
glm::vec3 model_position = glm::vec3(400.0f, 300.0f, 0.0f);
float model_scale = 50;
int width = 800, height = 600;
float progress_fill = 1.0f;
unsigned int textVAO, textVBO;
FT_Face fontface;
SpriteRenderer* spriteRenderer;

static void errorCallback(int errorCode, const char* errDescription) {
	std::cerr << "Error: " << errorCode << std::endl << errDescription << std::endl;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, 1);
		}
		key_pressed[key] = true;
	}
	else if(action==GLFW_RELEASE) {
		key_pressed[key] = false;
	}
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		ypos = height - ypos;
		model_position = glm::vec3(xpos, ypos, 0);
	}
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) {
		progress_fill += 0.05f;
		if (progress_fill > 1.0f) { 
			progress_fill = 1.0f; 
		}
	}
	if (yoffset < 0) {
		progress_fill -= 0.05f;
		if (progress_fill < 0) {
			progress_fill = 0;
		}
	}
}
void processEvent(double deltaTime) {

	if (key_pressed[GLFW_KEY_UP]) {
		model_position.y += 100*deltaTime;
	}
	if (key_pressed[GLFW_KEY_DOWN]) {
		model_position.y -= 100*deltaTime;
	}
	if (key_pressed[GLFW_KEY_LEFT]) {
		model_position.x -= 100*deltaTime;
	}
	if (key_pressed[GLFW_KEY_RIGHT]) {
		model_position.x += 100*deltaTime;
	}
	if (key_pressed[GLFW_KEY_A]) {
		model_scale += 50 * deltaTime;
	}
	if (key_pressed[GLFW_KEY_S]) {
		model_scale -= 50*deltaTime;
		if (model_scale < 1) model_scale = 1;
	}
	if (key_pressed[GLFW_KEY_Z]) {
		progress_fill -= 0.8f*deltaTime;
		if (progress_fill < 0) {
			progress_fill = 0;
		}
	}
	if (key_pressed[GLFW_KEY_X]) {
		progress_fill += 0.8f*deltaTime;
		if (progress_fill > 1.0f) {
			progress_fill = 1.0f;
		}
	}
}

static void framebuffersizeCallback(GLFWwindow* window, int width, int height) {
	std::cout << "frame buffer size callback of " << width << ", " << height << std::endl;
	glViewport(0,0,width, height);
	::width = width;
	::height = height;
	spriteRenderer->setViewport(width, height);
}

void load_font_char(wchar_t c) {
	if (FT_Load_Char(fontface, c, FT_LOAD_RENDER)) {
		std::cerr << "폰트 로딩 실패 : "<< c << std::endl;
		exit(EXIT_FAILURE);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
		fontface->glyph->bitmap.width,
		fontface->glyph->bitmap.rows,
		0, GL_RED, GL_UNSIGNED_BYTE,
		fontface->glyph->bitmap.buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	Character character = {
		texture,
		glm::ivec2(fontface->glyph->bitmap.width, fontface->glyph->bitmap.rows),
		glm::ivec2(fontface->glyph->bitmap_left, fontface->glyph->bitmap_top),
		static_cast<unsigned int>(fontface->glyph->advance.x)
	};
	characters.insert(std::pair<wchar_t, Character>(c, character));
}

void renderText(Shader& shader, std::wstring text, float x, float y, float scale, glm::vec3 color) {
	shader.Use();
	shader.setVector3f("textColor",color.x, color.y, color.z);
	shader.setMatrix4f("projection", projection);

	glBindVertexArray(textVAO);

	std::wstring::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		auto it = characters.find(*c);
		Character ch;
		if (it == characters.end()) {
			load_font_char(wchar_t(*c));
		}
		ch = characters[wchar_t(*c)];
		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;
		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		glBindTexture(GL_TEXTURE_2D, ch.textureID);

		//update buffer
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance>>6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void initGL() {
	// glfw initialization

	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) {
		std::cerr << "GLFW 초기화 실패" << std::endl;
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "HELLO", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// openGL loading
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "GL 로드 실패" << std::endl;
		exit(EXIT_FAILURE);
	}

	// glfw setting
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, framebuffersizeCallback);
	glfwSwapInterval(1);
}

int main() {
	initGL();

	// shader initialization
	Shader textshader("shaders/textvertexshader.glsl", "shaders/textfragmentshader.glsl");
	
	// font initialization
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "Freetype 초기화 실패" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	if (FT_New_Face(ft, "assets/NanumGothic.ttf", 0, &fontface)) {
		std::cerr << "폰트 로딩 실패" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	FT_Set_Pixel_Sizes(fontface, 0, 48);

	// VAO initialization for text
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*24, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Shader shader1,shader2;
	shader1.load("shaders/vertexshader.glsl", "shaders/fragmentshader.glsl");
	shader2.load("shaders/progressvertex.glsl", "shaders/progressfragment.glsl");
	//sprite renderer
	spriteRenderer = new SpriteRenderer(shader1,width,height);
	double lastTime = glfwGetTime();
	double accumulatedTime = 0;
	int index = 0;
	int frameCount = 22;
	Sprite sprite1("assets/coin_sheet.png", frameCount);
	Sprite sprite2("assets/progressbar.png");
	Sprite sprite3("assets/progressbar_fill.png");


	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.3125, 0.3438, 0.4219, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		double time = glfwGetTime();
		double deltaTime = time - lastTime;
		lastTime = time;

		accumulatedTime += deltaTime;
		if (accumulatedTime >= 0.1) {
			sprite1.index++;
			accumulatedTime -= 0.1;
		}
		
		spriteRenderer->draw_center(sprite1, model_position, glm::vec3(model_scale, model_scale, 1));
		

		glm::vec3 progressbar_position(0, 570.0f,0);
		glm::vec3 progressbar_scale(200.0f, 20.0f, 1.0f);
		spriteRenderer->draw_rect(sprite2, progressbar_position, progressbar_scale);
		spriteRenderer->draw_rect(sprite3, progressbar_position, progressbar_scale,0,progress_fill);
		renderText(textshader, L"Health Point", progressbar_position.x, progressbar_position.y +22.0f, 0.3f, glm::vec3(1.0f, 0, 0));
		projection = glm::ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);
		renderText(textshader, L"hello world!", 30.0f, 280.0f, 1.0f,glm::vec3(1.0f, 0.5f, 0.5f));
		glfwSwapBuffers(window);
		glfwPollEvents();
		processEvent(deltaTime);
	}
	delete spriteRenderer;
	FT_Done_Face(fontface);
	FT_Done_FreeType(ft);
	glfwTerminate();
	return 0;
}