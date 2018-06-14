#include"../inc/common.h"
#include "../inc/shader_m.h"
#include <iostream>

#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

unsigned int lightVAO;
glGenVertexArrays(1, &lightVAO);
glBindVertexArray(lightVAO);
// ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ���ӵ�VBO�������Ѿ���������ȷ�������嶥������
glBindBuffer(GL_ARRAY_BUFFER, VBO);
// ���õ�������Ķ������ԣ������ǵĵ���˵����ֻ��λ�����ݣ�
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	FragColor = vec4(lightColor * objectColor, 1.0);
}

// �ڴ�֮ǰ��Ҫ�������� use ��Ӧ����ɫ���������趨uniform��
lightingShader.use();
lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

#version 330 core
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0); // ���������ĸ�����ȫ������Ϊ1.0
}

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
model = glm::mat4();
model = glm::translate(model, lightPos);
model = glm::scale(model, glm::vec3(0.2f));

lampShader.use();
// ����ģ�͡���ͼ��ͶӰ����uniform
...
// ���Ƶ����������
glBindVertexArray(lightVAO);
glDrawArrays(GL_TRIANGLES, 0, 36);