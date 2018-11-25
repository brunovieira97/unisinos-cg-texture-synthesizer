#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <STB/stb_image_write.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <Classes/Shader.h>
#include <Classes/Image.h>


const unsigned int
	WINDOW_HEIGHT = 16,
	WINDOW_WIDTH = 16;

const char*
	WINDOW_TITLE = "Texture Synthesizer";

const char*
	ERROR_ARGUMENT_COUNT = R"(
		Please run the application informing the following parameters:
			Original texture image path
			Pre-synthesized image path
			Output path for the result file
	)";

GLFWwindow *window;


float DistanceBetweenColors(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);
unsigned char* GenerateImage(Image* sampleImage, Image* preSynImage);
int InitializeGLFW();


float DistanceBetweenColors(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2) {

	float distR = ((float)r2 - r1);
	float distG = ((float)g2 - g1);
	float distB = ((float)b2 - b1);
	return (sqrt(pow(distR, 2) + pow(distG, 2) + pow(distG, 2)));
}

unsigned char* GenerateImage(Image* sampleImage, Image* preSynImage) {

	unsigned char* sampleData = sampleImage -> data;
	int arrSize = preSynImage -> width * preSynImage -> height * 3;
	unsigned char* newImage = new unsigned char[arrSize];

	for (int i = 0; i < arrSize; i++) {
		newImage[i] = preSynImage -> data[i];
	}

	int imageTempX = 0, imageTempY = 0;
	int tempX = 0, tempY = 0;

	glm::vec2 winnerPixelPos;
	float winnerPontuation;
	float pontuation = 0;

	// For every pixel
	for (int y = 0; y < preSynImage->height; y++) {
		for (int x = 0; x < preSynImage->width; x++) {

			winnerPontuation = INT_MAX;

			// For every pixel in sample image
			for (int sY = 0; sY < sampleImage->height; sY++) {
				for (int sX = 0; sX < sampleImage->width; sX++) {

					pontuation = 0.0f;

					// Check neighbors
					for (int nY = -2; nY <= 0; nY++) {

						imageTempY = y + nY;
						if (imageTempY < 0) {
							imageTempY = preSynImage->height + imageTempY;
						} else if (imageTempY >= preSynImage->height) {
							imageTempY = abs(preSynImage->height - imageTempY);
						}

						tempY = sY + nY;
						if (tempY < 0) {
							tempY = sampleImage->height + tempY;
						} else if (tempY >= sampleImage->height) {
							tempY = abs(sampleImage->height - tempY);
						}

						for (int nX = -2; nX <= 2; nX++) {
							if (nY == 0 && nX > 0) { // Last line going only until the proper pixel
								break;
							}

							imageTempX = x + nX;
							if (imageTempX < 0) {
								imageTempX = preSynImage->width + imageTempX;
							} else if (imageTempX >= preSynImage->width) {
								imageTempX = abs(preSynImage->width - imageTempX);
							}

							tempX = sX + nX;
							if (tempX < 0) {
								tempX = sampleImage->width + tempX;
							} else if (tempX >= sampleImage->width) {
								tempX = abs(sampleImage->width - tempX);
							}

							// Compare pixels from sample with preSyn
							// 0 - R // 1 - G // 2 - B
							// Multiplying for 3 because every pixel has 3 components

							int newImagePos = imageTempX * 3 + (imageTempY * preSynImage->width * 3);
							int tempPos = tempX * 3 + (tempY * sampleImage->width * 3);

							float dist = DistanceBetweenColors(
								newImage[newImagePos + 0],
								newImage[newImagePos + 1],
								newImage[newImagePos + 2],

								sampleData[tempPos + 0],
								sampleData[tempPos + 1],
								sampleData[tempPos + 2]
							);

							pontuation += dist;
						}
					}
					// End of neighbors

					// Assign new winner if pontuation was less
					if (pontuation < winnerPontuation) {
						winnerPixelPos.x = sX;
						winnerPixelPos.y = sY;
						winnerPontuation = pontuation;
					}

				}
			}
			// End of sample check

			int pixelPos = x * 3 + (y * preSynImage->width * 3);
			int winerPixelPos = winnerPixelPos.x * 3 + (winnerPixelPos.y * sampleImage->width * 3);

			newImage[pixelPos + 0] = sampleData[winerPixelPos + 0];
			newImage[pixelPos + 1] = sampleData[winerPixelPos + 1];
			newImage[pixelPos + 2] = sampleData[winerPixelPos + 2];

		}
	}

	return newImage;
}

int InitializeGLFW() {
	if (!glfwInit()) {
		std::cerr << "Could not start GLFW." << std::endl;
		
		return EXIT_FAILURE;
	}

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	if (!window) {
		std::cerr << "Could not open application's window using GLFW." << std::endl;
		
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	
	if (!glewInit()) {
		std::cerr << "Failed to initialize GLEW." << std::endl;
		
		return EXIT_FAILURE;
	}

	glEnable(GL_DEPTH_TEST);

	return EXIT_SUCCESS;
}

int main(int argc, char** argv) {

	if (argc < 3) {
		std::cerr << ERROR_ARGUMENT_COUNT << std::endl;

		return EXIT_FAILURE;
	}

	if (InitializeGLFW() != EXIT_SUCCESS) {
		std::cerr << "Error while initializing GLFW and GLEW. Application will terminate." << std::endl;

		exit(EXIT_FAILURE);
	}

	stbi_set_flip_vertically_on_load(true);

	Image sampleImage;
	sampleImage.Load(argv[1], false);

	Image preSynImage;
	preSynImage.Load(argv[2], false);

	unsigned char* newData = GenerateImage(&sampleImage, &preSynImage);

	std::cout << "Starting synthesis " << std::endl;

	stbi_write_jpg(argv[3], sampleImage.width, sampleImage.height, 3, &newData[0], 100);
	
	std::cout << "Finished writing to " << argv[3] << std::endl;

	glfwTerminate();
	
	return EXIT_SUCCESS;
}
