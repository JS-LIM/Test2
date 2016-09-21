#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "math.h"
#include <stdbool.h>

const int width = 800;
const int height = 800;

float* pixels = new float[width*height * 3];

bool Large_Circle1(const double x, const double y); //���ǹ����� �Լ�����
bool Small_Circle2(const double x, const double y); //���ǹ����� �Լ�����

void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

// scratched from https://courses.engr.illinois.edu/ece390/archive/archive-f2000/mp/mp4/anti.html
// see 'Rasterization' part.

void drawrowLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
{
	for (int i = i0; i <= i1; i++)
	{
		const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0; 

		drawPixel(i, j, red, green, blue);
	}
}

void drawcolLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue) // �������� �����ϴ� �Լ�
{
	for (int j = j0; j <= j1; j++)
	{
		const int i = (i1 - i0)*(j - j0) / (j1 - j0) + i0; //���� ������ ���� �Լ����� i,j�� �ٲ� �и� 0�϶� �����ȵǴ� ��츦 ����

		drawPixel(i, j, red, green, blue);
	}
}

void paintsquare()//��ĥ�� �簢���� ����� �Լ�
{
	const int i_center=620, j_center = 620; //������ǥ ����
	const int thickness = 100;

	for (int j = j_center - thickness; j < j_center + thickness; j++)
	{
		for (int i = i_center - thickness; i < j_center + thickness; i++)
		{
			drawPixel(i, j, 1.0f, 0.0f, 1.0f); //2�������� ���� ����,���� ���̰� 100�� ���簢�� ����
		}
	}
}

void drawcircle1()
{
	const int x = 900, y = 300;
	for (int j = y; j > 0; j--)
	{
		for (int i = 0; i < x; i++)
		{
			if (Large_Circle1(i, j) == true) //true ���� �޴´�.(���� ����)
			{
				drawPixel(i, j, 0.0f, 0.0f, 1.0f); //���� ������ �ݺ����� ���� ä���.
			}
		}
	}
}

void drawcircle2()
{
	const int x = 900, y = 300;
	for (int j = y; j > 0; j--)
	{
		for (int i = 0; i < x; i++)
		{
			if (Small_Circle2(i, j) == true) //true ���� �޴´�.(���� ����)
			{
				drawPixel(i, j, 1.0f, 1.0f, 1.0f); //���� ������ �ݺ����� ���� ä���.
			}
		}
	}
}

bool Large_Circle1(const double x, const double y)  //����ī���� �̿�
{
	const double x_c = 650;
	const double y_c = 150;
	const double r = 100;
	const double f = (x - x_c)*(x - x_c) + (y - y_c)*(y - y_c) - (r*r);

		if (f > 0)
			return false;
		else
			return true;
}

bool Small_Circle2(const double x, const double y) 
{
	const double x_c = 650;
	const double y_c = 150;
	const double r = 98;
	const double f = (x - x_c)*(x - x_c) + (y - y_c)*(y - y_c) - (r*r);

	if (f > 0)
		return false;
	else
		return true;
}

void drawthicknerline() // �β����ִ� ���� ����� �Լ� ����
{
	int i0 = 50, i1 = 250; //���۵Ǵ� x��ǥ ����
	int j0 = 400, j1 = 600; //���۵Ǵ� y��ǥ ����
	for (int n = 0; n < 20; n++) //20���� �ݺ�
	{
		for (int j = j0; j < j1; j++)
		{
			for (int i = i0; i < i1; i++)
			{
				drawrowLine(i0, j0, i1, j1, 1.0f, 0.0f, 0.0f); //���� ����
			}
		}
		j0++; //j0�� j1�� �������� ���Ⱑ ���� ���� 20�� �����Ͽ� ��ġ�� �Ѵ�.
		j1++;
	}
}

/*void drawOnPixelBuffer()
{
	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	std::fill_n(pixels, width*height * 3, 1.0f);	// white background

													//for (int i = 0; i<width*height; i++) {
													//	pixels[i * 3 + 0] = 1.0f; // red 
													//	pixels[i * 3 + 1] = 1.0f; // green
													//	pixels[i * 3 + 2] = 1.0f; // blue
													//}

	const int i = rand() % width, j = rand() % height;
	drawPixel(i, j, 0.0f, 0.0f, 0.0f);

	// drawing a line
	//TODO: anti-aliasing

	//TODO: try moving object
}*/

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Style Programming", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			pixels[(i + width* j) * 3 + 0] = 1.0f;
			pixels[(i + width* j) * 3 + 1] = 1.0f;
			pixels[(i + width* j) * 3 + 2] = 1.0f; //2�� for���� ���� ��� ��� ����
		}
	}

	drawthicknerline(); //�β����ִ� ��

	drawrowLine(300, 450, 500, 450, 0.0f, 0.0f, 0.0f);
	drawrowLine(300, 650, 500, 650, 0.0f, 0.0f, 0.0f);
	drawcolLine(300, 450, 300, 650, 0.0f, 0.0f, 0.0f);
	drawcolLine(500, 450, 500, 650, 0.0f, 0.0f, 0.0f);//�� ���� ���� ȣ���Ͽ� �簢���� �����.

	paintsquare();//��ĥ�� �簢��


	drawrowLine(50, 50, 250, 50, 0.0f, 1.0f, 0.0f);
	drawrowLine(50, 50, 150, 200, 0.0f, 1.0f, 0.0f);
	drawrowLine(150, 200, 250, 50, 0.0f, 1.0f, 0.0f);//�� ���� ���� ȣ���Ͽ� �ﰢ���� �����.

	drawcircle1();
	drawcircle2();//�� ���� ȣ���Ͽ� ū���� �ȿ� �������� ��ġ���� ���� ������ �����Ѵ�.

	drawrowLine(300, 50, 400, 50, 0.0f, 0.0f, 0.0f);
	drawrowLine(250, 150, 300, 50, 0.0f, 0.0f, 0.0f);
	drawrowLine(400, 50, 450, 150, 0.0f, 0.0f, 0.0f);
	drawrowLine(250, 150, 350, 250, 0.0f, 0.0f, 0.0f);
	drawrowLine(350, 250, 450, 150, 0.0f, 0.0f, 0.0f);//�ټ� ���� ���� ȣ���Ͽ� �������� �����.
	
							  /* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);

		//TODO: RGB struct
		//Make a pixel drawing function
		//Make a line drawing function

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array 

	return 0;
}